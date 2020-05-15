#define NOMINMAX

#include "TGConfig.h"
#include "TGUtils.h"
#include "TGVulkanCanvas.h"

#include <vulkan/vulkan.hpp>

#include <functional>
#include <set>
#include <shaderc/shaderc.hpp>

const std::vector<const char*> kValidationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> kDeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

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
	descriptions[0].binding = 0;
	descriptions[0].location = 1;
	descriptions[0].format = vk::Format::eR32G32B32Sfloat;
	descriptions[0].offset = offsetof(Vertex, color);

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
	//Bind(wxEVT_PAINT, &TGVulkanCanvas::on_paint, this);
	//Bind(wxEVT_SIZE, &TGVulkanCanvas::on_resize, this);

	initialize_vulkan(size);
}

TGVulkanCanvas::~TGVulkanCanvas() {

	device_.destroyPipeline(graphics_pipeline_);
	device_.destroyPipelineLayout(pipeline_layout_);
	device_.destroyRenderPass(render_pass_);

	for (size_t i = 0; i < swapchain_image_views_.size(); i++) {
		device_.destroyImageView(swapchain_image_views_[i]);
	}
	swapchain_image_views_.resize(0);

	device_.destroySwapchainKHR(swapchain_);

	instance_.destroySurfaceKHR(surface_);
	instance_.destroy();
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
	create_graphics_pipeline();
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
		VK_API_VERSION_1_0
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

	shaderc::Compiler compiler;
	shaderc::CompileOptions opts;

	std::string shader_dir = std::string(TG_SHADER_DIR) + "/";

	std::string vert_file = "shader.vert";
	auto vert_src = TGUtils::read_file(shader_dir + vert_file);

	shaderc::SpvCompilationResult vert_result = compiler.CompileGlslToSpv(
		vert_src,
		shaderc_shader_kind::shaderc_glsl_vertex_shader,
		vert_file.c_str(),
		opts
	);

	if (vert_result.GetCompilationStatus() != shaderc_compilation_status_success) {
		throw std::runtime_error("Failed to compile vertex shader");
	}

	std::string frag_file = "shader.frag";
	auto frag_src = TGUtils::read_file(shader_dir + frag_file);

	shaderc::SpvCompilationResult frag_result = compiler.CompileGlslToSpv(
		frag_src,
		shaderc_shader_kind::shaderc_glsl_fragment_shader,
		frag_file.c_str(),
		opts
	);

	if (frag_result.GetCompilationStatus() != shaderc_compilation_status_success) {
		throw std::runtime_error("Failed to compile fragment shader");
	}

	std::vector<uint32_t> vert_code(vert_result.begin(), vert_result.end());
	vk::ShaderModule vert_module = create_shader_module(vert_code);
	vk::PipelineShaderStageCreateInfo vert_stage(
		{},
		vk::ShaderStageFlagBits::eVertex,
		vert_module,
		"main"
	);

	std::vector<uint32_t> frag_code(frag_result.begin(), frag_result.end());
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

	vk::ShaderModule module = device_.createShaderModule(info);

	return module;
}
