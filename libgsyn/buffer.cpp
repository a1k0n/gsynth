#include "buffer.h"
#include <stdio.h>

int global_frameref = -1;
static int next_id = 1;

SBuffer::SBuffer(int id):
	sequence(this)
{
	inbuf = outbuf = NULL;
	name = NULL;
	format = NULL;
	buffer_length = 0;
	channelsin=0;
	channelsout=0;
	set_id(id);
}

void SBuffer::set_id(int id)
{
	if(id == -1) id = next_id++;
	// is this sufficient to avoid duplicate IDs?
	else if(id > next_id) next_id = id+1;
	bufid = id;
}

SBuffer::~SBuffer()
{
	free(inbuf);
	free(outbuf);
}

void SBuffer::alloc_buffers(int len)
{
	int len_in, len_out;

	if(buffer_length>=len) return;

	len_in = len*channelsin;
	len_out = len*channelsout;

	if(len_in) inbuf = (float*) realloc(inbuf, sizeof(float)*len_in);
	if(len_out) outbuf = (float*) realloc(outbuf, sizeof(float)*len_out);
	buffer_length = len;
}

void SBuffer::fill_inbuf(int size)
{
	list<SBuffer*>::iterator i;

	if(!channelsin) return;

	memset(inbuf, 0, sizeof(float)*size*channelsin);
	for(i=dependencies.begin();i!=dependencies.end();i++)
		(*i)->mix_into(inbuf, size, channelsin);
}

void SBuffer::fill_outbuf(int size)
{
	list<SynthModule*>::iterator i;
	memset(outbuf, 0, sizeof(float)*size*channelsout);
	for(i=modules.begin();i!=modules.end();i++)
		(*i)->Process(outbuf, inbuf, size);

	// FIXME (optimization): check return value of Process, set an 
	// "available" flag so if nothing was processed here, then any
	// dependencies satisfied by this module don't need to copy this
	// buffer.
}

void SBuffer::fill(int size)
{
	if(need_fill()) {
		alloc_buffers(size);
		fill_inbuf(size);
		fill_outbuf(size);
	}
	set_filled();
}

// FIXME: gain is not implemented
void SBuffer::mix_into(float *out, int len, int outbufchannels)
{
	int i;
	if(need_fill()) fill(len);
	if(outbufchannels == channelsout) {
		len*=outbufchannels;
		for(i=0;i<len;i++) out[i]+=outbuf[i];
		return;
	}
	// if this module outputs stereo, and we need mono
	if(outbufchannels*2 == channelsout) {
		len*=outbufchannels;
		for(i=0;i<len;i++) out[i]+=outbuf[2*i];
		return;
	}
	// if this module outputs mono, and we need stereo
	if(outbufchannels == channelsout*2) {
		len*=channelsout;
		for(i=0;i<len;i++) {
			out[2*i]+=outbuf[i];
			out[2*i+1]+=outbuf[i];
		}
		return;
	}
	fprintf(stderr, "SBuffer::mix_into: %d->%d channels unimplemented\n",
		channelsout, outbufchannels);
}

int SBuffer::add_module(SynthModule *m)
{
	ModuleInfo mi;
	m->GetModuleInfo(&mi);

	// If we don't have any modules in the list, adopt the channel requirements
	// of this first one.
	if(modules.size()==0) {
		channelsin = mi.channelsin;
		channelsout = mi.channelsout;
		modules.push_back(m);
		format = m->GetParameterList();
		set_name(mi.name);
		return 1;
	}
	// If we do have modules in the list and this one doesn't match, fail.
	if(mi.channelsin != channelsin || mi.channelsout != channelsout)
		return 0;

	// All else is good; add it to the list.
	modules.push_back(m);
	return 1;
}

int SBuffer::add_dependency(SBuffer *d)
{
	// does this module take any input?
	if(!channelsin) return 0;
	// does the dependency give output?
	if(!d->channelsout) return 0;
	// are we already connected to this module?
	if(find(dependencies.begin(), dependencies.end(), d) != dependencies.end())
		return 0;
	// is this going to cause a loop?
	if(d->find_dependency(this))
		return 0;
	dependencies.push_back(d);
	return 1;
}

int SBuffer::remove_dependency(SBuffer *d)
{
	list<SBuffer*>::iterator i;
	i = find(dependencies.begin(), dependencies.end(), d);
	if(i == dependencies.end()) return 0;
	dependencies.erase(i);
	return 1;
}

int SBuffer::find_dependency(SBuffer *d)
{
	list<SBuffer*>::iterator i;
	for(i=dependencies.begin();i!=dependencies.end();i++) {
		if((*i) == d) return 1;
		if((*i)->find_dependency(d)) return 1;
	}
	return 0;
}

gsynth_Pattern *SBuffer::new_pattern(int len, const char *name)
{
	gsynth_Pattern *pat = new gsynth_Pattern(format, len, modules.size(), name);
	patterns.push_back(pat);
	sequence.RemoveEntry(0);
	sequence.SetEntry(pat, 0);
	sequence.SetLoopBegin(0);
	sequence.SetLoopEnd(len);
	return pat;
}

