#include "tb303.h"
#include <stdlib.h>
#include <libgsyn/dll.h>
#include <stdio.h>
#include <math.h>

extern "C" DLLEXPORT void module_info(ModuleInfo *ptr);
extern "C" DLLEXPORT SynthModule *module_instantiate(void);

static SynthParameter 
	p1 = {1, 3, 0, 12*5, "Not", "Note to trigger"},
	p2 = {0, 1, 0, 1, "A", "0 = no accent, 1 = accent"},
	p3 = {0, 1, 0, 1, "S", "1 = slide from previous note to this one"},
	p4 = {0, 4, 0, 0xffff, "Cut", "Filter envelope cutoff"},
	p5 = {0, 4, 0, 0xffff, "Res", "Filter resonance"},
	p6 = {0, 4, 0, 0xffff, "Env", "Filter envelope modulation"},
	p7 = {0, 4, 0, 0xffff, "Dec", "Filter envelope decay"},
	p8 = {0, 4, 0, 0xffff, "Acc", "Accent amount"};

static SynthParameter *sp_list[] = { &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, NULL};

// optimization: only recalc filter coefficients every ENVINC samples
#define ENVINC 64

TB303::TB303()
{
	long defaultdata[] = {-1,0,0,0,0,0,0,0};
	vco_inc = 0.0;
	vco_k = 0;
	
	vcf_cutoff = 0; vcf_envmod = 0;
	vcf_reso = 0; vcf_envdecay = 0;
	vcf_envpos = ENVINC;
	vcf_a = vcf_b = vcf_d1 = vcf_d2 = 0;
	vcf_c0 = 0; vcf_e0 = 0; vcf_e1 = 0;

	vca_mode = 2;  vca_a = 0;
	vca_attack = 1.0 - 0.94406088;
	vca_decay = 0.99897516;
	vca_a0 = 0.5;
	ReadRow(defaultdata);
}

TB303::~TB303()
{
}

void TB303::GetModuleInfo(ModuleInfo *ptr)
{
	module_info(ptr);
}

SynthParameter** TB303::GetParameterList()
{
	return sp_list;
}

void TB303::ReadRow(long *patdata)
{
	int do_recalc = 0;
	// patdata[1] - accent
	// patdata[2] - slide
	if(patdata[3] != -1) { // cutoff
		vcf_cutoff = (float)patdata[3]/65535.0;
		do_recalc = 1;
	}
	if(patdata[4] != -1) { // resonance
		vcf_reso = (float)patdata[4]/65535.0;
		vcf_rescoeff = exp(-1.20 + 3.455*vcf_reso);
		do_recalc = 1;
	}
	if(patdata[5] != -1) { // envmod
		vcf_envmod = (float)patdata[5]/65535.0;
		do_recalc = 1;
	}
	if(patdata[6] != -1) { // decay
		float d = (float)patdata[6]/65535.0;
		d = 0.2 + (2.3*d);
		d*=44100;
		vcf_envdecay = pow(0.1, 1.0/d * ENVINC);
	}

	// patdata[7] // accent amount

	if(do_recalc) {
		vcf_e1 = exp(6.109 + 1.5876*vcf_envmod + 2.1553*vcf_cutoff - 1.2*(1.0-vcf_reso));
		vcf_e0 = exp(5.613 - 0.8*vcf_envmod + 2.1553*vcf_cutoff - 0.7696*(1.0-vcf_reso));
		vcf_e0*=M_PI/44100.0;
		vcf_e1*=M_PI/44100.0;
		vcf_e1 -= vcf_e0;
		vcf_envpos = ENVINC;
	}

	// A-4 is concert A (440Hz)
	if(patdata[0] != -1) { // note
		vco_inc = (440.0/44100.0)*pow(2, (patdata[0]-57)*(1.0/12.0));
		vca_mode = 0;
		vcf_c0 = vcf_e1;
		vcf_envpos = ENVINC;
	}
	else vca_mode = 1;
}

int TB303::Process(float *outbuf, const float*, int size)
{
	int i;
	float w,k;
	for(i=0;i<size;i++) {
		// update vcf
		if(vcf_envpos >= ENVINC) {
			w = vcf_e0 + vcf_c0;
			k = exp(-w/vcf_rescoeff);
			vcf_c0 *= vcf_envdecay;
			vcf_a = 2.0*cos(2.0*w) * k;
			vcf_b = -k*k;
			vcf_c = 1.0 - vcf_a - vcf_b;
			vcf_envpos = 0;
		}
		
		// compute sample
		outbuf[i]=vcf_a*vcf_d1 + vcf_b*vcf_d2 + vcf_c*vco_k*vca_a;
		vcf_d2=vcf_d1; 
		vcf_envpos++;
		vcf_d1=outbuf[i];

		// update vco
		vco_k += vco_inc;
		if(vco_k > 0.5) vco_k -= 1.0;

		// update vca
		if(i==size/2) vca_mode = 2;
		if(!vca_mode) vca_a+=(vca_a0-vca_a)*vca_attack;
		else if(vca_mode == 1) {
			vca_a *= vca_decay;
			// the following line actually speeds up processing on SGIs
			if(vca_a < (1/65536.0)) { vca_a = 0; vca_mode = 2; }
		}
	}
	return 1;
}

void TB303::SetParameter(int parm, float data)
{
}

////////////////////////////////
// module stuff
extern "C" DLLEXPORT void module_info(ModuleInfo *ptr) 
{
	ptr->name = "TB303";
	ptr->channelsin=0;
	ptr->channelsout=1;
	ptr->about = "TB303 emulator\n"
		"Derived from original gsyn code.\n"
		"By Andy Sloane";
}

extern "C" DLLEXPORT SynthModule *module_instantiate(void)
{
	return new TB303;
}

