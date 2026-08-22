#if KATHA_XR

#include "xr.hpp"
#include "../vulkan/vulkan_context.hpp"
#include "../../katha/core/config.hpp"

#include <SDL2/SDL_syswm.h>

// openxr_platform.h depends on following, but doesn't include it
#define XR_USE_GRAPHICS_API_VULKAN 1
#include <vulkan/vulkan.h>
#include <openxr/openxr_platform.h>

katha::xr_t xr_global_instance = {};
katha::xr_t *const katha::xr = &xr_global_instance;

bool katha::xr_t::check_result(const XrResult result, const char* message) const
{
	if (nullptr == instance)
	{
		return false;
	}
	if (XR_SUCCESS == result)
	{
		return true;
	}

	char buffer[64] = {};
	XrResult res = xrResultToString(instance, result, buffer);
	if (XR_SUCCESS == res)
	{
		log_line("error_xr: {s} -> {s}", message, buffer);
	}
	else
	{
		log_line("error_xr: failed to check result: {s}", message);
	}

	return false;
}

katha::result_e katha::xr_t::init(
	const config_t& config,
	SDL_Window* window
)
{
	graphics_api = config.graphics_api;

	XrApplicationInfo app_info = {
		.applicationVersion = GAME_VERSION.major,
		.engineVersion = ENGINE_VERSION.major,
		.apiVersion = XR_CURRENT_API_VERSION
	};
	string_t::cstring_copy(GAME_NAME, app_info.applicationName, XR_MAX_APPLICATION_NAME_SIZE);
	string_t::cstring_copy(ENGINE_NAME, app_info.engineName, XR_MAX_ENGINE_NAME_SIZE);

	const char** extensions = get_instance_extensions();
	XrInstanceCreateInfo instance_info = {
		.type = XR_TYPE_INSTANCE_CREATE_INFO,
		.next = nullptr,
		.createFlags = 0,
		.applicationInfo = app_info,
		.enabledApiLayerCount = 0,
		.enabledApiLayerNames = nullptr,
		.enabledExtensionCount = 1,
		.enabledExtensionNames = extensions
	};
	XrResult result = xrCreateInstance(&instance_info, &instance);
	if (!check_result(result, "xrCreateInstance"))
	{
		return result_e::error_xr;
	}
	log_line("xr: instance {p}", instance);

	XrSystemGetInfo system_info = {
		.type = XR_TYPE_SYSTEM_GET_INFO,
		.next = nullptr,
		.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY
	};
	result = xrGetSystem(instance, &system_info, &system_id);
	if (!check_result(result, "xrGetSystem"))
	{
		return result_e::error_xr;
	}
	log_line("xr: system_id {u64}", system_id);

	XrSystemProperties system_properties = {
		.type = XR_TYPE_SYSTEM_PROPERTIES
	};
	result = xrGetSystemProperties(instance, system_id, &system_properties);
	if (!check_result(result, "xrGetSystemProperties"))
	{
		return result_e::error_xr;
	}

	log_line("xr: system `{s}`", system_properties.systemName);
	log_line("xr: max swapchain res {u}x{u}",
		system_properties.graphicsProperties.maxSwapchainImageWidth,
		system_properties.graphicsProperties.maxSwapchainImageHeight
	);
	log_line("xr: orientation tracking: {b}",
		(bool)system_properties.trackingProperties.orientationTracking
	);
	log_line("xr: position tracking: {b}",
		(bool)system_properties.trackingProperties.positionTracking
	);

	result_e res = create_view();
	if (!katha::check_result(res, "xr::create_view"))
	{
		return result_e::error_xr;
	}

	res = result_e::success;
	switch (config.graphics_api)
	{
		case graphics_api_e::gl:
			res = create_session_gl(config, window);
			if (!katha::check_result(res, "xr::create_session_gl"))
			{
				return res;
			}
			res = create_swapchain_gl();
			if (!katha::check_result(res, "xr::create_swapchain_gl"))
			{
				return res;
			}
			break;
		
		case graphics_api_e::vulkan:
			res = create_session_vulkan(config);
			if (!katha::check_result(res, "xr::create_session_vulkan"))
			{
				return result_e::error;
			}
			break;
	}

	res = create_space();
	if (katha::check_result(res, "xr::create_space"))
	{
		return res;
	}
	
	return result_e::success;
}

