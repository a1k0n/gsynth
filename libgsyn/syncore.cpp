#include "syncore.h"
#include "modload.h"
#include <string.h>

SynthCore::SynthCore()
{
	gk_mutex_init(&core_mutex);
	master = new MasterOutput(this);
	masterbuf = AddModule(master);
	masterbuf->set_id(0); // buffer id 0 = master

	songname = strdup("Untitled Song");

	SetTempo(140);
	master->StartOutput(44100, 16384);
}

SynthCore::~SynthCore()
{
	master->StopOutput();
	free(songname);
	delete master;
	gk_mutex_destroy(&core_mutex);
}

SBuffer *SynthCore::InstantiateModule(ModuleInfo *mi)
{
	SBuffer *buf = AddModule(modload_instantiate(mi));
	buf->set_moduleinfo(mi);
	return buf;
}

SBuffer *SynthCore::AddModule(SynthModule *mod)
{
	SBuffer *buf = new SBuffer;
	Lock();
	modlist.push_back(mod);
	bufferlist.push_back(buf);
	buf->add_module(mod);
	Unlock();
	return buf;
}

void SynthCore::RemoveModule(SBuffer *buf)
{
	list<SynthModule*>::iterator i,j;
	list<SBuffer*>::iterator s,t;

	Lock();
	// Remove any dependencies on this buffer first
	t = bufferlist.end();
	for(s=bufferlist.begin();s!=bufferlist.end();s++) {
		if((*s) == buf) t = s;
		else (*s)->remove_dependency(buf);
	}
	if(t != bufferlist.end()) bufferlist.erase(t);

	// next, remove the modules in the SBuffer
	for(i=buf->modules.begin();i!=buf->modules.end();i++) {
		j = find(modlist.begin(), modlist.end(), *i);
		if(j != modlist.end()) modlist.erase(j);
		delete (*i);
	}
	// then remove the buffer
	delete buf;
	Unlock();
}

void SynthCore::SetTempo(float bpm)
{
	Lock();
	tempo = (int) bpm;
	samplespertick = (int) ((44100.0 * 60.0)/(4.0*bpm));
	Unlock();
}

void SynthCore::ProcessAudio()
{
	Lock();
	int looped=0;
	global_frameref++;
	list<SBuffer*>::iterator i;
	for(i=bufferlist.begin();i != bufferlist.end(); i++)
		if((*i)->sequence.Tick()) looped = 1;
	if(looped) {
		for(i=bufferlist.begin();i != bufferlist.end(); i++)
			(*i)->sequence.Restart();
	}
		
	masterbuf->fill(samplespertick);
	Unlock();
}

void SynthCore::SetSongName(const char *name)
{
	free(songname);
	songname = strdup(name);
}

