#ifndef _DLL_H
#define _DLL_H

#ifdef _POSIX
#include <dlfcn.h>

#define DLLEXPORT
typedef void* DLLHANDLE;
#define DLLOPEN(a) dlopen(a, RTLD_LAZY)
#define DLLSYM(h,a) dlsym(h, a)
#define DLLCLOSE(a) dlclose(a)
#define DLLERROR dlerror()
#define DLLEXT ".so"
#define DLLEXTLEN 3
#define DIRSEPARATOR '/'
#endif

#ifdef _WIN32
#include <windows.h>

#define DLLEXPORT __declspec( dllexport )

// handle to our DLL
typedef HINSTANCE DLLHANDLE;
// opens our DLL
#define DLLOPEN(a) LoadLibrary(a)
// gets function pointer in the DLL by name
#define DLLSYM(h,a) GetProcAddress(h, a)
// Closes the DLL
#define DLLCLOSE(a) FreeLibrary(a)
// the DLL extension
#define DLLEXT ".dll"
// the length of the extension
#define DLLEXTLEN 4
// the seperator of the directories
#define DIRSEPARATOR '\\'
#endif

#endif //_DLL_H
