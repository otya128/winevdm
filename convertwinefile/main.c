/*
 * Converts wine DLL source Makefiles to MSVC projects.
 * 
 * Copyright (C) 2017 otya
 */
#include <stdio.h>
#include <shlwapi.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
const char * getdirname(const char *path)
{
	const char *a = strpbrk(path, "\\/");
	if (!a) return path;
	while (1)
	{
		a++;
		path = a;
		if (!*a) return path;
		a = strpbrk(path, "\\/");
		if (!a) return path;
	}
}
inline static size_t memcount(const char* begin, char c, size_t n) {
    size_t r = 0;
    for (size_t i = 0; i < n; i++)
        if (begin[i] == c)
            r++;
    return r;
}
/** Escape for cmd and xcopy MSVCRT command-line
 * JS: `"${arg.replace(/(\\*)($|")/g, '$1$1$2').replace(/"/g, '""')}"`;
 */
char * escape(const char* op)
{
    // estimate size
    size_t l = strlen(op);
    size_t bufsz = 2 + l + memcount(op, '\\', l) + memcount(op, '"', l) + 1;
    char* ret = malloc(bufsz);
    if (!ret) return ret;

    size_t spos = 0, tpos = 0;
    ret[tpos++]='"';

    // find quotes or end
    const char* quote = NULL;
    while ((quote = memchr(&op[spos], '"', l)) || 
           (quote = op + l)) {
        // Count consec backslash
        const char* k;
        for (k = quote - 1; *k == '\\'; k--) {
            /* PASS */
        }

        // Copy up to and including k
        size_t stertch = (k - op) - spos + 1;
        memcpy(&ret[tpos], &op[spos], stertch);
        spos += stertch;
        tpos += stertch;

        // Double backslashes
        for (const char* s = k + 1; s < quote; s++) {
            ret[tpos++]='\\';
            ret[tpos++]='\\';
            spos++;
        }
        assert(&op[spos] == quote);

        // Double the quote
        if (spos != l) {
            ret[tpos++] = '"';
            ret[tpos++] = '"';
            spos++;
        } else {
          break;
        }
    }
    ret[tpos++] = '"';
    ret[tpos] = '\0';
    return ret;
}

#define ESCAPE(Var) const char* e ## Var = escape(Var)
#define E(Var)      (e ## Var)
#define FESC(Var)   free(e ## Var)