void katha::xr_t::clear()
{
	log_line("xr::clear()");
	XrResult xr_result = XR_SUCCESS;

	if (space)
	{
		xr_result = xrDestroySpace(space);
		check_result(xr_result, "xrDestroySpace");
		space = XR_NULL_HANDLE;
	}

	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		swapchain_t& swapchain = swapchains[i];
		if (nullptr == swapchain.swapchain)
		{
			continue;
		}

		if (graphics_api_e::gl == graphics_api)
		{
			// destroy framebuffers
			for (uint32_t j = 0; j < swapchain.image_count; j++)
			{
				framebuffer_t framebuffer = swapchain.framebuffers[j];
				// this texture is managed by OpenXR
				framebuffer.color_0 = {};
				gl->delete_framebuffer(&framebuffer);
			}
		}
		else if (graphics_api_e::vulkan == graphics_api)
		{
			// destroy image views
		}

		xr_result = xrDestroySwapchain(swapchain.swapchain);
		check_result(xr_result, "xrDestroySwapchain");
		
		swapchain = {};
	}
	
	if (session)
	{
		xr_result = xrDestroySession(session);
		check_result(xr_result, "xrDestroySession");
		session = XR_NULL_HANDLE;
	}

	system_id = 0;
	
	if (instance)
	{
		xr_result = xrDestroyInstance(instance);
		check_result(xr_result, "xrDestroyInstance");
		instance = XR_NULL_HANDLE;
	}
}

PFN_xrVoidFunction katha::xr_t::get_proc_address(const char* proc_name) const
{
	log_line("xr: querying proc address {s}", proc_name);

	PFN_xrVoidFunction proc = nullptr;;
	XrResult result = xrGetInstanceProcAddr(instance, proc_name, &proc);
	if (!check_result(result, "xrGetInstanceProcAddr"))
	{
		return nullptr;
	}

	return proc;
}

const char** katha::xr_t::get_instance_extensions() const
{
	switch (graphics_api)
	{
		case graphics_api_e::gl:
			return get_instance_extensions_gl();
	}

	return nullptr;
}

katha::result_e katha::xr_t::create_view()
{
	uint32_t count = 0;
	XrResult xr_result = xrEnumerateViewConfigurations(
		instance, system_id, 0, &count, nullptr
	);
	if (!check_result(xr_result, "xrEnumerateViewConfigurations::1"))
	{
		return result_e::error_xr;
	}
	log_line("xr: found {u} view configurations", count);

	constexpr uint32_t MAX_VIEW_CONFIGS = 16;
	XrViewConfigurationType view_configs[MAX_VIEW_CONFIGS] = {};
	if (count > MAX_VIEW_CONFIGS)
	{
		count = MAX_VIEW_CONFIGS;
		log_line("xr: only checking first {u} view configurations",
			MAX_VIEW_CONFIGS
		);
	}

	xr_result = xrEnumerateViewConfigurations(
		instance, system_id,
		count, &count, view_configs
	);
	if (!check_result(xr_result, "xrEnumerateViewConfigurations::2"))
	{
		return result_e::error_xr;
	}

	bool found = false;
	for (uint32_t i = 0; i < count; i++)
	{
		if (VIEW_CONFIGURATION_TYPE == view_configs[i])
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		log_line("error-xr: system doesn't support stereo view type");
		return result_e::error_xr;
	}

	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		views[i].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
	}

	count = VIEW_COUNT;
	xr_result = xrEnumerateViewConfigurationViews(
		instance, system_id,
		VIEW_CONFIGURATION_TYPE,
		count, &count, views
	);
	if (!check_result(xr_result, "xrEnumerateViewConfigurationViews (retrieve views)"))
	{
		return result_e::error_xr;
	}

	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		log_line("xr: view_{u} recommended size {u}x{u}",
			i,
			views[i].recommendedImageRectWidth,
			views[i].recommendedImageRectHeight
		);
	}

	return result_e::success;
}

