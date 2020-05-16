#define NOMINMAX

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TGConfig.h"
#include "TGUtils.h"
#include "TGVulkanCanvas.h"

#include <vulkan/vulkan.hpp>

#include <chrono>
#include <functional>
#include <set>
#include <shaderc/shaderc.hpp>

const std::vector<const char*> kValidationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> kDeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<Vertex> kVertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> kIndices = {
	0, 1, 2, 2, 3, 0
};

const int kMaxFramesInFlight = 2;

#ifdef _DEBUG
const bool kEnableValidationLayers = true;
#else
const bool kEnableValidationLayers = false;
#endif

vk::VertexInputBindingDescription Vertex::binding_description() {
	vk::VertexInputBindingDescription desc(
		0,
		sizeof(Vertex),
		vk::VertexInputRate::eVertex
	);

	return desc;
}

std::array<vk::VertexInputAttributeDescription, 2> Vertex::attribute_descriptions() {

	std::array<vk::VertexInputAttributeDescription, 2> descriptions;

	// inPosition
	descriptions[0].binding = 0;
	descriptions[0].location = 0;
	descriptions[0].format = vk::Format::eR32G32Sfloat;
	descriptions[0].offset = offsetof(Vertex, pos);

	// inColor 
	descriptions[1].binding = 0;
	descriptions[1].location = 1;
	descriptions[1].format = vk::Format::eR32G32B32Sfloat;
	descriptions[1].offset = offsetof(Vertex, color);

	return descriptions;
}

TGVulkanCanvas::TGVulkanCanvas(
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name
) : wxWindow(parent, id, pos, size, style, name),
	vulkan_initialized_(false), instance_(nullptr),
	surface_(nullptr), physical_device_(nullptr),
	device_(nullptr), swapchain_(nullptr),
	render_pass_(nullptr), pipeline_layout_(nullptr),
	graphics_pipeline_(nullptr), cmd_pool_(nullptr)
{
	Bind(wxEVT_PAINT, &TGVulkanCanvas::on_paint, this);
	//Bind(wxEVT_SIZE, &TGVulkanCanvas::on_resize, this);

	initialize_vulkan(size);
}

TGVulkanCanvas::~TGVulkanCanvas() {

	cleanup_swapchain();

	device_.destroyDescriptorSetLayout(descriptor_set_layout_);

	device_.destroyBuffer(index_buffer_);
	device_.freeMemory(index_buffer_memory_);
	device_.destroyBuffer(vertex_buffer_);
	device_.freeMemory(vertex_buffer_memory_);

	device_.destroyCommandPool(copy_pool_);
	device_.destroyCommandPool(cmd_pool_);

	device_.destroy();

	instance_.destroySurfaceKHR(surface_);
	instance_.destroy();
}

void TGVulkanCanvas::cleanup_swapchain() {

	for (size_t i = 0; i < swap_framebuffers_.size(); i++) {
		device_.destroyFramebuffer(swap_framebuffers_[i]);
	}
	swap_framebuffers_.resize(0);

	device_.freeCommandBuffers(cmd_pool_, cmd_buffers_);
	
	device_.destroyPipeline(graphics_pipeline_);
	device_.destroyPipelineLayout(pipeline_layout_);
	device_.destroyRenderPass(render_pass_);

	for (size_t i = 0; i < swapchain_image_views_.size(); i++) {
		device_.destroyImageView(swapchain_image_views_[i]);
	}
	swapchain_image_views_.resize(0);

	device_.destroySwapchainKHR(swapchain_);

	for (size_t i = 0; i < uniform_buffers_.size(); i++) {
		device_.destroyBuffer(uniform_buffers_[i]);
		device_.freeMemory(uniform_buffers_memory_[i]);
	}

	device_.destroyDescriptorPool(descriptor_pool_);
}

void TGVulkanCanvas::recreate_swapchain() {
	device_.waitIdle();

	cleanup_swapchain();

	wxSize size = GetSize();
	create_swapchain(size);
	create_image_views();
	create_render_pass();
	create_graphics_pipeline();
	create_framebuffers();
	create_uniform_buffers();
	create_descriptor_pool();
	allocate_descriptor_sets();
	create_command_buffers();
}

