#pragma once
#ifndef KATHA_TYPE_COMMAND_LINE_H__
#define KATHA_TYPE_COMMAND_LINE_H__ 1

#include "primitive.hpp"
#include "bitfield.hpp"

namespace katha
{
	struct CommandLine
	{
		static CommandLine __instance;
		
		enum class Command : uint32_t
		{
			DISPLAY_INDEX,
			DEBUG_GRAPHICS,
			ASSET_ROOT,
	
			__MAX
		};
		
		EField<Command> commands = {};
		int display_index = 0;
		const char* asset_root = nullptr;

		void parse(int argc, char** args);
		static inline void Parse(int argc, char** args)
		{
			__instance.parse(argc, args);
		}

		void log();
		static inline void Log()
		{
			__instance.log();
		}

		static inline bool Has(const Command command)
		{
			return __instance.commands.hasEnum(command);
		}

		static inline int GetDisplayIndex(const int default_index)
		{
			if (Has(Command::DISPLAY_INDEX)) {
				return __instance.display_index;
			}
			return default_index;
		}

		static inline const char* GetAssetRoot(const char* default_asset_root = "./")
		{
			if (Has(Command::ASSET_ROOT)) {
				return __instance.asset_root;
			}
			return default_asset_root;
		}
	};
}

#endif