void copy(const char * src, const char *dst)
{
	char buf[512];
    ESCAPE(src); ESCAPE(dst);
	int c = snprintf(buf, sizeof(buf), "xcopy %s %s /I", E(src), E(dst));
    FESC(src); FESC(dst);
    assert(c < sizeof(buf) - 1);
	system(buf);
}
typedef struct
{
	char module[100];
	char imports[32][32];
	char csrcs[32][32];
	char rcsrcs[32][32];
} makein;
char *fgetsex(char *buffer, size_t bufsiz, FILE *fp)
{
	if (!fgets(buffer, bufsiz, fp))
		return NULL;
	size_t len = strlen(buffer);
	if (buffer[len - 2] == '\\')
	{
		fgetsex(buffer + len - 3, bufsiz - len - 1, fp);
		return buffer;
	}
	return buffer;
}
const char *skipspaces(const char *in)
{
	while (*in)
	{
		if (!isspace(*in))
			return in;
		in++;
	}
	return in;
}
const char *getitem(const char *in, char *buf, size_t bufsiz)
{
	in = skipspaces(in);
	if (*in == '\0')
		return 0;
	size_t buflen = 0;
	while (1)
	{
		if (isspace(*in))
			break;
		buflen++;
		if (buflen < bufsiz)
		{
			buf[buflen - 1] = *in;
		}
		in++;
	}
	if (buflen < bufsiz)
	{
		buf[buflen] = '\0';
	}
	else
	{
		buf[bufsiz - 1] = '\0';
	}
	in = skipspaces(in);
	return in;
}
void parsemakein(FILE *fp, makein *in)
{
	char line[2048];
	while (fgetsex(line, sizeof(line), fp))
	{
		const char *linebuf = line;
		char buf[128];
		linebuf = getitem(linebuf, buf, sizeof(buf));
		if (!linebuf)
			continue;
		if (*linebuf != '=')
		{
			continue;
		}
		linebuf++;
		linebuf = skipspaces(linebuf);
		if (!strcmp(buf, "MODULE"))
		{
			linebuf = getitem(linebuf, buf, sizeof(buf));
			strncpy(in->module, buf, sizeof(in->module));
		}
		if (!strcmp(buf, "IMPORTS"))
		{
			for (int i = 0; i < sizeof(in->imports); i++)
			{
				linebuf = getitem(linebuf, buf, sizeof(buf));
				if (!linebuf) break;
				strncpy(in->imports[i], buf, sizeof(in->imports[0]));
			}
		}
		if (!strcmp(buf, "C_SRCS"))
		{
			for (int i = 0; i < sizeof(in->csrcs); i++)
			{
				linebuf = getitem(linebuf, buf, sizeof(buf));
				if (!linebuf) break;
				strncpy(in->csrcs[i], buf, sizeof(in->csrcs[0]));
			}
		}
		if (!strcmp(buf, "RC_SRCS"))
		{
			for (int i = 0; i < sizeof(in->rcsrcs); i++)
			{
				linebuf = getitem(linebuf, buf, sizeof(buf));
				if (!linebuf) break;
				strncpy(in->rcsrcs[i], buf, sizeof(in->rcsrcs[0]));
			}
		}
	}
}
typedef enum
{
	TEMPLATE_NONE,
	TEMPLATE_TargetExt,
	TEMPLATE_ClCompile_PreprocessorDefinitions,
	TEMPLATE_Link_AdditionalDependencies,
	TEMPLATE_C_SRCS, TEMPLATE_DEF_FILE, TEMPLATE_OBJ_FILE, TEMPLATE_RootNamespace,
    TEMPLATE_CUSTOM_SPEC, TEMPLATE_CUSTOM_COMMAND, TEMPLATE_DEF_FILE2

} vcxpojlineattr;
typedef struct
{
	vcxpojlineattr attr;
	const char *line;
}vcxpojline;
vcxpojline vcxtemplate[] = {
    { TEMPLATE_NONE,"<?xml version=\"1.0\" encoding=\"utf-8\"?>" },
    { TEMPLATE_NONE,"<Project DefaultTargets=\"Build\" ToolsVersion=\"14.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" },
    { TEMPLATE_NONE,"  <ItemGroup Label=\"ProjectConfigurations\">" },
    { TEMPLATE_NONE,"    <ProjectConfiguration Include=\"Debug|Win32\">" },
    { TEMPLATE_NONE,"      <Configuration>Debug</Configuration>" },
    { TEMPLATE_NONE,"      <Platform>Win32</Platform>" },
    { TEMPLATE_NONE,"    </ProjectConfiguration>" },
    { TEMPLATE_NONE,"    <ProjectConfiguration Include=\"Release|Win32\">" },
    { TEMPLATE_NONE,"      <Configuration>Release</Configuration>" },
    { TEMPLATE_NONE,"      <Platform>Win32</Platform>" },
    { TEMPLATE_NONE,"    </ProjectConfiguration>" },
    { TEMPLATE_NONE,"  </ItemGroup>" },
    { TEMPLATE_NONE,"  <PropertyGroup Label=\"Globals\">" },
    //{ TEMPLATE_NONE,"    <ProjectGuid>{B83EEE1C-F8DE-4F82-8928-67F1B142E5F2}</ProjectGuid>" },
    { TEMPLATE_NONE,"    <Keyword>Win32Proj</Keyword>" },
    { TEMPLATE_NONE,"    <WindowsTargetPlatformVersion>10.0.17134.0</WindowsTargetPlatformVersion>"},
    { TEMPLATE_RootNamespace,"    <RootNamespace>keyboard</RootNamespace>" },
    { TEMPLATE_NONE,"  </PropertyGroup>" },
    { TEMPLATE_NONE,"  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.Default.props\" />" },
    { TEMPLATE_NONE,"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">" },
    { TEMPLATE_NONE,"    <ConfigurationType>DynamicLibrary</ConfigurationType>" },
    { TEMPLATE_NONE,"    <UseDebugLibraries>true</UseDebugLibraries>" },
    { TEMPLATE_NONE,"    <PlatformToolset>v141</PlatformToolset>" },
    { TEMPLATE_NONE,"    <CharacterSet>Unicode</CharacterSet>" },
    { TEMPLATE_NONE,"  </PropertyGroup>" },
    { TEMPLATE_NONE,"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">" },
    { TEMPLATE_NONE,"    <ConfigurationType>DynamicLibrary</ConfigurationType>" },
    { TEMPLATE_NONE,"    <UseDebugLibraries>false</UseDebugLibraries>" },
    { TEMPLATE_NONE,"    <PlatformToolset>v141</PlatformToolset>" },
    { TEMPLATE_NONE,"    <WholeProgramOptimization>true</WholeProgramOptimization>" },
    { TEMPLATE_NONE,"    <CharacterSet>Unicode</CharacterSet>" },
    { TEMPLATE_NONE,"  </PropertyGroup>" },
    { TEMPLATE_NONE,"  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.props\" />" },
    { TEMPLATE_NONE,"  <ImportGroup Label=\"ExtensionSettings\">" },
    { TEMPLATE_NONE,"  </ImportGroup>" },
    { TEMPLATE_NONE,"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" },
    { TEMPLATE_NONE,"    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />" },
    { TEMPLATE_NONE,"  </ImportGroup>" },
    { TEMPLATE_NONE,"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" },
    { TEMPLATE_NONE,"    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />" },
    { TEMPLATE_NONE,"  </ImportGroup>" },
    { TEMPLATE_NONE,"  <Import Project=\"..\\PropertySheet.props\" />"},
    { TEMPLATE_NONE,"  <PropertyGroup Label=\"UserMacros\" />" },
    { TEMPLATE_NONE,"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" },
    { TEMPLATE_NONE,"    <LinkIncremental>true</LinkIncremental>" },
    { TEMPLATE_TargetExt,"" },
    { TEMPLATE_NONE,"  </PropertyGroup>" },
    { TEMPLATE_NONE,"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" },
    { TEMPLATE_NONE,"    <LinkIncremental>false</LinkIncremental>" },
    { TEMPLATE_NONE,"    <IntDir>Release\</IntDir>" },
    { TEMPLATE_TargetExt,"" },
    { TEMPLATE_NONE,"  </PropertyGroup>" },
    { TEMPLATE_NONE,"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" },
    { TEMPLATE_NONE,"    <ClCompile>" },
    { TEMPLATE_NONE,"      <PrecompiledHeader>" },
    { TEMPLATE_NONE,"      </PrecompiledHeader>" },
    { TEMPLATE_NONE,"      <WarningLevel>Level3</WarningLevel>" },
    { TEMPLATE_NONE,"      <Optimization>Disabled</Optimization>" },
    { TEMPLATE_ClCompile_PreprocessorDefinitions,"      <PreprocessorDefinitions>WIN32;_DEBUG;_WINDOWS;_USRDLL;_X86_;__WINESRC__;__i386__;USE_COMPILER_EXCEPTIONS;HAVE_STRNCASECMP;HAVE__STRNICMP;_WINTERNL_;NtCurrentTeb=NtCurrentTeb__;inline=__inline;%(PreprocessorDefinitions)</PreprocessorDefinitions>" },
    { TEMPLATE_NONE,"      <AdditionalIncludeDirectories>../wine</AdditionalIncludeDirectories>" },
    { TEMPLATE_NONE,"    </ClCompile>" },
    { TEMPLATE_NONE,"    <Link>" },
    { TEMPLATE_NONE,"      <SubSystem>Windows</SubSystem>" },
    { TEMPLATE_NONE,"      <GenerateDebugInformation>true</GenerateDebugInformation>" },
    { TEMPLATE_NONE,"      <AdditionalDependencies>$(OutDir)winecrt0.lib;$(OutDir)libwine.lib;$(OutDir)wow32.lib;$(OutDir)krnl386.lib;" },
    { TEMPLATE_Link_AdditionalDependencies,"" },
    { TEMPLATE_NONE,"%(AdditionalDependencies)</AdditionalDependencies>" },
    { TEMPLATE_NONE,"      <GenerateDebugInformation>true</GenerateDebugInformation>" },
    { TEMPLATE_NONE,"      <ModuleDefinitionFile>" },
    { TEMPLATE_DEF_FILE,"" },
    { TEMPLATE_NONE,"      </ModuleDefinitionFile>" },
    { TEMPLATE_NONE,"    </Link>" },
    { TEMPLATE_NONE,"  </ItemDefinitionGroup>" },
    { TEMPLATE_NONE,"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" },
    { TEMPLATE_NONE,"    <ClCompile>" },
    { TEMPLATE_NONE,"      <WarningLevel>Level3</WarningLevel>" },
    { TEMPLATE_NONE,"      <PrecompiledHeader>" },
    { TEMPLATE_NONE,"      </PrecompiledHeader>" },
    { TEMPLATE_NONE,"      <Optimization>MaxSpeed</Optimization>" },
    { TEMPLATE_NONE,"      <FunctionLevelLinking>true</FunctionLevelLinking>" },
    { TEMPLATE_NONE,"      <IntrinsicFunctions>true</IntrinsicFunctions>" },
    { TEMPLATE_ClCompile_PreprocessorDefinitions,"      <PreprocessorDefinitions>WIN32;NDEBUG;_WINDOWS;_USRDLL;_X86_;__WINESRC__;__i386__;USE_COMPILER_EXCEPTIONS;HAVE_STRNCASECMP;HAVE__STRNICMP;_WINTERNL_;NtCurrentTeb=NtCurrentTeb__;inline=__inline;DECLSPEC_HIDDEN=;%(PreprocessorDefinitions)</PreprocessorDefinitions>" },
    { TEMPLATE_NONE,"      <AdditionalIncludeDirectories>../wine</AdditionalIncludeDirectories>" },
    { TEMPLATE_NONE,"    </ClCompile>" },
    { TEMPLATE_NONE,"    <Link>" },
    { TEMPLATE_NONE,"      <SubSystem>Windows</SubSystem>" },
    { TEMPLATE_NONE,"      <GenerateDebugInformation>true</GenerateDebugInformation>" },
    { TEMPLATE_NONE,"      <EnableCOMDATFolding>true</EnableCOMDATFolding>" },
    { TEMPLATE_NONE,"      <OptimizeReferences>true</OptimizeReferences>" },
    { TEMPLATE_NONE,"      <ImageHasSafeExceptionHandlers>false</ImageHasSafeExceptionHandlers>" },
    { TEMPLATE_NONE,"      <ModuleDefinitionFile>" },
    { TEMPLATE_DEF_FILE,"" },
    { TEMPLATE_NONE,"      </ModuleDefinitionFile>" },
    { TEMPLATE_NONE,"      <AdditionalDependencies>$(OutDir)winecrt0.lib;$(OutDir)libwine.lib;$(OutDir)wow32.lib;$(OutDir)krnl386.lib;" },
    { TEMPLATE_Link_AdditionalDependencies,"" },
    { TEMPLATE_NONE,"%(AdditionalDependencies)</AdditionalDependencies>" },
    { TEMPLATE_NONE,"    </Link>" },
    { TEMPLATE_NONE,"  </ItemDefinitionGroup>" },
    { TEMPLATE_NONE,"  <ItemGroup>" },
    { TEMPLATE_C_SRCS,"    <ClCompile Include=\"keyboard.c\" />" },
    { TEMPLATE_NONE,"  </ItemGroup>" },
    { TEMPLATE_NONE,"  <ItemGroup>" },
    { TEMPLATE_OBJ_FILE,"    <Object Include=\"keyboard.drv16.obj\" />" },
    { TEMPLATE_NONE,"  </ItemGroup>" },
    { TEMPLATE_NONE, "  <ItemGroup>" },
    { TEMPLATE_CUSTOM_SPEC, "    <CustomBuild Include=\"%s.spec\">" },
    { TEMPLATE_NONE, "      <FileType>Document</FileType>" },
    { TEMPLATE_CUSTOM_COMMAND, "      <Command Condition=\"'$(Platform)'=='Win32'\">\"$(OutDir)convspec\" \"%%(Filename).spec\" %s &gt; \"%%(Filename).asm\" &amp;&amp; \"$(AsmPath)as\" --32 -o \"%%(Filename).obj\" \"%%(Filename).asm\"</Command>" },
    { TEMPLATE_NONE, "      <Outputs Condition=\"'$(Platform)'=='Win32'\">%(Filename).obj</Outputs>" },
    { TEMPLATE_NONE, "    </CustomBuild>" },
    { TEMPLATE_NONE, "  </ItemGroup>" },
    { TEMPLATE_NONE, "  <ItemGroup>" },
    { TEMPLATE_DEF_FILE2, "    <None Include=\"%s.def\" />\n" },
    { TEMPLATE_NONE, "  </ItemGroup>" },
    { TEMPLATE_NONE,"  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.targets\" />" },
    { TEMPLATE_NONE,"  <ImportGroup Label=\"ExtensionTargets\">" },
    { TEMPLATE_NONE,"  </ImportGroup>" },
    { TEMPLATE_NONE,"</Project>" }
};
#define GetArrayLength(A) (sizeof(A) / sizeof(A[0]))
void generateVCproject(makein *in, FILE *out)
{
	const char * targetext = PathFindExtensionA(in->module);
	const char target[128] = { 0 };
	strncpy(target, in->module, strlen(in->module) - strlen(targetext));
    for (int i = 0; i < GetArrayLength(vcxtemplate); i++)
    {
        if (vcxtemplate[i].attr == TEMPLATE_RootNamespace)
        {
            fprintf(out, "    <RootNamespace>%s</RootNamespace>\n", target);

        }
        else if (vcxtemplate[i].attr == TEMPLATE_TargetExt)
        {
            fprintf(out, "<TargetExt>%s</TargetExt>\n", targetext);
        }
        else if (vcxtemplate[i].attr == TEMPLATE_Link_AdditionalDependencies)
        {
            for (int j = 0; j < GetArrayLength(in->imports); j++)
            {
                if (!strlen(in->imports[j]))
                    break;
                fprintf(out, "%s.lib;", in->imports[j]);
            }
        }
        else if (vcxtemplate[i].attr == TEMPLATE_C_SRCS)
        {
            for (int j = 0; j < GetArrayLength(in->csrcs); j++)
            {
                if (!strlen(in->csrcs[j]))
                    break;
                fprintf(out, "    <ClCompile Include=\"%s\" />\n", in->csrcs[j]);
            }
        }
        else if (vcxtemplate[i].attr == TEMPLATE_DEF_FILE)
        {
            fprintf(out, "%s.def\n", target);
        }
        else if (vcxtemplate[i].attr == TEMPLATE_OBJ_FILE)
        {
            fprintf(out, "    <Object Include=\"%s.obj\" />\n", in->module);
        }
        else if (vcxtemplate[i].attr == TEMPLATE_CUSTOM_SPEC)
        {
            fprintf(out, vcxtemplate[i].line, in->module);
            fprintf(out, "\n");
        }
        else if (vcxtemplate[i].attr == TEMPLATE_CUSTOM_COMMAND)
        {
            char mod[2445];
            for (int j = 0; target[j]; j++)
            {
                mod[j] = toupper(target[j]);
                if (!target[j + 1])
                    mod[j + 1] = 0;
            }
            fprintf(out, vcxtemplate[i].line, mod);
            fprintf(out, "\n");
        }
        else if (vcxtemplate[i].attr == TEMPLATE_DEF_FILE2)
        {
            fprintf(out, vcxtemplate[i].line, target);
        }
		else
		{
			fprintf(out, "%s\n", vcxtemplate[i].line);
		}

	}
}
void generateDEF(makein *makedata, const char *spec, const char *path)
{
	char buf[512];
    ESCAPE(spec);
	int c = snprintf(buf, sizeof(buf), "convspec %s -DEF > \"%s\"", E(spec), path);
    FESC(spec);
    assert(c < sizeof(buf) - 1);
	system(buf);
}
void generateASM(makein *makedata, const char *spec, const char *path, const char *mod)
{
	char buf[512];
    ESCAPE(spec); ESCAPE(mod);
	int c = snprintf(buf, sizeof(buf), "convspec %s %s > \"%s\"", E(spec), E(mod), path);
    FESC(spec); FESC(mod);
    assert(c < sizeof(buf) - 1);
	system(buf);
}
int main(int argc, char **argv)
{
	if (argc != 3)
	{
        puts("Converts wine DLL source Makefiles to MSVC projects.");
		puts("usage: convertwinefile <dll directory> <2>");
		return 0;
	}
	char *dir = argv[1];
	char *dir2 = argv[2];
	copy(dir, dir2);
	makein makedata = { 0 };
	char makefile_in_path[MAX_PATH] = { 0 };
	strncpy(makefile_in_path, dir2, sizeof(makefile_in_path));
	strncat(makefile_in_path, "/Makefile.in", sizeof(makefile_in_path));
	FILE *makefile_in = fopen(makefile_in_path, "r");
	parsemakein(makefile_in, &makedata);
	fclose(makefile_in);
	char proj_in_path[MAX_PATH] = { 0 };
	strncpy(proj_in_path, dir2, sizeof(proj_in_path));
	strncat(makefile_in_path, "/", sizeof(makefile_in_path));
	strncat(makefile_in_path, "/Makefile.in", sizeof(makefile_in_path));
	char projname[MAX_PATH] = { 0 };
	strncpy(projname, PathFindFileNameA(dir2), sizeof(projname));
	PathRemoveExtensionA(projname);
	char projpath[MAX_PATH] = { 0 };
	strncpy(projpath, dir2, sizeof(projpath));
	PathAddBackslashA(projpath);
	strncat(projpath, projname, MAX_PATH);//PathAppendA(projpath, projpath, projname);
	PathAddExtensionA(projpath, ".vcxproj");
	FILE *proj = fopen(projpath, "w+");
	generateVCproject(&makedata, proj);
	fclose(proj);
	char specpath[MAX_PATH] = { 0 };
	strncpy(specpath, dir2, sizeof(specpath));
	PathAddBackslashA(specpath);
	strncat(specpath, makedata.module, MAX_PATH);//	PathAppendA(asmpath, asmpath, makedata.module);
	strncat(specpath, ".spec", MAX_PATH);
	char defpath[MAX_PATH] = { 0 };
	strncpy(defpath, dir2, sizeof(defpath));
	PathAddBackslashA(defpath);
	strncat(defpath, projname, MAX_PATH);//	PathAppendA(defpath, defpath, projname);
	PathAddExtensionA(defpath, ".def");
	generateDEF(&makedata, specpath, defpath);
	return 0;
}