void TGVulkanCanvas::initialize_vulkan(const wxSize& size) {

	std::vector<const char*> required_exts = {
		"VK_KHR_surface",
		"VK_KHR_win32_surface"
	};

	std::vector<const char*> required_layers;
	if (kEnableValidationLayers) {
		required_layers = kValidationLayers;
	}

	create_instance(required_exts, required_layers);
	create_window_surface();
	select_physical_device();
	create_logical_device();
	create_swapchain(size);
	create_image_views();
	create_render_pass();
	create_descriptor_set_layout();
	create_graphics_pipeline();
	create_framebuffers();
	create_command_pool();
	create_vertex_buffers();
	create_index_buffers();
	create_uniform_buffers();
	create_descriptor_pool();
	allocate_descriptor_sets();
	create_command_buffers();
	create_sync_objects();
}

void TGVulkanCanvas::create_instance(
	const std::vector<const char*>& exts,
	const std::vector<const char*>& layers
) {
	
	vk::ApplicationInfo app_info(
		"TerrainGen",
		VK_MAKE_VERSION(2, 0, 0),
		"TerrainGenEngine",
		VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_2
	);

	vk::InstanceCreateInfo info(
		{},
		&app_info,
		static_cast<uint32_t>(layers.size()),
		layers.data(),
		static_cast<uint32_t>(exts.size()),
		exts.data()
	);

	instance_ = vk::createInstance(info);
}

void TGVulkanCanvas::create_window_surface() {
	
#ifdef _WIN32
	vk::Win32SurfaceCreateInfoKHR info(
		{},
		GetModuleHandle(nullptr),
		GetHWND()
	);

	surface_ = instance_.createWin32SurfaceKHR(info);
#else
#error TerrainGen currently only supports Windows. Sorry!
#endif
}

void TGVulkanCanvas::select_physical_device() {
	std::vector<vk::PhysicalDevice> devices = instance_
		.enumeratePhysicalDevices();


	auto it_device = std::find_if(
		devices.begin(),
		devices.end(),
		std::bind(
			std::mem_fn(&TGVulkanCanvas::is_device_suitable),
			this,
			std::placeholders::_1
		)
	);

	if (it_device != devices.end()) {
		physical_device_ = *it_device;
		queue_family_indices_ = get_queue_families(physical_device_);
	}
	else {
		throw std::runtime_error("Unable to find suitable vulkan device");
	}
}

bool TGVulkanCanvas::is_device_suitable(
	vk::PhysicalDevice device
) const noexcept {

	QueueFamilies indices = get_queue_families(device);
	
	bool valid = indices.is_complete();
	valid &= device_supports_extensions(device);

	if (valid) {
		SwapchainSupport support = get_swapchain_support(device);

		valid &= !support.formats.empty() &&
			!support.present_modes.empty();
	}

	return valid;
}

QueueFamilies TGVulkanCanvas::get_queue_families(
	vk::PhysicalDevice device
) const noexcept {

	QueueFamilies families;

	std::vector<vk::QueueFamilyProperties> available_families = device
		.getQueueFamilyProperties();

	int i = 0;
	for (const vk::QueueFamilyProperties& family : available_families) {
		if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
			families.graphics_family = i;
		}

		if (device.getSurfaceSupportKHR(i, surface_)) {
			families.present_family = i;
		}

		if (families.is_complete()) {
			break;
		}

		i++;
	}

	return families;
}

bool TGVulkanCanvas::device_supports_extensions(
	vk::PhysicalDevice device
) const noexcept {

	std::vector<vk::ExtensionProperties> available_exts = device
		.enumerateDeviceExtensionProperties();

	std::set<std::string> required_exts(
		kDeviceExtensions.begin(),
		kDeviceExtensions.end()
	);

	for (const vk::ExtensionProperties& ext : available_exts) {
		required_exts.erase(static_cast<std::string>(ext.extensionName));
	}

	return required_exts.empty();
}

SwapchainSupport TGVulkanCanvas::get_swapchain_support(
	vk::PhysicalDevice device
) const noexcept {

	SwapchainSupport support_details;

	support_details.capabilities = device.getSurfaceCapabilitiesKHR(surface_);
	support_details.formats = device.getSurfaceFormatsKHR(surface_);
	support_details.present_modes = device.getSurfacePresentModesKHR(surface_);

	return support_details;
}

