//==================================================================================
#include <windows.h>
#include "Detours/src/detours.h"
#include "clientHooking.h"
#include "cvar.h"
//==================================================================================
typedef bool (WINAPI *g_QPC)(LARGE_INTEGER*);
g_QPC QueryPerformanceCounterOrg = NULL;
//==================================================================================
char hackdir[256];
//==================================================================================
void HookGLFunctions(void);
void UnHookGLFunctions(void);
//==================================================================================
bool WINAPI QueryPerformanceCounterNew(LARGE_INTEGER* pLI)
{
	if ( !bClientActive )
		ActivateClient( );
	if ( !bEngineActive )
		ActivateEngine( );
	return (QueryPerformanceCounterOrg)(pLI);
}
//==================================================================================
void PatchEngine ( );
void HookFunctions(void)
{
	//patch the engine
	PatchEngine();
	DetourTransactionBegin();
	QueryPerformanceCounterOrg = (g_QPC)DetourFindFunction("Kernel32.dll", "QueryPerformanceCounter");
	DetourAttach(&(PVOID&)QueryPerformanceCounterOrg, (PVOID)QueryPerformanceCounterNew);
	DetourTransactionCommit();
	HookGLFunctions();
}
//==================================================================================
void UnHookFunctions(void)
{
	DetourTransactionBegin();
	DetourDetach(&(PVOID&)QueryPerformanceCounterOrg, (PVOID)QueryPerformanceCounterNew);
	DetourTransactionCommit();
	UnHookGLFunctions();
}
//==================================================================================
bool WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		GetModuleFileName((HINSTANCE)hinstDLL,hackdir,255);
		char* pos = hackdir+strlen(hackdir);
		while(pos>=hackdir && *pos!='\\') --pos;
			pos[1]=0;
		HookFunctions();
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
		UnHookFunctions();
	return true;
}
//==================================================================================