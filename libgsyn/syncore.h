#ifndef __SYNTHCORE_H
#define __SYNTHCORE_H

#include "buffer.h"
#include "master.h"
#include "gkthread.h"

class SynthCore
{
private:
	list<SynthModule*> modlist;
	list<SBuffer*> bufferlist;
	MasterOutput *master;
	SBuffer *masterbuf;
	int tempo, samplespertick;
	gk_mutex_t core_mutex;
	
	char *songname;
public:
	SynthCore();
	virtual ~SynthCore();

	SBuffer *InstantiateModule(ModuleInfo *mi);
// External modules added to SynthCore are freed by it upon remove.
// I guess the preferred method is to use InstantiateModule.
	SBuffer *AddModule(SynthModule *mod);
	void RemoveModule(SBuffer *buf);

	int AddTrack(SBuffer *buf); // ?
	int RemoveTrack(SBuffer *buf); // ?

	void SetTempo(float bpm);

	void SetSongName(const char *name);
	const char *GetSongName(void) { return songname; }

	SBuffer *GetMasterBuf(void) { return masterbuf; }
	SynthModule *GetMaster(void) { return master; }

	void ProcessAudio();

	void Lock(void) { gk_mutex_lock(&core_mutex); }
	void Unlock(void) { gk_mutex_unlock(&core_mutex); }
};

#endif

