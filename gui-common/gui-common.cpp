#include "machine.h"
#include "tracker.h"
#include <gsynth.h>
#include <gsynfile.h>

static MachineEditor *gEditor = NULL;
static TrackEditor *gTracker = NULL;

extern "C" void init_common_gui(void)
{
	gsynth_init();
	gEditor = new MachineEditor;
	gTracker = new TrackEditor;
	gsynth_setup_default_machine(gEditor);
}

// Machine editor callbacks

#define CHECK1 if(!gEditor) return

extern "C" void cb_machine_repaint() 
{ CHECK1; gEditor->Redraw(); }

extern "C" void cb_machine_mouse_button(int btn, int down, int x, int y) 
{ CHECK1; gEditor->OnMouseButton(btn, down, x, y); }

extern "C" void cb_machine_mouse_move(int x, int y) 
{ CHECK1; gEditor->OnMouseMove(x, y); }

extern "C" void cb_machine_add(void *modinfo)
{ CHECK1; gEditor->CreateModule((ModuleInfo*)modinfo); }

extern "C" int cb_machine_popup_dialog(void)
{ CHECK1(0); return gEditor->OnMenuPopup(); }

extern "C" void cb_machine_remove(void)
{ CHECK1; gEditor->OnRemoveModule(); }

extern "C" void cb_machine_disconnect(void)
{ CHECK1; gEditor->OnDisconnect(); }

extern "C" void cb_machine_rename(const char *newname)
{ CHECK1; gEditor->OnRenameModule(newname); }

extern "C" const char *cb_machine_get_modulename(void)
{ CHECK1(NULL); return gEditor->GetModuleName(); }

extern "C" const char *cb_machine_about_module(void)
{ CHECK1(NULL); return gEditor->OnAboutModule(); }

// Tracker callbacks

#define CHECK2 if(!gTracker) return

extern "C" void cb_tracker_repaint()
{ CHECK2; gTracker->Redraw(); }

extern "C" int cb_tracker_key(int keycode, int state)
{ CHECK2(0); return gTracker->OnKey(keycode, state); }

extern "C" void cb_tracker_module_select(void *mod)
{ CHECK2; gTracker->OnModuleSelect((SBuffer*)mod); }

extern "C" void cb_tracker_pattern_select(void *pat)
{ CHECK2; gTracker->OnPatternSelect((gsynth_Pattern*)pat); }

extern "C" void cb_tracker_create_pattern(const char *name, int len)
{ CHECK2; gTracker->OnCreatePattern(name, len); }

// Menubar callbacks

extern "C" void cb_new_module(void)
{ gsynth_new(); }

extern "C" void cb_load_module(const char *fname)
{ gsynth_open(fname); }

extern "C" void cb_save_module(const char *fname)
{ gsynth_save(fname); }

