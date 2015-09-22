// convspec.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "main.c.h"
static int parse_input_file(DLLSPEC *spec);
int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("wine spec file convert tool\nusage: %s specfile(16bit only) module name\n", argv[0]);
		fatal_error("file argument '%s' not allowed in this mode\n", argv[0]);
	}
	DLLSPEC *spec = alloc_dll_spec();
	spec_file_name = argv[1];
	exec_mode = MODE_DLL;
	spec->type = SPEC_WIN16;
	spec->file_name = spec_file_name;
	output_file = stdout;
	//	init_dll_name(spec);
	if (argc > 2)
	{
		spec->main_module = xstrdup(argv[2]);
		spec->dll_name = xstrdup(argv[2]);
	}
	else
	{
		init_dll_name(spec);
	}
	switch (exec_mode)
	{
	case MODE_DLL:
		if (spec->subsystem != IMAGE_SUBSYSTEM_NATIVE)
			spec->characteristics |= IMAGE_FILE_DLL;
		/* fall through */
	case MODE_EXE:
		load_resources(argv, spec);
		load_import_libs(argv);
		if (spec_file_name && !parse_input_file(spec)) break;
		if (fake_module)
		{
			if (spec->type == SPEC_WIN16) output_fake_module16(spec);
			else output_fake_module(spec);
			break;
		}
		//read_undef_symbols(spec, argv);
		switch (spec->type)
		{
		case SPEC_WIN16:
			output_spec16_file(spec);
			break;
		case SPEC_WIN32:
			BuildSpec32File(spec);
			break;
		default: assert(0);
		}
		break;
	}
	if (nb_errors) exit(1);
	return EXIT_SUCCESS;
}

