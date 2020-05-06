#include "TGVulkanCanvas.h"

#include <vulkan/vulkan.hpp>

#include <set>

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

	initialize_vulkan();
}

TGVulkanCanvas::~TGVulkanCanvas() {
	instance_.destroySurfaceKHR(surface_);
	instance_.destroy();
}

void TGVulkanCanvas::initialize_vulkan() {

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
		TGVulkanCanvas::is_device_suitable
	);

	if (it_device != devices.end()) {
		physical_device_ = *it_device;
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
