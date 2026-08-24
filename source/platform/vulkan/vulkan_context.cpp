#include "vulkan_context.hpp"
#include "../../katha/core/config.hpp"

#include <SDL2/SDL_vulkan.h>

katha::vulkan_t vulkan_global_instance = {};
katha::vulkan_t *const katha::vulkan = &vulkan_global_instance;

katha::result_e katha::vulkan_t::init(const config_t& config, SDL_Window* window)
{
	result_e result = load_instance_proc_addresses();
	if (!check_result(result, "vulkan::load_instance_proc_addresses"))
	{
		return result;
	}
	
	return result_e::success;
}

void katha::vulkan_t::clear()
{}

PFN_vkVoidFunction katha::vulkan_t::get_proc_address(const char* proc_name) const
{
	if (nullptr == vkGetInstanceProcAddr)
	{
		log_line("error-vulkan: vkGetInstanceProcAddr is not loaded");
		return {};
	}

	PFN_vkVoidFunction proc = vkGetInstanceProcAddr(instance, proc_name);
	if (nullptr == proc)
	{
		log_line("error-vulkan: proc {s} not found", proc_name);
		return {};
	}

	return proc;
}

katha::result_e katha::vulkan_t::load_instance_proc_addresses()
{
	#define KATHA_VK_LOAD_PROC_I(proc)\
		if (!load_proc_addr(proc, #proc))\
		{\
			return result_e::error_vulkan;\
		}

	KATHA_VK_LOAD_PROC_I(vkEnumerateInstanceExtensionProperties);
	KATHA_VK_LOAD_PROC_I(vkEnumerateInstanceLayerProperties);

	return result_e::success;
}