katha::result_e katha::xr_t::create_session_vulkan(const config_t& config)
{
	graphics_api = graphics_api_e::vulkan;

	return result_e::success;
}

katha::result_e katha::xr_t::create_space()
{
	XrReferenceSpaceCreateInfo info = {
		.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
		.next = nullptr,
		.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL,
		.poseInReferenceSpace = {
			.orientation = { .x = 0, .y = 0, .z = 0, .w = 1 },
			.position = { .x = 0, .y = 0, .z = 0 }
		}
	};
	XrResult xr_result = xrCreateReferenceSpace(session, &info, &space);
	if (!check_result(xr_result, "xrCreateSpace"))
	{
		return result_e::error_xr;
	}

	log_line("xr: space {p}", space);
	return result_e::success;
}

katha::result_e katha::xr_t::poll_events()
{
	
	XrResult xr_result = XR_SUCCESS;
	while (XR_SUCCESS == xr_result)
	{
		XrEventDataBuffer event_data = {
			.type = XR_TYPE_EVENT_DATA_BUFFER
		};
		xr_result = xrPollEvent(instance, &event_data);
		if ((XR_EVENT_UNAVAILABLE != xr_result)
			&& !check_result(xr_result, "xrPollEvents")
		)
		{
			return result_e::error_xr;
		}

		switch (event_data.type)
		{
			case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
				on_session_state_changed(event_data);
				break;
		}
	}
	return result_e::success;
}

katha::result_e katha::xr_t::on_session_state_changed(const XrEventDataBuffer& event)
{
	const XrEventDataSessionStateChanged* state =
		(XrEventDataSessionStateChanged*)&event;
	
	if (state->session != session)
	{
		return result_e::error_xr;
	}

	if (XR_SESSION_STATE_READY == state->state)
	{
		session_running = 1;
		XrSessionBeginInfo begin_info = {
			.type = XR_TYPE_SESSION_BEGIN_INFO,
			.next = 0,
			.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO
		};
		XrResult xr_result = xrBeginSession(session, &begin_info);
		if (!check_result(xr_result, "xrBeginSession"))
		{
			return result_e::error_xr;
		}
		log_line("xr: begin session");
		return result_e::success;
	}

	if (XR_SESSION_STATE_STOPPING == state->state)
	{
		session_running = 0;
		XrResult xr_result = xrEndSession(session);
		if (!check_result(xr_result, "xrEndSession"))
		{
			return result_e::error_xr;
		}
		log_line("xr: end session");
		return result_e::success;
	}

	const bool exiting = XR_SESSION_STATE_EXITING == state->state;
	const bool loss_pending = XR_SESSION_STATE_LOSS_PENDING == state->state;
	if (exiting || loss_pending)
	{
		session_running = 0;
		return result_e::success;
	}

	return result_e::success;
}

