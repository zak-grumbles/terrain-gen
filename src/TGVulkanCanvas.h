#ifndef TG_VULKAN_CANVAS_H
#define TG_VULKAN_CANVAS_H

#include <wx/wx.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include <optional>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static vk::VertexInputBindingDescription binding_description();
	static std::array<vk::VertexInputAttributeDescription, 2> attribute_descriptions();
};

struct UniformBufferObject {
	alignas(16)glm::mat4 model;
	alignas(16)glm::mat4 view;
	alignas(16)glm::mat4 proj;
};

struct QueueFamilies {
	std::optional<uint32_t> graphics_family;
	std::optional<uint32_t> present_family;

	bool is_complete() {
		return graphics_family.has_value() && present_family.has_value();
	}
};

struct SwapchainSupport {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> present_modes;
};

class TGVulkanCanvas : public wxWindow {

private:
	vk::Instance instance_;
	vk::DebugUtilsMessengerEXT dbg_messenger_;
	vk::SurfaceKHR surface_;

	vk::PhysicalDevice physical_device_;
	QueueFamilies queue_family_indices_;
	vk::Device device_;
	vk::Queue graphics_queue_;
	vk::Queue present_queue_;

	vk::SurfaceFormatKHR swap_format_;
	vk::PresentModeKHR swap_present_mode_;
	vk::Extent2D swap_extent_;
	vk::SwapchainKHR swapchain_;
	std::vector<vk::Image> swapchain_images_;
	std::vector<vk::ImageView> swapchain_image_views_;

	vk::RenderPass render_pass_;

	vk::DescriptorSetLayout descriptor_set_layout_;
	vk::DescriptorPool descriptor_pool_;
	std::vector<vk::DescriptorSet> descriptor_sets_;

	vk::PipelineLayout pipeline_layout_;
	vk::Pipeline graphics_pipeline_;

	std::vector<vk::Framebuffer> swap_framebuffers_;

	vk::Buffer vertex_buffer_;
	vk::DeviceMemory vertex_buffer_memory_;
	vk::Buffer index_buffer_;
	vk::DeviceMemory index_buffer_memory_;

	std::vector<vk::Buffer> uniform_buffers_;
	std::vector<vk::DeviceMemory> uniform_buffers_memory_;

	vk::CommandPool cmd_pool_;
	std::vector<vk::CommandBuffer> cmd_buffers_;

	vk::CommandPool copy_pool_;

	std::vector<vk::Semaphore> img_available_semaphores_;
	std::vector<vk::Semaphore> render_finished_semaphores_;
	std::vector<vk::Fence> in_flight_fences_;
	std::vector<vk::Fence> imgs_in_flight_;
	size_t current_frame_ = 0;

	bool framebuffer_resized_ = false;
	bool vulkan_initialized_;
public:
	TGVulkanCanvas(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = "TGVulkanCanvasName"
	);

	virtual ~TGVulkanCanvas() noexcept;

private:

	void initialize_vulkan(const wxSize& size);

	void create_instance(
		const std::vector<const char*>& exts,
		const std::vector<const char*>& layers 
	);

	void create_window_surface();
	void select_physical_device();
	void create_logical_device();
	void create_swapchain(const wxSize& size);
	void create_image_views();
	void create_render_pass();
	void create_descriptor_set_layout();
	void create_graphics_pipeline();
	void create_framebuffers();
	void create_command_pool();
	void create_vertex_buffers();
	void create_index_buffers();
	void create_uniform_buffers();
	void create_descriptor_pool();
	void allocate_descriptor_sets();
	void create_command_buffers();
	void create_sync_objects();

	bool is_device_suitable(vk::PhysicalDevice device) const noexcept;
	QueueFamilies get_queue_families(vk::PhysicalDevice device) const noexcept;
	bool device_supports_extensions(vk::PhysicalDevice device) const noexcept;

	SwapchainSupport get_swapchain_support(
		vk::PhysicalDevice device
	) const noexcept;

	vk::SurfaceFormatKHR select_swap_surface_format(
		const std::vector<vk::SurfaceFormatKHR>& formats
	) const noexcept;

	vk::PresentModeKHR select_swap_present_mode(
		const std::vector<vk::PresentModeKHR>& modes
	) const noexcept;

	vk::Extent2D select_swap_extent(
		const vk::SurfaceCapabilitiesKHR& capabilities,
		const wxSize& size
	) const noexcept;

	vk::ImageView view_from_image(const vk::Image& image);

	vk::ShaderModule create_shader_module(std::vector<uint32_t> code);
	vk::ShaderModule create_shader_module(std::vector<char> code);

	vk::Framebuffer framebuffer_from_image_view(const vk::ImageView& img_view);

	void cleanup_swapchain();
	void recreate_swapchain();

	uint32_t find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags props);

	void create_buffer(
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		vk::MemoryPropertyFlags props,
		vk::Buffer& buffer, vk::DeviceMemory& buffer_memory
	);

	void copy_buffer(
		vk::Buffer src,
		vk::Buffer dest,
		vk::DeviceSize size
	);

	void update_uniform_buffer(uint32_t current_img);

	virtual void on_paint(wxPaintEvent& e);
	virtual void on_resize(wxPaintEvent& e);
};

#endif