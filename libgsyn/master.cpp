#include "master.h"
#include "syncore.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef _POSIX
#include <unistd.h>
#endif
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif
#ifdef linux
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#endif

//#define usefile

static volatile int die=0;

#ifdef usefile
static int outfd;
#endif

static void* dspthread(void *dat)
{
	MasterOutput *inst = (MasterOutput*) dat;
	while(!die) inst->core->ProcessAudio();
	return NULL;
}

void MasterOutput::GetModuleInfo(ModuleInfo *ptr)
{
	ptr->name = "Master";
	ptr->basename = "<Master>";
	ptr->channelsin = 2;
	ptr->channelsout = 0;
	ptr->about = "Generic Sound Module\n"
		"Supports Open Sound System (Linux/BSD),\n"
		"SGI Audio Library,\n"
		"Solaris /dev/audio,\n"
		"and Windows WavOut";
}

static SynthParameter sp_mastervol = { 0, 4, 0, 0x7fff, "Vol", 
	"Master Volume"};

static SynthParameter* sp_list[] = {&sp_mastervol, NULL};

SynthParameter** MasterOutput::GetParameterList() 
{ 
	return sp_list; 
}

void MasterOutput::ReadRow(long *patdata)
{
	if(*patdata == -1) return;
	m_vol = (float) *patdata;
}

MasterOutput::MasterOutput(SynthCore *c)
{
	core = c;
	m_vol = 0x6000;
}

int MasterOutput::Process(float *, const float *inbuf, int len)
{
	int i;
	core->Unlock();
	len*=2;
	if(len>bufsize) {
		printf("len>bufsize!*(\n");
		len=bufsize;
	}
	
	if(!inbuf) memset(outbuf, 0, len*2);
	else for(i=0;i<len;i++) {
		float k = m_vol*inbuf[i];
		if(k>32767.0) k=32767;
		if(k<-32767.0) k=-32767;
		outbuf[i] = (short)k;
	}
	
#ifdef sun
	// wait for most of the sound to drain
	audio_info_t info;
	for(;;) {
	    ioctl(fd, AUDIO_GETINFO, &info);
	    if(written - info.play.samples < rate / 10) // 0.1s fudge factor
		break;
	    usleep(10000);
	}
	written += len;
#endif
#if defined(linux) || defined(sun)
	write(fd, (char*)outbuf, len*sizeof(short));
#endif
#ifdef usefile
	write(outfd, (char*)outbuf, len*sizeof(short));
#endif
#ifdef sgi
	ALwritesamps(port, outbuf, len);
#endif

#ifdef _WIN32
	int tmpbuflen;
	char *audiobuf;

	// FINE!&@*! I'll take out this loop.
//	while(!(lpWaveHdr[curwavebuf]->dwFlags & WHDR_DONE) && (lpWaveHdr[curwavebuf]->dwFlags & WHDR_INQUEUE)) ; // wait until this is done.

	while(!(lpWaveHdr[curwavebuf]->dwFlags & WHDR_DONE) && (lpWaveHdr[curwavebuf]->dwFlags & WHDR_INQUEUE))
	{ // wait until this is done.
		WaitForSingleObject(soundevent, INFINITE);
		ResetEvent(soundevent);
//		Sleep(len*250/44100);
	}

    waveOutUnprepareHeader(hwo, lpWaveHdr[curwavebuf], sizeof(WAVEHDR));
	audiobuf = lpWaveHdr[curwavebuf]->lpData;
	tmpbuflen = lpWaveHdr[curwavebuf]->dwBufferLength;
	memset(lpWaveHdr[curwavebuf],0,sizeof(WAVEHDR));
	lpWaveHdr[curwavebuf]->lpData = audiobuf;
	lpWaveHdr[curwavebuf]->dwBufferLength = tmpbuflen;

	memcpy(lpWaveHdr[curwavebuf]->lpData, outbuf, len*sizeof(short));
	lpWaveHdr[curwavebuf]->dwBufferLength = len*sizeof(short);

    waveOutPrepareHeader(hwo, lpWaveHdr[curwavebuf], sizeof(WAVEHDR));
	waveOutWrite(hwo, lpWaveHdr[curwavebuf], sizeof(WAVEHDR));

	curwavebuf++;
	if(curwavebuf >= NUMWAVBUFFERS) curwavebuf = 0;
#endif

	core->Lock();
	return 1;
}


void MasterOutput::StartOutput(int hz, int buf)
{
	rate=hz;
	stereo=1;
#ifdef linux
	int format=AFMT_S16_LE, frag=0x00020000;
	char *dsplist[]={"/dev/dsp", "/dev/dsp1"};
	int i;
	fd=-1;
    for(i=0;i<2;i++) {
        fd=::open(dsplist[i], O_WRONLY);
        if(fd>=0) break;
    }
    if(fd<0) { perror("/dev/dsp"); outbuf=NULL; return; }
    ioctl(fd, SNDCTL_DSP_SETFMT, &format);
    ioctl(fd, SNDCTL_DSP_STEREO, &stereo);
    ioctl(fd, SNDCTL_DSP_SPEED, &rate);
	for(i=buf;i;i>>=1) frag++;
    ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &frag);
