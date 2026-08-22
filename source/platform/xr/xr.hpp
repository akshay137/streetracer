#pragma once
#ifndef KATHA_PLATFORM_XR_H__
#define KATHA_PLATFORM_XR_H__ 1

#if KATHA_XR

#include "../../katha/core.hpp"
#include "../../katha/graphics/framebuffer.hpp"
#include "../../katha/physics/transform.hpp"

#include "../gl/gl.hpp"

#include <SDL2/SDL_video.h>
#include <openxr/openxr.h>

namespace katha
{
	// TODO: Add checks to verify support of hardcoded values
	struct xr_t
	{
		constexpr static inline XrViewConfigurationType VIEW_CONFIGURATION_TYPE = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
		constexpr static inline uint32_t EYE_LEFT = 0;
		constexpr static inline uint32_t EYE_RIGHT = 1;

		graphics_api_e graphics_api = graphics_api_e::gl;

		XrInstance instance = XR_NULL_HANDLE;
		XrSystemId system_id = 0;
		XrSession session = XR_NULL_HANDLE;

		XrSessionState session_state = XR_SESSION_STATE_UNKNOWN;

		// We only support left & right view HMD
		constexpr static uint32_t VIEW_COUNT = 2;
		XrViewConfigurationView views[VIEW_COUNT] = {};

		struct swapchain_t
		{
			// We need only 3,
			// but could get more in minimum recommended
			constexpr static uint32_t MAX_IMAGE_COUNT = 8;
			XrSwapchain swapchain = XR_NULL_HANDLE;
			uvec2 size = {};

			format_e format = format_e::none;
			uint32_t image_count = 0;
			framebuffer_t framebuffers[MAX_IMAGE_COUNT] = {};
		};
		swapchain_t swapchains[VIEW_COUNT] = {}; // swapchain per view

		XrSpace space = XR_NULL_HANDLE;
		XrPosef poses[VIEW_COUNT] = {};
		XrFovf fovs[VIEW_COUNT] = {};

		uint32_t debug_layer_present:1 = 0;
		uint32_t hand_tracking_available:1 = 0;
		uint32_t session_running:1 = 0;

		bool check_result(const XrResult result, const char* message) const;

		result_e init(const config_t& config, SDL_Window* window);
		void clear();

		PFN_xrVoidFunction get_proc_address(const char* proc_name) const;
		const char** get_instance_extensions() const;

		result_e create_view();

		const char** get_instance_extensions_gl() const;
		result_e create_session_gl(const config_t& config, SDL_Window* window);
		result_e create_session_vulkan(const config_t& config);

		result_e get_opengl_swapchain_format(
			int64_t* out_xr_format,
			format_e* out_format
		) const;
		result_e create_swapchain_gl();

		result_e create_space();

		result_e poll_events();
		result_e on_session_state_changed(const XrEventDataBuffer& event);

		struct frame_t
		{
			XrFrameState state = {};
			XrView views[VIEW_COUNT] = {};
			XrCompositionLayerProjectionView layer_views[VIEW_COUNT] = {};
			framebuffer_t framebuffers[VIEW_COUNT] = {};

			const framebuffer_t& framebuffer_left() const
			{
				return framebuffers[EYE_LEFT];
			}

			const framebuffer_t& framebuffer_right() const
			{
				return framebuffers[EYE_RIGHT];
			}

			vec3 get_position(const uint32_t eye) const
			{
				vec3 position(views[eye].pose.position.x,
					views[eye].pose.position.y,
					views[eye].pose.position.z
				);
				return position;
			}

			quat_t get_orientation(const uint32_t eye) const
			{
				quat_t orientation(
					views[eye].pose.orientation.x,
					views[eye].pose.orientation.y,
					views[eye].pose.orientation.z,
					views[eye].pose.orientation.w
				);
				return orientation;
			}

			transform_t get_transform(const uint32_t eye) const
			{
				transform_t t = {};
				t.orientation = get_orientation(eye);
				t.position = get_position(eye);
				return t;
			}
		};

		result_e begin_frame(frame_t* out_frame);
		result_e end_frame(const frame_t& frame);

		uvec2 get_swapchain_size(const int eye) const
		{
			return swapchains[eye].size;
		}
	};

	extern xr_t *const xr;
}
#endif

#endif