#include "bdrum.h"
#include <stdlib.h>
#include <libgsyn/dll.h>
#include <math.h>

extern "C" DLLEXPORT void module_info(ModuleInfo *ptr);
extern "C" DLLEXPORT SynthModule *module_instantiate(void);

static SynthParameter
	p1 = {0, 1, 0, 1, "X", "Trigger drum"},
	p2 = {0, 4, 0, 0xffff, "Amt", "Amount"},
	p3 = {0, 4, 0, 0xffff, "Dec", "Decay"};
	
static SynthParameter *sp_list[] = { &p1, &p2, &p3, NULL};

BDrum::BDrum() 
{
	long defaultdata[] = {0, 0x1000, 0x3000};
	Fs = 44100.0;
	ring.SetT(0.3*Fs);
	Fbegin=ring.CalcFc(100.0/Fs);
	Fend=ring.CalcFc(10.0/Fs);
	Fc = 0;
	ReadRow(defaultdata);
}

BDrum::~BDrum()
{
}

void BDrum::GetModuleInfo(ModuleInfo *ptr)
{
	module_info(ptr);
}

SynthParameter** BDrum::GetParameterList()
{
	return sp_list;
}

void BDrum::ReadRow(long *patdata)
{
	if(patdata[1] != -1) // Bamt
		bamt = patdata[1]*(4.0/65535.0);
	if(patdata[2] != -1) // Bdec
		bdecay = pow(0.1, 1.0/(100.0 + patdata[2]/4.0));
	if(patdata[0] != -1) // trigger drum
	{
		ring.Ring(bamt);
		Fc = Fbegin-Fend;
	}
}

int BDrum::Process(float *outbuf, const float*, int size)
{
	int i;
	for(i=0;i<size;i++) {
		ring.SetFc(Fc+Fend);
		Fc*=bdecay;
		outbuf[i] = ring.Process();
	}
	if(Fc < (1/65536.0)) { Fc = 0; }
	return 1;
}

void BDrum::SetParameter(int parm, float data)
{
	switch(parm) {
		case PARM_RATE:
			Fs = data;
			break;
	}
}

////////////////////////////////
// module stuff
extern "C" DLLEXPORT void module_info(ModuleInfo *ptr) 
{
	ptr->name = "BDrum";
	ptr->channelsin=0;
	ptr->channelsout=1;
	ptr->about = "Bass Drum\n"
		"By Andy Sloane";
}

extern "C" DLLEXPORT SynthModule *module_instantiate(void)
{
	return new BDrum;
}

