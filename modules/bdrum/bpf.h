/*
Simple band-pass filter

i: input sample
t = i*C - D2*B + D1*A
o = t-D1
D2 = D1
D1 = t
o: output sample

Q = Fc / (Fh - Fl)
where Fh and Fl are upper and lower 3dB points
A = 2cos(2*M_PI*Fc) * exp(-M_PI*Fc/Q)
B = exp(-2*M_PI*Fc/Q)
C = cos(M_PI*Fc) * sqrt(1.0 - A + B)

Gain at Fc is approximately Q for Q>3
*/



#ifndef __BPF_H
#define __BPF_H

class BPF
{
private:
	float D1, D2;
	float A, B, C;

public:
	BPF() { D1 = D2 = 0.0; }
	void Set(float Fc, float Q);
	float Process(float i) {
		float t,o;
		t = i*C - D2*B + D1*A;
		o = t-D1;
		D2 = D1;
		D1 = t;
		return o;
	}
};

#endif

