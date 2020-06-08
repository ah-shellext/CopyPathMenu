#include "stdafx.h"
#include "resource.h"
#include "CopyShell_i.h"
#include "dllmain.h"

CCopyShellModule _AtlModule;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}
