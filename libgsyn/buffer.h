#ifndef __BUFFER_H
#define __BUFFER_H

#include <libgsyn/module.h>
#include <libgsyn/pattern.h>
#include <stlstuff.h>

// global frameref.  Incrementing this invalidates all buffers.
extern int global_frameref;

// if frameref<global_frameref, this buffer needs to be filled
// if any in (dependencies) has frameref<global_frameref, those
// buffers need filling first.
class SBuffer
{
private:
// input buffer (all inputs mixed together) and output buffer (processed input)
	float *inbuf, *outbuf;
// length of each buffer in mono or stereo samples
	int buffer_length;
// number of channels in buffers
	int channelsin, channelsout;
	char *name;
	SynthParameter **format;
	ModuleInfo *moduleinfo;
	int bufid;

// gain [0..1] and pan [-1..1]
	float gain, pan;

	int frameref;

	void alloc_buffers(int len);

	// fill_inbuf fills the output buffers of all of the dependencies, then
	// adds those buffers together.
	void fill_inbuf(int size);
	// adds the output of modules to the output buffer.
	void fill_outbuf(int size);

	int need_fill(void) { return global_frameref != frameref; }
	void set_filled(void) { frameref = global_frameref; }
public:
	list<SynthModule*> modules;  // output buffer is filled by these modules
	list<SBuffer*> dependencies; // input buffer is filled by these
	list<gsynth_Pattern*> patterns;
	gsynth_Sequence sequence;

	SBuffer(int id=-1);
	virtual ~SBuffer();

	void set_id(int k);
	int get_id(void) { return bufid; }

	// mix this buffer's output into outbuf with correct number of channels
	void mix_into(float *out, int len, int outbufchannels);

	// fills its output buffer.
	void fill(int size);

	// Add a module to the processing list
	// returns 0 if channel requirements mismatch
	int add_module(SynthModule *m);
	int remove_module(SynthModule *m);
	// Add a module to the input dependency list
	int add_dependency(SBuffer *d);
	int remove_dependency(SBuffer *d);
	// Walk dependency tree to find this module; return 1 if found
	int find_dependency(SBuffer *d);

	int add_track(void);
	int remove_track(void);

	gsynth_Pattern *new_pattern(int len, const char *name);

	const char *get_name(void) { return name; }
	void set_name(const char *newname) { if(name) free(name); name = strdup(newname); }
	int get_channelsin(void) { return channelsin; }
	int get_channelsout(void) { return channelsout; }
	void set_moduleinfo(ModuleInfo *mi) { moduleinfo = mi; }
	ModuleInfo *get_moduleinfo(void) { return moduleinfo; }
	SynthParameter** get_format(void) { return format; }
};

#endif

