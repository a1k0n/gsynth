#include "modload.h"
#include <stlstuff.h>
#include <libgsyn/dll.h>
#include <sys/types.h>
#ifdef _POSIX
#include <dirent.h>
#endif
#ifdef _WIN32
#include <io.h>
#endif
#include <stdio.h>

typedef SynthModule* (*SynthCreateFunc)(void);

// this version is internal to the module loader
// ultimately this will do refcounting but modload_remove_modules isn't
// currently used.
struct privateModuleInfo: public ModuleInfo
{
	const char *filename;
	SynthCreateFunc func;
	DLLHANDLE handle;
};

static list<privateModuleInfo> module_list;
static list<privateModuleInfo>::iterator module_list_itr;

char *get_basename(const char *name)
{
	char bname[64], *dotptr;
#ifdef _POSIX
	strncpy(bname, strrchr(name, DIRSEPARATOR)+4, 64); // strip "lib"
#else
	strncpy(bname, strrchr(name, DIRSEPARATOR)+1, 64);
#endif
	dotptr = strchr(bname, '.');
	*dotptr = 0;
	return strdup(bname);
}

static int loadmod(const char *name)
{
	privateModuleInfo dlinfo;
	void (*modinfo)(ModuleInfo*);

	dlinfo.handle = DLLOPEN(name);
	if(!dlinfo.handle) {
#ifdef DLLERROR
		fprintf(stderr, "Can't load module %s: %s\n", name, DLLERROR);
#else
		fprintf(stderr, "Can't load module %s\n", name);
#endif
		return 0;
	}

	modinfo = (void(*)(ModuleInfo*)) DLLSYM(dlinfo.handle, "module_info");
	dlinfo.func = (SynthCreateFunc) DLLSYM(dlinfo.handle, "module_instantiate");

	if(!modinfo || !dlinfo.func) {
		fprintf(stderr, "%s is not a gsynth module\n", name);
		return 0;
	}

	dlinfo.filename = strdup(name);
	dlinfo.basename = get_basename(name);
	
	modinfo(&dlinfo);
	
	module_list.push_back(dlinfo);
	fprintf(stderr, "Loaded module %s\n", name);
	return 1;
}

int modload_scan_modules(const char *directory)
{
#ifdef _WIN32
	struct _finddata_t dll_file;
	long hFile;
	char name[MAX_PATH];
	int num_found=0;
	sprintf(name, "%s%c*.dll", directory, DIRSEPARATOR);

	/* Find first .dll file in current directory */
    if( (hFile = _findfirst( name, &dll_file )) == -1L )
	{
		fprintf(stderr, "Couldn't find module directory or there were no DLLs in that directory.\n");
	}
	else
	{
		sprintf(name, "%s%c%s", directory, DIRSEPARATOR, dll_file.name);
		num_found+=loadmod(name);
		while( _findnext( hFile, &dll_file ) == 0 )
		{
			sprintf(name, "%s%c%s", directory, DIRSEPARATOR, dll_file.name);
			num_found+=loadmod(name);
		}

		_findclose( hFile );
	}
#else
	DIR *fp;
	int i, num_found=0;
	char name[PATH_MAX];
	struct dirent *dent;

	fp=opendir(directory);
	if(!fp) {
		fprintf(stderr, "Couldn't find module directory.\n");
		return 0;
	}
	for(;(dent=readdir(fp));)
	{
		sprintf(name, "%s%c%s", directory, DIRSEPARATOR, dent->d_name);
		i=strlen(name);
		if(i<3) continue;
		if(strncmp(name+i-DLLEXTLEN, DLLEXT, DLLEXTLEN)) continue;
		num_found+=loadmod(name);
	}
#endif
	return num_found;
}	

ModuleInfo *modload_next_module(int head)
{
	if(head) module_list_itr = module_list.begin();
	else module_list_itr++;

	if(module_list_itr == module_list.end()) return NULL;
	return &(*module_list_itr);
}

SynthModule *modload_instantiate(ModuleInfo *modinfo)
{
	privateModuleInfo *pinfo = (privateModuleInfo*) modinfo;
	return pinfo->func();
}

void modload_delete(ModuleInfo *modinfo, SynthModule *mod)
{
	// privateModuleInfo *pinfo = (privateModuleInfo*) modinfo;
	// try to find modinfo in module_list if paranoid

	delete mod;
}

