#if KATHA_XR

#include "xr.hpp"
#include "../../katha/core/config.hpp"

#include <SDL2/SDL_syswm.h>

#if KATHA_PLATFORM == KATHA_LINUX
#define XR_USE_PLATFORM_XLIB 1
#define XR_USE_PLATFORM_WAYLAND 1
#endif

#define XR_USE_GRAPHICS_API_OPENGL 1
#include <GL/glx.h>
#include <openxr/openxr_platform.h>

#if KATHA_PLATFORM == KATHA_LINUX
#include <SDL2/SDL_loadso.h>

bool fill_xlib_graphics_binding(
	XrGraphicsBindingOpenGLXlibKHR* binding,
	const SDL_SysWMinfo& wm
)
{
	void* xlib = SDL_LoadObject("libX11.so");
	if (nullptr == xlib)
	{
		katha::log_line("error: SDL_LoadObject(X11) {s}", SDL_GetError());
		return false;
	}

	typedef int (*PFN_XGetWindowAttributes)(Display*, Window, XWindowAttributes*);
	typedef VisualID (*PFN_XVisualIDFromVisual)(Visual*);
	typedef int (*PFN_XFree)(void*);

	PFN_XGetWindowAttributes _XGetWindowAttributes =
		(PFN_XGetWindowAttributes)SDL_LoadFunction(xlib, "XGetWindowAttributes");
	PFN_XVisualIDFromVisual _XVisualIDFromVisual =
		(PFN_XVisualIDFromVisual)SDL_LoadFunction(xlib, "XVisualIDFromVisual");
	PFN_XFree _XFree = (PFN_XFree)SDL_LoadFunction(xlib, "XFree");
	if (!(_XGetWindowAttributes && _XVisualIDFromVisual && _XFree))
	{
		katha::log_line("error: failed to load xlib function pointers");
		SDL_UnloadObject(xlib);
		return false;
	}

	Display* display = wm.info.x11.display;
	Window xlib_window = wm.info.x11.window;
	XWindowAttributes attribs = {};
	uint32_t visual_id = 0;
	if (_XGetWindowAttributes(display, xlib_window, &attribs))
	{
		visual_id = _XVisualIDFromVisual(attribs.visual);
	}
	else
	{
		katha::log_line("error-xr: failed to get visual id");
		SDL_UnloadObject(xlib);
		return false;
	}

	typedef GLXFBConfig* (*PFN_glXGetFBConfigs)(Display*, int, int*);
	typedef int (*PFN_glXGetFBConfigAttrib)(Display*, GLXFBConfig, int, int*);

	PFN_glXGetFBConfigs glx_get_fbconfigs =
		(PFN_glXGetFBConfigs)SDL_GL_GetProcAddress("glXGetFBConfigs");
	PFN_glXGetFBConfigAttrib glx_get_fbconfig_attrib =
		(PFN_glXGetFBConfigAttrib)SDL_GL_GetProcAddress("glXGetFBConfigAttrib");
	if ((nullptr == glx_get_fbconfigs) || (nullptr == glx_get_fbconfig_attrib))
	{
		katha::log_line("xr: failed to get proc address for GLXFBConfig");
		SDL_UnloadObject(xlib);
		return false;
	}

	const int screen = DefaultScreen(display);
	
	int config_count = 0;
	GLXFBConfig* configs = glx_get_fbconfigs(display, screen, &config_count);
	if ((nullptr == configs) || (config_count <= 0))
	{
		katha::log_line("error-xr: glXGetFBConfigs");
		SDL_UnloadObject(xlib);
		return false;
	}

	katha::log_line("xr: found {i} GLXFBConfigs", config_count);
	GLXFBConfig current_config = nullptr;
	for (int i = 0; i < config_count; i++)
	{
		int id = 0;
		if (glx_get_fbconfig_attrib(display, configs[i], GLX_VISUAL_ID, &id))
		{
			continue;
		}
		if (static_cast<VisualID>(id) == static_cast<VisualID>(visual_id))
		{
			current_config = configs[i];
			break;
		}
	}
	
	_XFree(configs);
	SDL_UnloadObject(xlib);
	
	if (nullptr == current_config)
	{
		katha::log_line("error-xr: no matching FBConfig found");
		return false;
	}

	binding->next = nullptr,
	binding->xDisplay = display;
	binding->visualid = visual_id;
	binding->glxFBConfig = current_config;
	binding->glxDrawable = xlib_window;
	binding->glxContext = (GLXContext)katha::gl->context;

	return true;
}
#endif

const char** katha::xr_t::get_instance_extensions_gl() const
{
	static const char* extensions[] = {
		XR_KHR_OPENGL_ENABLE_EXTENSION_NAME
	};
	return extensions;
}