katha::result_e katha::xr_t::begin_frame(frame_t* out_frame)
{
	frame_t frame = {};
	frame.state = { .type = XR_TYPE_FRAME_STATE };
	XrFrameWaitInfo wait_info = { .type = XR_TYPE_FRAME_WAIT_INFO };
	
	XrResult xr_result = xrWaitFrame(session, &wait_info, &frame.state);
	if (!check_result(xr_result, "xrWaitFrame"))
	{
		return result_e::error_xr;
	}

	XrFrameBeginInfo begin_info = { .type = XR_TYPE_FRAME_BEGIN_INFO };
	xr_result = xrBeginFrame(session, &begin_info);
	if (!check_result(xr_result, "xrBeginFrame"))
	{
		return result_e::error_xr;
	}

	if ((0 == session_running) || !frame.state.shouldRender)
	{
		log_line("xr: skipping frame");
		XrFrameEndInfo end_info = {
			.type = XR_TYPE_FRAME_END_INFO,
			.next = nullptr,
			.displayTime = frame.state.predictedDisplayTime,
			.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
			.layerCount = 0,
			.layers = nullptr
		};
		xr_result = xrEndFrame(session, &end_info);
		if (!check_result(xr_result, "xrEndFrame"))
		{
			return result_e::error_xr;
		}
		return result_e::request_skip;
	}

	// locate views
	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		frame.views[i].type = XR_TYPE_VIEW;
	}
	XrViewState view_state = { .type = XR_TYPE_VIEW_STATE };
	XrViewLocateInfo locate_info = {
		.type = XR_TYPE_VIEW_LOCATE_INFO,
		.next = nullptr,
		.viewConfigurationType = VIEW_CONFIGURATION_TYPE,
		.displayTime = frame.state.predictedDisplayTime,
		.space = space
	};
	uint32_t view_count = 0;
	xr_result = xrLocateViews(session, &locate_info, &view_state,
		VIEW_COUNT, &view_count, frame.views
	);
	if (!check_result(xr_result, "xrLocateViews") || (VIEW_COUNT != view_count))
	{
		return result_e::error_xr;
	}

	// acquire swapchain images
	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		swapchain_t& swapchain = swapchains[i];

		uint32_t image_index = 0;
		XrSwapchainImageAcquireInfo image_acquire_info = {
			.type = XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO
		};
		xr_result = xrAcquireSwapchainImage(
			swapchain.swapchain,
			&image_acquire_info,
			&image_index
		);
		if (!check_result(xr_result, "xrAcquireSwapchainImage"))
		{
			return result_e::error_xr;
		}
		frame.framebuffers[i] = swapchain.framebuffers[image_index];

		XrSwapchainImageWaitInfo image_wait_info = {
			.type = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO,
			.timeout = 1000000000l
		};
		xr_result = xrWaitSwapchainImage(
			swapchain.swapchain, &image_wait_info
		);
		if (!check_result(xr_result, "xrWaitSwapchainImage"))
		{
			return result_e::error_xr;
		}

		XrCompositionLayerProjectionView& lview = frame.layer_views[i];
		lview.type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
		lview.next = nullptr;
		lview.fov = frame.views[i].fov;
		lview.pose = frame.views[i].pose;
		lview.subImage.swapchain = swapchain.swapchain;
		lview.subImage.imageRect.offset = { .x = 0, .y = 0 };
		lview.subImage.imageRect.extent = {
			.width = static_cast<int32_t>(swapchain.size.x),
			.height = static_cast<int32_t>(swapchain.size.y)
		};
		lview.subImage.imageArrayIndex = 0;
	}

	write_checked(out_frame, frame);
	return result_e::success;
}

katha::result_e katha::xr_t::end_frame(const frame_t& frame)
{
	XrResult xr_result = XR_SUCCESS;

	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		swapchain_t& swapchain = swapchains[i];
		XrSwapchainImageReleaseInfo release_info = {
			.type = XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO
		};
		xr_result = xrReleaseSwapchainImage(swapchain.swapchain, &release_info);
		if (!check_result(xr_result, "xrReleaseSwapchainImage"))
		{
			return result_e::error_xr;
		}
	}

	XrCompositionLayerProjection layer_projection = {
		.type = XR_TYPE_COMPOSITION_LAYER_PROJECTION,
		.next = nullptr,
		.layerFlags = 0,
		.space = space,
		.viewCount = VIEW_COUNT,
		.views = frame.layer_views
	};
	XrCompositionLayerBaseHeader* layers[] = {
		(XrCompositionLayerBaseHeader*)&layer_projection
	};

	XrFrameEndInfo end_info = {
		.type = XR_TYPE_FRAME_END_INFO,
		.next = nullptr,
		.displayTime = frame.state.predictedDisplayTime,
		.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
		.layerCount = 1,
		.layers = layers
	};
	xr_result = xrEndFrame(session, &end_info);
	if (!check_result(xr_result, "xrEndFrame"))
	{
		return result_e::error_xr;
	}

	return result_e::success;
}

#endif