#endif

#ifdef usefile
	outfd = open("output.raw", O_CREAT|O_TRUNC|O_WRONLY, 0644);
#endif

#ifdef sgi
	long params[2] = {AL_OUTPUT_RATE, rate};
	ALsetparams(AL_DEFAULT_DEVICE, params, 2);
	cfg=ALnewconfig();
	ALsetchannels(cfg, stereo ? AL_STEREO : AL_MONO);
	ALsetwidth(cfg, AL_SAMPLE_16);
	ALsetqueuesize(cfg, buf);
	port=ALopenport("synth", "w", cfg);
#endif

#ifdef sun
	char *devname = getenv("AUDIODEV");
	if(!devname) devname = "/dev/audio";
	fd = ::open(devname, O_WRONLY);
	if(fd < 0) {
	    perror("opening audio device");
	    outbuf = 0;
	    return;
	}
	audio_info_t info;
	AUDIO_INITINFO(&info);
	// Assume dbri or crystal device for now (not amd)
	info.play.sample_rate = rate;
	info.play.channels = stereo ? 2 : 1;
	info.play.encoding = AUDIO_ENCODING_LINEAR;
	info.play.precision = 16; // can only handle 16 bit in linear mode
	ioctl(fd, AUDIO_SETINFO, &info);
	written = 0;
#endif

	bufallocsize=0; bufsize=0;
	outbuf=NULL;
	
	bufsize=bufallocsize=buf;
	outbuf=(short*)malloc(bufsize*sizeof(short));	// Create our thread
#ifdef _POSIX
	pthread_create(&dspth, NULL, dspthread, (void*)this);
#endif

#ifdef _WIN32
	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
    pcmwf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.nChannels = stereo?2:1;
	pcmwf.nSamplesPerSec = rate;
	pcmwf.wBitsPerSample = 16;
	pcmwf.nAvgBytesPerSec = pcmwf.nChannels * pcmwf.nSamplesPerSec * pcmwf.wBitsPerSample / 8;
	pcmwf.nBlockAlign = pcmwf.nChannels * pcmwf.wBitsPerSample / 8;
	pcmwf.cbSize = 0;

	soundevent = CreateEvent(NULL, TRUE, FALSE, NULL);

	MMRESULT res = waveOutOpen(&hwo, WAVE_MAPPER, &pcmwf, (unsigned long)soundevent, 0, CALLBACK_EVENT);
	while(res != MMSYSERR_NOERROR)
	{
		int retval = MessageBox(NULL, "Try again? (Cancel will exit the app)", "Error: waveOutOpen failure", MB_RETRYCANCEL  );
		if(retval == IDCANCEL)
			exit(-1);
		// try again
		res = waveOutOpen(&hwo, WAVE_MAPPER, &pcmwf, NULL, 0, CALLBACK_NULL);
	}

	mixbuf=(short*)malloc(bufsize*sizeof(short)*8);
	for(int i=0;i<NUMWAVBUFFERS;i++)
	{
		lpWaveHdr[i] = new WAVEHDR;
		memset(lpWaveHdr[i],0,sizeof(WAVEHDR));
		lpWaveHdr[i]->lpData = (char*)mixbuf+buf*pcmwf.nBlockAlign*i;
		lpWaveHdr[i]->dwBufferLength = buf*pcmwf.nBlockAlign;
		lpWaveHdr[i]->dwFlags = 0L;
		lpWaveHdr[i]->dwLoops = 0L;
		lpWaveHdr[i]->dwUser = 0;
		waveOutPrepareHeader(hwo, lpWaveHdr[i], sizeof(WAVEHDR));
//		waveOutWrite(hwo, lpWaveHdr[i], sizeof(WAVEHDR));
	}

	curwavebuf = 0; // this is the next buffer to play.

	DWORD threadId;
	dspth = CreateThread(NULL, NULL, (unsigned long (__stdcall *)(void *))dspthread, (void*)this, 0, &threadId);
#endif
}

void MasterOutput::StopOutput()
{
	die=1;
#ifdef _POSIX
	void *duh;

	pthread_join(dspth, &duh);
#endif

#ifdef _WIN32
	DWORD duh;

	WaitForSingleObject(dspth, INFINITE);
	GetExitCodeThread(dspth, &duh);
#endif

#ifdef sgi
	ALcloseport(port);
	ALfreeconfig(cfg);
#elif defined(_WIN32)
	waveOutReset(hwo);
	waveOutClose(hwo);
	free(outbuf);
	for(int i=0;i<NUMWAVBUFFERS;i++)
	{
		delete lpWaveHdr[i];
	}
	CloseHandle(soundevent);
#else
	close(fd);
#endif
#ifdef usefile
	close(outfd);
#endif
}

