// Bass drum module as shown on page 537 of Hal Chamberlin's Musical
// Applications Of Microprocessors
// Andy Sloane 6/99

#include <libgsyn/module.h>
#include "ring.h"

class BDrum: public SynthModule
{
	float boom;
	float bamt;
	float bdecay;

	RingGen ring;

	float Fs;
	float Fbegin, Fend, Fc;

public:
	BDrum();
	virtual ~BDrum();

	virtual void GetModuleInfo(ModuleInfo *ptr);
	virtual SynthParameter** GetParameterList();
	virtual void ReadRow(long *patdata);
	virtual int Process(float *outbuf, const float *inbuf, int size);
	virtual void SetParameter(int parm, float data);
};