void TGVulkanCanvas::create_logical_device() {

	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = {
		queue_family_indices_.graphics_family.value(),
		queue_family_indices_.present_family.value()
	};

	float queue_priority = 1.0f;
	for (uint32_t family : unique_queue_families) {
		vk::DeviceQueueCreateInfo queue_info(
			{},
			family,
			1,
			&queue_priority
		);

		queue_create_infos.push_back(queue_info);
	}

	vk::PhysicalDeviceFeatures device_features;

	uint32_t layer_count = 0;
	const char* const* layers = nullptr;

	if (kEnableValidationLayers) {
		layer_count = static_cast<uint32_t>(kValidationLayers.size());
		layers = kValidationLayers.data();
	}

	vk::DeviceCreateInfo device_info(
		{},
		static_cast<uint32_t>(queue_create_infos.size()),
		queue_create_infos.data(),
		layer_count,
		layers,
		static_cast<uint32_t>(kDeviceExtensions.size()),
		kDeviceExtensions.data(),
		&device_features
	);

	device_ = physical_device_.createDevice(device_info);

	graphics_queue_ = device_.getQueue(
		queue_family_indices_.graphics_family.value(), 0
	);

	present_queue_ = device_.getQueue(
		queue_family_indices_.present_family.value(), 0
	);
}

void TGVulkanCanvas::create_swapchain(const wxSize& size) {

	SwapchainSupport support_details = get_swapchain_support(physical_device_);

	swap_format_ = select_swap_surface_format(support_details.formats);
	swap_present_mode_ = select_swap_present_mode(support_details.present_modes);
	swap_extent_ = select_swap_extent(support_details.capabilities, size);

	uint32_t img_count = support_details.capabilities.minImageCount + 1;
	if (support_details.capabilities.maxImageCount > 0 &&
		img_count > support_details.capabilities.maxImageCount) {
		img_count = support_details.capabilities.maxImageCount;
	}

	uint32_t queue_families[] = {
		queue_family_indices_.graphics_family.value(),
		queue_family_indices_.present_family.value()
	};
	vk::SharingMode sharing_mode = vk::SharingMode::eExclusive;
	uint32_t queue_index_count = 0;

	if (queue_family_indices_.graphics_family.value() !=
		queue_family_indices_.present_family.value()) {
		
		sharing_mode = vk::SharingMode::eConcurrent;
		queue_index_count = 2;
	}

	vk::SwapchainCreateInfoKHR info(
		{},
		surface_,
		img_count,
		swap_format_.format,
		swap_format_.colorSpace,
		swap_extent_,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		sharing_mode,
		queue_index_count,
		queue_families,
		support_details.capabilities.currentTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		swap_present_mode_,
		VK_TRUE
	);

	swapchain_ = device_.createSwapchainKHR(info);
	swapchain_images_ = device_.getSwapchainImagesKHR(swapchain_);
}

vk::SurfaceFormatKHR TGVulkanCanvas::select_swap_surface_format(
	const std::vector<vk::SurfaceFormatKHR>& formats
) const noexcept {

	vk::SurfaceFormatKHR chosen_format = formats[0];

	// See if desired format is supported
	for (const vk::SurfaceFormatKHR& f : formats) {
		if (f.format == vk::Format::eB8G8R8A8Unorm &&
			f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

			chosen_format = f;
			break;

		}
	}

	return chosen_format;
}

vk::PresentModeKHR TGVulkanCanvas::select_swap_present_mode(
	const std::vector<vk::PresentModeKHR>& modes
) const noexcept {

	// Guaranteed to be supported
	vk::PresentModeKHR chosen_mode = vk::PresentModeKHR::eFifo;

	for (const vk::PresentModeKHR& mode : modes) {
		// Prefered
		if (mode == vk::PresentModeKHR::eMailbox) {
			chosen_mode = mode;
			break;
		}
	}

	return chosen_mode;
}

