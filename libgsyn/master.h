#ifndef __MASTER_H
#define __MASTER_H

#include <libgsyn/module.h>

#ifdef _POSIX
#include <pthread.h>
#elif defined(WIN32)
#include <windows.h>

#define NUMWAVBUFFERS 3
#endif

#ifdef sgi
#include <audio.h>
#endif

#ifdef sun
#include <sys/audioio.h>
#endif

class SynthCore;
class MasterOutput: public SynthModule
{
public:
	float m_vol;
	SynthCore *core;
#ifdef sgi
	ALconfig cfg;
	ALport port;
#else
	int fd;
#endif
#ifdef sun
        int written;		/* # of samples written to the device */
#endif
	int rate;
	short *outbuf;
	int bufsize, bufallocsize;
	int stereo;
#ifdef _POSIX
	pthread_t dspth;
#elif defined(_WIN32)
	HANDLE dspth;

	HANDLE musicevent;

	HWAVEOUT hwo;
	int curwavebuf;
	WAVEHDR *lpWaveHdr[NUMWAVBUFFERS];
	WAVEFORMATEX pcmwf;

	HANDLE soundevent;

	short *mixbuf;

#endif
public:
	MasterOutput(SynthCore *c);

	void StartOutput(int hz, int buf);
	void StopOutput();

	virtual void GetModuleInfo(ModuleInfo *ptr);
	virtual SynthParameter** GetParameterList();
	virtual void ReadRow(long *patdata);
	virtual int Process(float *outbuf, const float *inbuf, int size);
};

#endif

