#ifndef __MODLOAD_H
#define __MODLOAD_H

#include <libgsyn/module.h>

// Call scan_modules once for each directory to scan
// return number of modules found
extern int modload_scan_modules(const char *directory);

// Iterate through loaded modules.  Call with head=1 to reset iterator,
// and repeat with head=0 until NULL is returned.
extern ModuleInfo* modload_next_module(int head);

// ModuleInfo* passed in here _must_ have come from modload_next_module.
extern SynthModule* modload_instantiate(ModuleInfo *modinfo);

// This may have to be changed
extern void modload_delete(ModuleInfo *modinfo, SynthModule *mod);

#endif