vk::Extent2D TGVulkanCanvas::select_swap_extent(
	const vk::SurfaceCapabilitiesKHR& capabilities,
	const wxSize& size
) const noexcept {

	vk::Extent2D chosen_extent = capabilities.currentExtent;

	// If uint max, we can select whatever
	if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
		chosen_extent = {
			static_cast<uint32_t>(size.GetWidth()),
			static_cast<uint32_t>(size.GetHeight())
		};

		chosen_extent.width = std::max(
			capabilities.minImageExtent.width, 
			std::min(capabilities.maxImageExtent.width, chosen_extent.width)
		);

		chosen_extent.height = std::max(
			capabilities.minImageExtent.height, 
			std::min(capabilities.maxImageExtent.height, chosen_extent.height)
		);
	}

	return chosen_extent;
}

void TGVulkanCanvas::create_image_views() {
	
	swapchain_image_views_.reserve(swapchain_images_.size());

	std::transform(
		swapchain_images_.begin(),
		swapchain_images_.end(),
		std::back_inserter(swapchain_image_views_),
		std::bind(
			std::mem_fn(&TGVulkanCanvas::view_from_image),
			this,
			std::placeholders::_1
		)
	);
}

vk::ImageView TGVulkanCanvas::view_from_image(const vk::Image& img) {
	
	vk::ComponentMapping mapping = {};
	mapping.r = vk::ComponentSwizzle::eIdentity;
	mapping.g = vk::ComponentSwizzle::eIdentity;
	mapping.b = vk::ComponentSwizzle::eIdentity;
	mapping.a = vk::ComponentSwizzle::eIdentity;

	vk::ImageSubresourceRange range = {};
	range.aspectMask = vk::ImageAspectFlagBits::eColor;
	range.baseMipLevel = 0;
	range.levelCount = 1;
	range.baseArrayLayer = 0;
	range.layerCount = 1;

	vk::ImageViewCreateInfo info(
		{},
		img,
		vk::ImageViewType::e2D,
		swap_format_.format,
		mapping,
		range
	);

	return device_.createImageView(info);
}

void TGVulkanCanvas::create_render_pass() {

	vk::AttachmentDescription color_attachment(
		{},
		swap_format_.format,
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	);

	vk::AttachmentReference color_ref(
		0, vk::ImageLayout::eColorAttachmentOptimal
	);

	vk::SubpassDescription subpass(
		{},
		vk::PipelineBindPoint::eGraphics,
		0,
		nullptr,
		1,
		&color_ref
	);

	vk::SubpassDependency dep(
		{},
		{},
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		{},
		vk::AccessFlagBits::eColorAttachmentWrite
	);

	vk::RenderPassCreateInfo info(
		{},
		1,
		&color_attachment,
		1,
		&subpass,
		1,
		&dep
	);

	render_pass_ = device_.createRenderPass(info);
}

void TGVulkanCanvas::create_descriptor_set_layout() {

	vk::DescriptorSetLayoutBinding ubo_layout_binding(
		0,									// location = 0
		vk::DescriptorType::eUniformBuffer, // buffer type
		1,									// descriptor count
		vk::ShaderStageFlagBits::eVertex	// shader stage(s)
	);

	vk::DescriptorSetLayoutCreateInfo layout_info(
		{},				// flags
		1,				// # of bindings
		&ubo_layout_binding
	);

	descriptor_set_layout_ = device_.createDescriptorSetLayout(layout_info);
}

