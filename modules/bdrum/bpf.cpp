#include "bpf.h"
#include <math.h>

void BPF::Set(float Fc, float Q)
{
//	float Q = Fc / BW;
	Fc *= M_PI;
	A = 2*cos(2*Fc) * exp(-Fc/Q);
	B = exp(-2*Fc/Q);
	C = cos(Fc) * sqrt(1.0 - A + B);
}

