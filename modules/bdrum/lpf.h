/*
Simple low-pass filter

i: input sample
o = i*C - D2*B + D1*A
D2 = D1
D1 = o
o: output sample

Q = Fc / (Fh - Fl)
where Fh and Fl are upper and lower 3dB points
A = 2cos(2*M_PI*Fc) * exp(-M_PI*Fc/Q)
B = exp(-2*M_PI*Fc/Q)
C = 1-A+B

Gain at Fc is approximately Q for Q>3
*/



#ifndef __LPF_H
#define __LPF_H

class LPF
{
private:
	float D1, D2;
	float A, B, C;

public:
	LPF() { D1 = D2 = 0.0; }
	void Set(float Fc, float Q);
	float Process(float i) {
		float o;
		o = i*C - D2*B + D1*A;
		D2 = D1;
		D1 = o;
		return o;
	}
};

#endif

