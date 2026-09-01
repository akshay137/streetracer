#pragma once
#ifndef KATHA_TYPE_COMMAND_LINE_H__
#define KATHA_TYPE_COMMAND_LINE_H__ 1

#include "primitive.hpp"
#include "bitfield.hpp"

namespace katha
{
	struct command_line
	{
		static command_line __instance;
		
		enum class command : uint32_t
		{
			display_index,
			debug_graphics,
			asset_root,
			force_opengl_es,
	
			__max
		};
		efield_t<command> commands = {};
		int display_index = 0;
		const char* asset_root = nullptr;

		void parse_impl(int argc, char** args);
		static void parse(int argc, char** args)
		{
			__instance.parse_impl(argc, args);
		}

		void log_impl();
		static void log() { __instance.log_impl(); }

		static bool has(const command command)
		{
			return __instance.commands.has_enum(command);
		}

		static int get_display_index(const int default_index)
		{
			if (has(command::display_index))
			{
				return __instance.display_index;
			}
			return default_index;
		}

		static const char* get_asset_root(const char* default_asset_root = "./")
		{
			if (has(command::asset_root))
			{
				return __instance.asset_root;
			}
			return default_asset_root;
		}
	};
}

#endif