void TGVulkanCanvas::create_graphics_pipeline() {

	std::string shader_dir = std::string(TG_SHADER_DIR) + "/";
	std::vector<char> vert_code = TGUtils::read_bytes(shader_dir + "vert.spv");
	std::vector<char> frag_code = TGUtils::read_bytes(shader_dir + "frag.spv");

	vk::ShaderModule vert_module = create_shader_module(vert_code);
	vk::PipelineShaderStageCreateInfo vert_stage(
		{},
		vk::ShaderStageFlagBits::eVertex,
		vert_module,
		"main"
	);

	vk::ShaderModule frag_module = create_shader_module(frag_code);
	vk::PipelineShaderStageCreateInfo frag_stage(
		{},
		vk::ShaderStageFlagBits::eFragment,
		frag_module,
		"main"
	);

	vk::PipelineShaderStageCreateInfo stages[] = {
		vert_stage,
		frag_stage
	};

	auto vert_binding_desc = Vertex::binding_description();
	auto vert_attr_desc = Vertex::attribute_descriptions();

	vk::PipelineVertexInputStateCreateInfo vert_input(
		{},
		1,
		&vert_binding_desc,
		static_cast<uint32_t>(vert_attr_desc.size()),
		vert_attr_desc.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo assembly(
		{},
		vk::PrimitiveTopology::eTriangleList,
		VK_FALSE
	);

	vk::Viewport viewport(
		0.0f,
		0.0f,
		(float)swap_extent_.width,
		(float)swap_extent_.height,
		0.0f,
		1.0f
	);

	vk::Offset2D scissor_offset = { 0, 0 };
	vk::Rect2D scissor(
		scissor_offset,
		swap_extent_
	);

	vk::PipelineViewportStateCreateInfo vp_state(
		{},
		1,
		&viewport,
		1,
		&scissor
	);

	vk::PipelineRasterizationStateCreateInfo rasterizer(
		{},
		VK_FALSE,
		VK_FALSE,
		vk::PolygonMode::eFill,
		vk::CullModeFlagBits::eBack,
		vk::FrontFace::eCounterClockwise,
		VK_FALSE
	);
	rasterizer.lineWidth = 1.0f;

	// No multisampling for now
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    // When fragment shader returns a color, it needs to be combined with
    // what is already in framebuffer. This specifies how that happens.
    // Contains configuration per attached framebuffer
    vk::ColorComponentFlags write_mask = vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA;
    vk::PipelineColorBlendAttachmentState blend_attachment;
    blend_attachment.colorWriteMask = write_mask;
    blend_attachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo color_blend_info;
    color_blend_info.logicOpEnable = VK_FALSE;
    color_blend_info.attachmentCount = 1;
    color_blend_info.pAttachments = &blend_attachment;

    vk::PipelineLayoutCreateInfo pipeline_layout_info(
        {},
        1,
        &descriptor_set_layout_,
        0,
        nullptr
    );

    pipeline_layout_ = device_.createPipelineLayout(pipeline_layout_info);

    vk::GraphicsPipelineCreateInfo graphics_info(
        {},
        2,
        stages,
        &vert_input,
        &assembly,
        {},
        &vp_state,
        &rasterizer,
        &multisampling,
        {},
        &color_blend_info,
        {},
        pipeline_layout_,
        render_pass_,
        0,
        {}
    );

    graphics_pipeline_ = device_.createGraphicsPipeline({}, graphics_info);

    device_.destroyShaderModule(frag_module);
    device_.destroyShaderModule(vert_module);
}

vk::ShaderModule TGVulkanCanvas::create_shader_module(std::vector<uint32_t> code) {

	vk::ShaderModuleCreateInfo info(
		{},
		code.size(),
		code.data()
	);
	
	vk::ShaderModule module;
	module = device_.createShaderModule(info);

	return module;
}

vk::ShaderModule TGVulkanCanvas::create_shader_module(std::vector<char> code) {

	vk::ShaderModuleCreateInfo info(
		{},
		code.size(),
		reinterpret_cast<const uint32_t*>(code.data())
	);
	
	vk::ShaderModule module;
	module = device_.createShaderModule(info);

	return module;
}

void TGVulkanCanvas::create_framebuffers() {
	swap_framebuffers_.reserve(swapchain_image_views_.size());

	std::transform(
		swapchain_image_views_.begin(),
		swapchain_image_views_.end(),
		std::back_inserter(swap_framebuffers_),
		std::bind(
			std::mem_fn(&TGVulkanCanvas::framebuffer_from_image_view),
			this,
			std::placeholders::_1
		)
	);
}

vk::Framebuffer TGVulkanCanvas::framebuffer_from_image_view(
	const vk::ImageView& img_view
) {
	vk::ImageView attachments[] = {
		img_view
	};

	vk::FramebufferCreateInfo framebuf_info(
		{},
		render_pass_,
		1,
		attachments,
		swap_extent_.width,
		swap_extent_.height,
		1
	);

	return device_.createFramebuffer(framebuf_info);
}

void TGVulkanCanvas::create_command_pool() {

	vk::CommandPoolCreateInfo pool_info(
		{},
		queue_family_indices_.graphics_family.value()
	);

	cmd_pool_ = device_.createCommandPool(pool_info);

	// Transient pool for mem copy operations
	// & other short-lived cmd buffers
	vk::CommandPoolCreateInfo copy_info(
		vk::CommandPoolCreateFlagBits::eTransient,
		queue_family_indices_.graphics_family.value()
	);
	copy_pool_ = device_.createCommandPool(copy_info);
}

void TGVulkanCanvas::create_vertex_buffers() {

	vk::DeviceSize buf_size = sizeof(kVertices[0]) * kVertices.size();

	// Staging buffer
	vk::Buffer staging_buf;
	vk::DeviceMemory staging_memory;

	create_buffer(
		buf_size,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		staging_buf,
		staging_memory
	);

	void* data = device_.mapMemory(staging_memory, 0, buf_size);
	memcpy(data, kVertices.data(), (size_t)buf_size);
	device_.unmapMemory(staging_memory);

	create_buffer(
		buf_size,
		vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		vertex_buffer_,
		vertex_buffer_memory_
	);

	copy_buffer(staging_buf, vertex_buffer_, buf_size);

	device_.destroyBuffer(staging_buf);
	device_.freeMemory(staging_memory);
}

void TGVulkanCanvas::create_index_buffers() {
	vk::DeviceSize buf_size = sizeof(kIndices[0]) * kIndices.size();

	vk::Buffer staging_buf;
	vk::DeviceMemory staging_memory;

	create_buffer(
		buf_size,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		staging_buf,
		staging_memory
	);

	void* data = device_.mapMemory(staging_memory, 0, buf_size);
	memcpy(data, kIndices.data(), buf_size);
	device_.unmapMemory(staging_memory);

	create_buffer(
		buf_size,
		vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		index_buffer_,
		index_buffer_memory_
	);

	copy_buffer(staging_buf, index_buffer_, buf_size);

	device_.destroyBuffer(staging_buf);
	device_.freeMemory(staging_memory);
}

void TGVulkanCanvas::create_uniform_buffers() {
	vk::DeviceSize buf_size = sizeof(UniformBufferObject);

	uniform_buffers_.resize(swapchain_images_.size());
	uniform_buffers_memory_.resize(swapchain_images_.size());

	for (size_t i = 0; i < uniform_buffers_.size(); i++) {
		create_buffer(
			buf_size,
			vk::BufferUsageFlagBits::eUniformBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			uniform_buffers_[i],
			uniform_buffers_memory_[i]
		);
	}
}

void TGVulkanCanvas::create_buffer(
	vk::DeviceSize size,
	vk::BufferUsageFlags usage,
	vk::MemoryPropertyFlags props,
	vk::Buffer& buffer,
	vk::DeviceMemory& buffer_memory
) {
	vk::BufferCreateInfo buf_info(
		{},
		size,
		usage,
		vk::SharingMode::eExclusive
	);

	buffer = device_.createBuffer(buf_info);

	vk::MemoryRequirements mem_reqs = device_.getBufferMemoryRequirements(buffer);

	uint32_t mem_type_index = find_memory_type(
		mem_reqs.memoryTypeBits,
		props
	);

	vk::MemoryAllocateInfo alloc_info(
		mem_reqs.size,
		mem_type_index
	);

	buffer_memory = device_.allocateMemory(alloc_info);

	device_.bindBufferMemory(buffer, buffer_memory, 0);
}

uint32_t TGVulkanCanvas::find_memory_type(
	uint32_t type_filter,
	vk::MemoryPropertyFlags props
) {
	
	vk::PhysicalDeviceMemoryProperties mem_props =
		physical_device_.getMemoryProperties();

	for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
		bool props_met = (mem_props.memoryTypes[i].propertyFlags & props) == props;

		if (type_filter & (1 << i) && props_met) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type");
}

void TGVulkanCanvas::copy_buffer(
	vk::Buffer src,
	vk::Buffer dest,
	vk::DeviceSize size
) {
	vk::CommandBufferAllocateInfo alloc_info(
		copy_pool_,
		vk::CommandBufferLevel::ePrimary,
		1
	);

	vk::CommandBuffer cmd_buffer;
	if (device_.allocateCommandBuffers(&alloc_info, &cmd_buffer) != vk::Result::eSuccess) {
		throw std::runtime_error("Unable to create cmd buffer for copy");
	}

	vk::CommandBufferBeginInfo beg_info(
		vk::CommandBufferUsageFlagBits::eOneTimeSubmit
	);

	cmd_buffer.begin(beg_info);
	vk::BufferCopy copy_region(
		0,
		0,
		size
	);

	std::array<vk::BufferCopy, 1> regions = { copy_region };
	cmd_buffer.copyBuffer(src, dest, regions);
	cmd_buffer.end();

	vk::SubmitInfo sub_info;
	sub_info.commandBufferCount = 1;
	sub_info.pCommandBuffers = &cmd_buffer;

	graphics_queue_.submit(1, &sub_info, vk::Fence(nullptr));
	graphics_queue_.waitIdle();

	device_.freeCommandBuffers(copy_pool_, 1, &cmd_buffer);
}

void TGVulkanCanvas::create_descriptor_pool() {
	vk::DescriptorPoolSize pool_size(
		vk::DescriptorType::eUniformBuffer,
		static_cast<uint32_t>(uniform_buffers_.size())
	);

	vk::DescriptorPoolCreateInfo info(
		{},
		static_cast<uint32_t>(uniform_buffers_.size()),
		1,
		&pool_size
	);

	descriptor_pool_ = device_.createDescriptorPool(info);
}

void TGVulkanCanvas::allocate_descriptor_sets() {
	std::vector<vk::DescriptorSetLayout> layouts(
		uniform_buffers_.size(),
		descriptor_set_layout_
	);

	vk::DescriptorSetAllocateInfo alloc_info(
		descriptor_pool_,
		static_cast<uint32_t>(uniform_buffers_.size()),
		layouts.data()
	);

	descriptor_sets_.reserve(uniform_buffers_.size());
	descriptor_sets_ = device_.allocateDescriptorSets(alloc_info);

	for (size_t i = 0; i < descriptor_sets_.size(); i++) {
		vk::DescriptorBufferInfo buf_info(
			uniform_buffers_[i],
			0,
			sizeof(UniformBufferObject)
		);

		vk::WriteDescriptorSet write(
			descriptor_sets_[i],
			0,
			0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&buf_info
		);

		device_.updateDescriptorSets(1, &write, 0, nullptr);
	}
}

void TGVulkanCanvas::create_command_buffers() {
	cmd_buffers_.reserve(swap_framebuffers_.size());

    vk::CommandBufferAllocateInfo alloc_info(cmd_pool_, vk::CommandBufferLevel::ePrimary, swap_framebuffers_.size());
    cmd_buffers_ = device_.allocateCommandBuffers(alloc_info);

    for (size_t i = 0; i < cmd_buffers_.size(); i++) {
        vk::CommandBufferBeginInfo beg_info; // default values fine

        cmd_buffers_[i].begin(beg_info);

        vk::Rect2D render_area;
        render_area.offset.setX(0);
        render_area.offset.setY(0);
        render_area.extent = swap_extent_;

        std::array<float, 4> color = { 0.0f, 0.0f, 0.0f, 1.0f };
        vk::ClearValue clear_color(color);

        vk::RenderPassBeginInfo pass_info(
            render_pass_,
            swap_framebuffers_[i],
            render_area,
            1,
            &clear_color
        );

        cmd_buffers_[i].beginRenderPass(pass_info, vk::SubpassContents::eInline);
        cmd_buffers_[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline_);

        std::array<vk::Buffer, 1> vertex_buffers = { vertex_buffer_ };
        std::array<vk::DeviceSize, 1> offset = { 0 };

        cmd_buffers_[i].bindVertexBuffers(0, vertex_buffers, offset);
        cmd_buffers_[i].bindIndexBuffer(index_buffer_, 0, vk::IndexType::eUint16);

        cmd_buffers_[i].bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,   // bind point
            pipeline_layout_,                   // pipeline layout
            0,                                  // first set index
            1,                                  // # of sets
            &descriptor_sets_[i],               // sets to bind
            0,                                  // dynamic offset count
            nullptr                             // dynamic offsets
        );
        uint32_t index_count = static_cast<uint32_t>(kIndices.size());
        cmd_buffers_[i].drawIndexed(index_count, 1, 0, 0, 0);
        cmd_buffers_[i].endRenderPass();

        cmd_buffers_[i].end();
    }
}

