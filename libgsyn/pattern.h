#ifndef __PATTERN_H
#define __PATTERN_H

#include <libgsyn/module.h>

// Can patterns be shared between modules?  Yes, as long as they have the same
// format.  Perhaps I'll just check the format pointers as they should be the
// same since modules are only loaded once.

// Creating a pattern sets all data to -1, corresponding to no value.
// Data for each track is arranged sequentially, not interleaved.
class gsynth_Pattern
{
private:
	int length, alloclen;
	int tracks;
	int bytes_per_row;
	SynthParameter **format;
	long *patdata;
	char *name;
	int patid;

public:
	gsynth_Pattern(SynthParameter **fmt, int len, int tracks, 
		const char *patname, int id=-1);
	~gsynth_Pattern();

	void set_id(int k) { patid = k; }
	int get_id(void) { return patid; }

	const char *GetName(void) { return name; }
	void SetName(const char *n);
	int GetLength(void) { return length; }
	void SetLength(int l) { SetSize(l, tracks); }
	int GetTracks(void) { return tracks; }
	void SetTracks(int t) { SetSize(length, t); }

	void SetSize(int len, int trk);

	SynthParameter **GetFormat(void) { return format; }

// returns data at given track/row or NULL if out of range
	long *GetData(int track, int row);
// returns 0 if row/col is bad; clamps val to min/max values
	int SetData(int track, int row, int col, long val);
};

// each entry points to a pattern <pat> which is triggered on song row <row>
struct gsynth_seq_entry
{
	int row;
	gsynth_Pattern *pat;
	gsynth_seq_entry *prev, *next;
};

class SBuffer;
// sorted list of seq_entries; linear insertion/deletion time, constant time iteration
class gsynth_Sequence
{
private:
	SBuffer *module;
	// entry at beginning and end of list
	gsynth_seq_entry *head, *tail;
	// next entry to play and currently playing pattern
	gsynth_seq_entry *cued_pat, *playing_pat;
	int playing_row, pattern_row;
	int loop_begin, loop_end;

	gsynth_seq_entry *find_entry_at(int row);
public:
	gsynth_Sequence(SBuffer *mod);
	~gsynth_Sequence();

	// SetEntry will replace an entry at <row> if it exists
	void SetEntry(gsynth_Pattern *pat, int row);
	int RemoveEntry(int row);

	void SetLoopBegin(int begin) { loop_begin = begin; }
	void SetLoopEnd(int end) { loop_end = end; }
	void SetPlayPosition(int row);
	void Restart(void);

	gsynth_seq_entry* GetHeadEntry(void) { return head; }

	// tick sends a tick of the song's pattern data to the module
	// return 1 if looped
	int Tick();
};

#endif

