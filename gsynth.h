#ifndef __GSYNTH_H
#define __GSYNTH_H

#include <libgsyn/syncore.h>
#include <gui-common/machine.h>

extern void gsynth_init();
extern void gsynth_setup_default_machine(MachineEditor*);

extern SynthCore *gsynth_core;
extern MachineEditor *gsynth_editor;

#endif

