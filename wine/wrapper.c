#include <Windows.h>
//PROGMAN.EXE‚È‚Ç‚ÅCOMMDLG‚ª“Ç‚İ‚Ü‚ê‚È‚©‚Á‚½‚Ì‚ÅC³
HMODULE LoadLibraryAWrapper(const char *name)
{
    return LoadLibraryA(name);
}
