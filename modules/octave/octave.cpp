#include "octave.h"
#include <stdlib.h>
#include <libgsyn/dll.h>
#include <stdio.h>

extern "C" DLLEXPORT void module_info(ModuleInfo *ptr);
extern "C" DLLEXPORT SynthModule *module_instantiate(void);

static SynthParameter *sp_list[] = { NULL };

Octave::Octave()
{
}

Octave::~Octave()
{
}

void Octave::GetModuleInfo(ModuleInfo *ptr)
{
	module_info(ptr);
}

SynthParameter** Octave::GetParameterList()
{
	return sp_list;
}

void Octave::ReadRow(long *patdata)
{
}

int Octave::Process(float *outbuf, const float *inbuf, int size)
{
	int i;
	for(i=0;i<size/2;i++) 
	{ 
		float k = inbuf[0] + inbuf[size/2];
		*outbuf++ = k;
		*outbuf++ = k;
		inbuf++;
	}
	return 1;
}

////////////////////////////////
// module stuff
extern "C" DLLEXPORT void module_info(ModuleInfo *ptr) 
{
	ptr->name = "Octave";
	ptr->channelsin=1;
	ptr->channelsout=1;
	ptr->about = "Octave\n"
		"Shitty harmonic distortion thing.\n"
		"Just for testing throughput.";
}

extern "C" DLLEXPORT SynthModule *module_instantiate(void)
{
	return new Octave;
}

