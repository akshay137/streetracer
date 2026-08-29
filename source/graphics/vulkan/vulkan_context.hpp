#pragma once
#ifndef KATHA_PLATFORM_VULKAN_CONTEXT_H__
#define KATHA_PLATFORM_VULKAN_CONTEXT_H__ 1

#include "../graphics.hpp"
#include "../../config.hpp"

#include <vulkan/vulkan.h>

struct SDL_Window;

namespace katha
{
	struct vulkan_t
	{
		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

		VkSurfaceKHR surface = VK_NULL_HANDLE;

		// vk_device_t device = {};
		// vk_swapchain_t swapchain = {};

		result_e init(const config_t& config, SDL_Window* window);
		void clear();

		PFN_vkVoidFunction get_proc_address(const char* proc_name) const;
		result_e load_instance_proc_addresses();

		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = {};
		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = {};
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = {};
		PFN_vkCreateInstance vkCreateInstance = {};
		PFN_vkDestroyInstance vkDestroyInstance = {};

		PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = {};
		PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = {};

		template <typename PFN_T>
		bool load_proc_addr(PFN_T& out_proc, const char* name)
		{
			PFN_vkVoidFunction proc = get_proc_address(name);
			if (nullptr == proc)
			{
				return false;
			}
			out_proc = (PFN_T)proc;
			return true;
		}
	};

	extern vulkan_t *const vulkan;
}

#endif