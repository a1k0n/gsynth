#ifndef __MODULE_H
#define __MODULE_H

#include <stdlib.h>

struct ModuleInfo;

// all parameters have the data type 'long'.
struct SynthParameter
{
	int type; // type of parameter: 0 = hex value, 1 = note/octave
	int digits; // number of digits to display in tracker
	long min, max; // min/max values
	const char *name; // parameter name
	const char *description; // long description - displayed in status bar
};

// I really would rather have these be pure virtual, but certain compiler
// issues have arisen where __pure_virtual wasn't being linked into the
// modules.
class SynthModule
{
public:
	// Writes module info into ptr.
	virtual void GetModuleInfo(ModuleInfo *ptr) {}
	// Returns a null-terminated array of synth parameters.
	virtual SynthParameter** GetParameterList() { return NULL; }
	// Reads a row from the pattern.  Parameters are pre-parsed into longs.
	// a value of -1 means no value.
	virtual void ReadRow(long *patdata) {}
	// Process returns 1 if data was processed to the output buffer, 0 otherwise.
	virtual int Process(float *outbuf, const float *inbuf, int size) 
		{ return 0; }
	// Set various parameters such as tempo.  These are defined below.
	virtual void SetParameter(int parm, float data) {}
};

#define PARM_TEMPO	0 // tempo of song (bpm)
#define PARM_RATE	1 // output samplerate (Hz)
// To calculate number of samples in a tick (between ReadRow calls)
// use samplerate*60/(tempo*4) = samplerate*15/tempo.  a "beat" is four ticks.

struct ModuleInfo
{
// name is showed in the module dialog, basename is the basic filename
	const char *name, *basename;
	const char *about;
	int channelsin, channelsout;
};

// Each module also has the following functions in it:
// extern "C" DLLEXPORT void module_info(ModuleInfo *ptr);
// extern "C" DLLEXPORT SynthModule *module_instantiate(void);

#endif