katha::result_e katha::xr_t::create_session_gl(
	const config_t& config,
	SDL_Window* window
)
{
	graphics_api = graphics_api_e::gl;

	PFN_xrGetOpenGLGraphicsRequirementsKHR xrGetGraphicsRequirements =
		(PFN_xrGetOpenGLGraphicsRequirementsKHR)get_proc_address(
			"xrGetOpenGLGraphicsRequirementsKHR"
		);
	if (nullptr == xrGetGraphicsRequirements)
	{
		return result_e::error_xr;
	}

	XrGraphicsRequirementsOpenGLKHR requirements = {
		.type = XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR
	};
	XrResult result = xrGetGraphicsRequirements(instance, system_id, &requirements);
	if (!check_result(result, "xrGetOpenGLGraphicsRequirementsKHR"))
	{
		return result_e::error_xr;
	}
	log_line("xr: minumum OpenGL version required {u}.{u}.{u}",
		XR_VERSION_MAJOR(requirements.minApiVersionSupported),
		XR_VERSION_MINOR(requirements.minApiVersionSupported),
		XR_VERSION_PATCH(requirements.minApiVersionSupported)
	);

	SDL_SysWMinfo wm = {};
	SDL_VERSION(&(wm.version));
	if (!SDL_GetWindowWMInfo(window, &wm))
	{
		return result_e::error_sdl;
	}

	XrSessionCreateInfo session_info = {
		.type = XR_TYPE_SESSION_CREATE_INFO,
		.next = nullptr,
		.createFlags = 0,
		.systemId = system_id
	};

#if KATHA_PLATFORM == KATHA_LINUX
	XrGraphicsBindingOpenGLXlibKHR binding_xlib = {
		.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR
	};
	XrGraphicsBindingOpenGLWaylandKHR binding_wayland = {
		.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WAYLAND_KHR
	};
	if (SDL_SYSWM_X11 == wm.subsystem)
	{
		if (!fill_xlib_graphics_binding(&binding_xlib, wm))
		{
			return result_e::error_sdl;
		}
		session_info.next = &binding_xlib;
	}
#endif // KATHA_PLATFORM == KATHA_LINUX

	result = xrCreateSession(instance, &session_info, &session);
	if (!check_result(result, "xrCreateSession"))
	{
		return result_e::error_xr;
	}
	log_line("xr: session {p}", session);

	return result_e::success;
}

int64_t katha::xr_t::get_opengl_swapchain_format() const
{
	uint32_t count = 0;
	XrResult xr_result = xrEnumerateSwapchainFormats(
		session, 0, &count, nullptr
	);
	if (!check_result(xr_result, "xrEnumerateSwapchain"))
	{
		return 0;
	}
	if (0 == count)
	{
		log_line("error-xr: no swapchain formats found");
		return 0;
	}

	int64_t* formats = alloc<int64_t>(count);
	if (nullptr == formats)
	{
		return 0;
	}

	xr_result = xrEnumerateSwapchainFormats(session, count, &count, formats);
	if (!check_result(xr_result, "xrEnumerateSwapchainFormats"))
	{
		return 0;
	}

	int64_t selected_format = formats[0];
	for (uint32_t i = 0; i < count; i++)
	{
		if (GL_SRGB8_ALPHA8 == formats[i])
		{
			selected_format = formats[i];
			break;
		}

		if (GL_RGBA16F == formats[i])
		{
			selected_format = formats[i];
		}
	}

	release(formats);
	log_line("xr: opengl swapchain format {i64:x}", selected_format);
	return selected_format;
}

katha::result_e katha::xr_t::create_swapchain_gl()
{
	const int64_t swapchain_format = get_opengl_swapchain_format();
	for (uint32_t i = 0; i < VIEW_COUNT; i++)
	{
		swapchain_t& swapchain = swapchains[i];
		const XrViewConfigurationView& view = views[i];

		swapchain.size = ivec2(
			static_cast<int32_t>(view.recommendedImageRectWidth),
			static_cast<int32_t>(view.recommendedImageRectHeight)
		);

		XrSwapchainCreateInfo info = {
			.type = XR_TYPE_SWAPCHAIN_CREATE_INFO,
			.next = nullptr,
			.createFlags = 0,
			.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT
				| XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT,
			.format = swapchain_format,
			.sampleCount = view.recommendedSwapchainSampleCount,
			.width = view.recommendedImageRectWidth,
			.height = view.recommendedImageRectHeight,
			.faceCount = 1,
			.arraySize = 1,
			.mipCount = 1
		};

		XrResult xr_result = xrCreateSwapchain(session,
			&info, &(swapchain.swapchain)
		);
		if (!check_result(xr_result, "xrCreateSwapchain"))
		{
			return result_e::error_xr;
		}
		log_line("xr: swapchain_{u} {iv2} {p}",
			i, swapchain.size.array(), swapchain.swapchain
		);

		uint32_t image_count = 0;
		xr_result = xrEnumerateSwapchainImages(swapchain.swapchain,
			0, &image_count, nullptr
		);
		if (!check_result(xr_result, "xrEnumerateSwapchainImages::1"))
		{
			return result_e::error_xr;
		}
		if (image_count >= swapchain_t::MAX_IMAGE_COUNT)
		{
			log_line(
				"error-xr: swapchain created with {u} images, "
				"supported max images {u}",
				image_count,
				swapchain_t::MAX_IMAGE_COUNT
			);
			return result_e::error_xr;
		}

		swapchain.image_count = image_count;
		log_line("xr: image_count {u}", swapchain.image_count);

		XrSwapchainImageOpenGLKHR images[swapchain_t::MAX_IMAGE_COUNT] = {};
		for (uint32_t j = 0; j < image_count; j++)
		{
			images[j].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
		}
		xr_result = xrEnumerateSwapchainImages(
			swapchain.swapchain,
			image_count,
			&image_count,
			(XrSwapchainImageBaseHeader*)images
		);
		if (!check_result(xr_result, "xrEnumerateSwapchainImages::2"))
		{
			return result_e::error_xr;
		}

		for (uint32_t j = 0; j < image_count; j++)
		{
			uint32_t framebuffer = gl->create_framebuffer_from_texture(images[j].image);
			if (0 == framebuffer)
			{
				return result_e::error_gl;
			}

			swapchain.images[j] = static_cast<uint64_t>(images[j].image);
			swapchain.image_views[j] = static_cast<uint64_t>(framebuffer);
			log_line("xr: image_{u} {u64} view: {u64}",
				j, swapchain.images[j], swapchain.image_views[j]
			);
		}
	}

	return result_e::success;
}

#endif