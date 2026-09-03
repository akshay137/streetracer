#include "command_line.hpp"
#include "string.hpp"
#include "../utility.hpp"

#include <cstdlib>

katha::CommandLine katha::CommandLine::__instance = {};

void katha::CommandLine::parse(int argc, char** args)
{
	LogLine("parsing {i} command line arguments", argc);

	for (int i = 1; i < argc; i++)
	{
		const String arg = args[i];
		const bool has_next = i < (argc - 1);

		if (arg.equals("--display_index") && has_next)
		{
			commands.setEnum(Command::DISPLAY_INDEX);
			display_index = atoi(args[i + 1]);
			i++;
		}
		else if (arg.equals("--debug_graphics"))
		{
			commands.setEnum(Command::DEBUG_GRAPHICS);
		}
		else if (arg.equals("--asset_root") && has_next)
		{
			commands.setEnum(Command::ASSET_ROOT);
			asset_root = args[i + 1];
			i++;
		}
		else
		{
			LogLine("couldn't parse: {s}", args[i]);
		}
	}
}

void katha::CommandLine::log()
{
	LogLine("display_index: {b} {i}", Has(Command::DISPLAY_INDEX), display_index);
	LogLine("debug_graphics: {b}", Has(Command::DEBUG_GRAPHICS));
	LogLine("asset_root: {b} {s}", Has(Command::ASSET_ROOT), asset_root);
}