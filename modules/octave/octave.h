// Effect which plays samples an octave lower
// Andy Sloane 3/99

#ifndef __OCTAVE_H
#define __OCTAVE_H

#include <libgsyn/module.h>

class Octave: public SynthModule
{
public:
	Octave();
	virtual ~Octave();
	virtual void GetModuleInfo(ModuleInfo *ptr);
	virtual SynthParameter** GetParameterList();
	virtual void ReadRow(long *patdata);
	virtual int Process(float *outbuf, const float *inbuf, int size);
};

#endif

