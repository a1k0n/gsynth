// TB303 module based on original gsyn VCO, VCF, and VCA elements
// Andy Sloane 3/99

#ifndef __TB303_H
#define __TB303_H

#include <libgsyn/module.h>

class TB303: public SynthModule
{
	float vco_inc, vco_k;

	float vcf_cutoff, vcf_envmod, vcf_envdecay, vcf_reso, vcf_rescoeff;
	float vcf_e0, vcf_e1;
	float vcf_c0; // c0=e1 on retrigger; c0*=ed every sample; cutoff=e0+c0
	float vcf_d1, vcf_d2;
	float vcf_a, vcf_b, vcf_c;
	int vcf_envpos;

	float vca_attack, vca_decay, vca_a0, vca_a;
	int vca_mode; // attack/decay mode
public:
	TB303();
	virtual ~TB303();
	virtual void GetModuleInfo(ModuleInfo *ptr);
	virtual SynthParameter** GetParameterList();
	virtual void ReadRow(long *patdata);
	virtual int Process(float *outbuf, const float *inbuf, int size);
	virtual void SetParameter(int parm, float data);

private:
	void vcf_recalc_env();
};

#endif