void TGVulkanCanvas::create_sync_objects() {
	img_available_semaphores_.resize(kMaxFramesInFlight);
	render_finished_semaphores_.resize(kMaxFramesInFlight);
	in_flight_fences_.resize(kMaxFramesInFlight);
	imgs_in_flight_.resize(kMaxFramesInFlight);

	vk::SemaphoreCreateInfo sem_info;

	vk::FenceCreateInfo fence_info(
		vk::FenceCreateFlagBits::eSignaled
	);

	for (size_t i = 0; i < kMaxFramesInFlight; i++) {
		img_available_semaphores_[i] = device_.createSemaphore(sem_info);
		render_finished_semaphores_[i] = device_.createSemaphore(sem_info);
		in_flight_fences_[i] = device_.createFence(fence_info);
	}
}

void TGVulkanCanvas::update_uniform_buffer(uint32_t current_img) {
	static auto start_time = std::chrono::high_resolution_clock::now();

	auto current_time = std::chrono::high_resolution_clock::now();

	float delta = std::chrono::duration<float, std::chrono::seconds::period>(
		current_time - start_time).count();

	UniformBufferObject ubo{};

	ubo.model = glm::rotate(
		glm::mat4(1.0f),
		delta * glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	ubo.view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	ubo.proj = glm::perspective(
		glm::radians(45.0f),
		swap_extent_.width / (float)swap_extent_.height,
		0.1f,
		10.0f
	);

	ubo.proj[1][1] *= -1;

	void* data = device_.mapMemory(uniform_buffers_memory_[current_img], 0, sizeof(ubo));
	memcpy(data, &ubo, sizeof(ubo));
	device_.unmapMemory(uniform_buffers_memory_[current_img]);
}

void TGVulkanCanvas::on_paint(wxPaintEvent& e) {

	device_.waitForFences(in_flight_fences_[current_frame_], VK_TRUE, UINT64_MAX);

    uint32_t img_index;
    auto next_img = device_.acquireNextImageKHR(swapchain_, UINT64_MAX, img_available_semaphores_[current_frame_], {});

    if (next_img.result == vk::Result::eErrorOutOfDateKHR) {
        recreate_swapchain();
        return;
    }
    else if (next_img.result == vk::Result::eSuccess || next_img.result == vk::Result::eSuboptimalKHR) {
        img_index = next_img.value;
    }
    else {
        throw std::runtime_error("Failed to acquire next swapchain image");
    }

    if ((VkFence)imgs_in_flight_[img_index] != VK_NULL_HANDLE) {
        device_.waitForFences(imgs_in_flight_[img_index], VK_TRUE, UINT64_MAX);
    }
    imgs_in_flight_[img_index] = in_flight_fences_[current_frame_];

    vk::Semaphore wait_sems[] = {
        img_available_semaphores_[current_frame_]
    };

    vk::PipelineStageFlags wait_stages = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput
    };

    vk::Semaphore signal_sems[] = {
        render_finished_semaphores_[current_frame_]
    };

    update_uniform_buffer(img_index);

    vk::SubmitInfo submit(
        1,
        wait_sems,
        &wait_stages,
        1,
        &cmd_buffers_[img_index],
        1,
        signal_sems
    );

    device_.resetFences(in_flight_fences_[current_frame_]);

    graphics_queue_.submit(submit, in_flight_fences_[current_frame_]);

    vk::SwapchainKHR chains[] = { swapchain_ };
    vk::PresentInfoKHR present_info(
        1,
        signal_sems,
        1,
        chains,
        &img_index
    );

    auto present_result = present_queue_.presentKHR(present_info);

    if (present_result == vk::Result::eErrorOutOfDateKHR || present_result == vk::Result::eSuboptimalKHR) {
        recreate_swapchain();
    }
    else if (present_result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to present swapchain image");
    }

    current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;

}

void TGVulkanCanvas::on_resize(wxPaintEvent& e) {
	wxSize size = GetSize();

	if (size.GetWidth() == 0 || size.GetHeight() == 0) {
		return;
	}

	recreate_swapchain();
	wxRect refresh_rect(size);
	RefreshRect(refresh_rect, false);
}
