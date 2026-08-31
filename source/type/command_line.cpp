#include "command_line.hpp"
#include "string.hpp"
#include "../utility.hpp"

#include <cstdlib>

katha::command_line katha::command_line::__instance = {};

void katha::command_line::parse_impl(int argc, char** args)
{
	log_line("parsing {i} command line arguments", argc);

	for (int i = 1; i < argc; i++)
	{
		const string_t arg = args[i];
		const bool has_next = i < (argc - 1);

		if (arg.equals("-display_index") && has_next)
		{
			commands.set_enum(command::display_index);
			display_index = atoi(args[i + 1]);
			i++;
		}
		else if (arg.equals("--debug_graphics"))
		{
			commands.set_enum(command::debug_graphics);
		}
		else if (arg.equals("--force_opengl_es"))
		{
			commands.set_enum(command::force_opengl_es);
		}
		else
		{
			log_line("couldn't parse: {s}", args[i]);
		}
	}
}

void katha::command_line::log_impl()
{
	log_line("display_index: {b} {i}", has(command::display_index), display_index);
	log_line("debug_graphics: {b}", has(command::debug_graphics));
	log_line("force_opengl_es: {b}", has(command::force_opengl_es));
}