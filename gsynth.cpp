// main() is implemented in gui-whatever/, which interfaces with gui-common/.

#include "gsynth.h"
#include <libgsyn/modload.h>

void gsynth_init(void)
{
	gsynth_core = new SynthCore;
	modload_scan_modules(_MODULEDIR);
}

void gsynth_setup_default_machine(MachineEditor *ed)
{
	SBuffer *buf;
	ModuleInfo *mi;
	buf = gsynth_core->GetMasterBuf();
	Module *master = new Module(buf, 0.5, 0.5);
	ed->AddModule(master);
	ed->SetMaster(master);
	mi = new ModuleInfo;  // whee, this will never be freed.
	gsynth_core->GetMaster()->GetModuleInfo(mi);
	buf->set_moduleinfo(mi);
	gsynth_editor = ed;
}

SynthCore *gsynth_core = NULL;
MachineEditor *gsynth_editor = NULL;

