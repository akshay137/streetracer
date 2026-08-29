#pragma once
#ifndef KATHA_ENUM_FEATURES_H__
#define KATHA_ENUM_FEATURES_H__ 1

#include "../type/primitive.hpp"

namespace katha
{
	enum feature_e : uint64_t
	{
		vr = 1 << 0,
		vsync = 1 << 1,
		window_mode = 1 << 2,
		
		debug_graphics = 1 << 10,

		force_opengl_es = 1 << 20,
		log_frame_time = 1 << 21
	};
}

#endif