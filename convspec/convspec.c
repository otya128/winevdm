// -*- tab-width: 4 -*-
// # vim: set expandtab: ts=4 
// Converts Win32 wine spec files to Win16 specs. Based on winebuild.
// otya 2018
#include <stdio.h>
/* Stubs */
static char *optarg;
static int optind;
static int opterr;
static int optopt;
struct option;
struct option
{
    const char *name;
    int has_arg;
    int *flag;
    int val;
};
static int getopt_long(int ___argc, char *const *___argv,
    const char *__shortopts,
    const struct option *__longopts, int *__longind)
{
    fprintf(stderr, "getopt_long: stub\n");
    return 0;
}
static int getopt_long_only(int ___argc, char *const *___argv,
    const char *__shortopts,
    const struct option *__longopts, int *__longind)
{
    fprintf(stderr, "getopt_long_only: stub\n");
    return 0;
}
int mkstemps(char *template, int suffix_len)
{
    fprintf(stderr, "mkstemps: stub\n");
    return 0;
}
#define main __main
#include "main.c.h"
#include "ver.h"
char *nb_lib_paths;
#undef main
static int parse_input_file(DLLSPEC *spec);
int main(int argc, char* argv[])
{
    int add_ver = 0;
	if (argc <= 1)
	{
		printf("wine spec file convert tool\nusage: %s specfile(16bit only) module name [-EXE]\n%s specfile(16bit only) -DEF\n", argv[0], argv[0]);
		fatal_error("file argument '%s' not allowed in this mode\n", argv[0]);
	}
	DLLSPEC *spec = alloc_dll_spec();
	spec_file_name = argv[1];
	exec_mode = MODE_DLL;
	spec->type = SPEC_WIN16;
	spec->file_name = spec_file_name;
    /* remove dir */
    if (strrchr(spec->file_name, '\\'))
    {
        spec->file_name = strrchr(spec->file_name, '\\') + 1;
    }
    if (strrchr(spec->file_name, '/'))
    {
        spec->file_name = strrchr(spec->file_name, '/') + 1;
    }
    /* remove spec extension */
    if (strstr(spec->file_name, ".spec"))
    {
        spec->file_name = xstrdup(spec->file_name);
        strstr(spec->file_name, ".spec")[0] = '\0';
    }
	output_file = stdout;
    target_cpu = CPU_x86;
    init_dll_name(spec);
    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            if (!strcmp(argv[i], "--heap"))
            {
                spec->heap_size = atoi(argv[++i]);
            }
            if (!strcmp(argv[i], "-DEF"))
            {
                exec_mode = MODE_DEF;
            } 
            if (!strcmp(argv[i], "-EXE"))
            {
                exec_mode = MODE_EXE;
            }
            if (!strcmp(argv[i], "-32"))
            {
                spec->type = SPEC_WIN32;
            }
            if (!strcmp(argv[i], "-ver"))
            {
                add_ver = 1;
            }
        }
        if (exec_mode != MODE_DEF)
        {
            spec->main_module = xstrdup(argv[2]);
            spec->dll_name = xstrdup(argv[2]);
        }
    }
	else
	{
		init_dll_name(spec);
	}
	switch (exec_mode)
	{
	case MODE_DEF:
		if (!spec_file_name) fatal_error("missing .spec file\n");
		if (!parse_input_file(spec)) break;
		output_def_file(spec, 1);
		break;
	case MODE_DLL:
		if (spec->subsystem != IMAGE_SUBSYSTEM_NATIVE)
			spec->characteristics |= IMAGE_FILE_DLL;
		/* fall through */
	case MODE_EXE:
		load_resources(argv, spec);
		load_import_libs(argv);
		if (spec_file_name && !parse_input_file(spec)) break;
        if (add_ver)
        {
            char outname[12] = {0};
            strncpy(outname, spec->dll_name, 8);
            strcpy(ver_res + 0x144, outname);
            load_res16_from_buf(ver_res, ver_res_len, spec);
        }
		if (fake_module)
		{
			if (spec->type == SPEC_WIN16) output_fake_module16(spec);
			else output_fake_module(spec);
			break;
		}
		//read_undef_symbols(spec, argv);
        if (spec->type == SPEC_WIN16 && exec_mode == MODE_EXE)
            spec->init_func = xstrdup("__wine_spec_exe16_entry");
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

