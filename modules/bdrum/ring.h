// damped-sine "ring" generator
#include <math.h>

// times and frequencies are in number of samples
struct RingGen
{
	float l,r, Fc, Tc, t2;
	RingGen() { l=0; r=0; Fc=0; Tc=0; }
	float CalcFc(float F) { return 2*M_PI*sin(M_PI*F); }
	void SetT(float T) { Tc = pow(0.01, 1.0/T); }
	void SetFc(float F) { Fc = F; }
	void Ring(float amp) { l+=amp; }
	float Process(void) { t2 = l*Fc + r; r=t2; l = l*Tc - t2*Fc; return t2; }
};

