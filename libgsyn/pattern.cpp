#include "pattern.h"
#include <stdlib.h>
#include <string.h>
#include <libgsyn/buffer.h>
#include <stdio.h>

static int next_id = 1;

gsynth_Pattern::gsynth_Pattern(SynthParameter **fmt, int len, int trk, 
	const char *patname, int id)
{
	int i;
	format = fmt;
	for(i=0;fmt[i];i++);
	bytes_per_row = i*sizeof(long);

	name = strdup(patname);
	alloclen=0; patdata=NULL;
	length=0; tracks=0;
	SetSize(len, trk);

	if(id == -1) id = next_id++;
	else if(id > next_id) next_id = id+1;
	patid = id;
}

gsynth_Pattern::~gsynth_Pattern()
{
	free(patdata);
	free(name);
}

void gsynth_Pattern::SetName(const char *n)
{
	free(name);
	name = strdup(n);
}

void gsynth_Pattern::SetSize(int l, int trk)
{
	int act_len = l*bytes_per_row*trk;
	if(act_len != alloclen) {
		patdata = (long*) realloc(patdata, act_len);
		if(act_len > alloclen) 
			memset(patdata+alloclen, -1, act_len - alloclen);
		alloclen = act_len;
	}
	if(length && tracks) {
		printf("Sorry.  Pattern resizing is broken.\n");
		// are we adding rows?
		//   if so, move tracks>1 down accordingly
		// removing some?
		//   if so, move tracks>1 up accordingly
		// are we adding a track?
		//   if so, clear it out
		// removing one?
		//   we don't need to worry about that then.
	}
	length = l;
	tracks = trk;
}

long *gsynth_Pattern::GetData(int track, int row)
{
	if(row>=length) return NULL;
	if(row<0) return NULL;
	if(track>=tracks) return NULL;
	if(track<0) return NULL;

	return patdata + (track*length + row)*(bytes_per_row/sizeof(long));
}

int gsynth_Pattern::SetData(int track, int row, int col, long val)
{
	long *rowdata = GetData(track, row);
	if(!rowdata) return 0;
	if((unsigned)col>=(bytes_per_row/sizeof(long))) return 0;
	if(val != -1) {
		if(val < format[col]->min) val = format[col]->min;
		if(val > format[col]->max) val = format[col]->max;
	}
	rowdata[col] = val;
	return 1;
}

gsynth_Sequence::gsynth_Sequence(SBuffer *mod)
{
	module = mod;
	head = tail = NULL;
	cued_pat = NULL;
	playing_pat = NULL;
	playing_row = 0;
	loop_begin = 0;
	loop_end = 0;
}

gsynth_Sequence::~gsynth_Sequence()
{
	gsynth_seq_entry *ptr, *tmp;
	for(ptr = head; ptr != NULL;) {
		tmp = ptr->next;
		delete tmp;
		ptr = tmp;
	}
}

void gsynth_Sequence::Restart(void)
{
	playing_row = 0;
	pattern_row = 0;
	playing_pat = head;
	if(playing_pat) cued_pat = playing_pat->next;
	else cued_pat = NULL;
}

void gsynth_Sequence::SetEntry(gsynth_Pattern *pat, int row)
{
	gsynth_seq_entry *ptr, *prev, *entry;
	// find position to insert
	for(ptr = head, prev = head; ptr; prev = ptr, ptr = ptr->next)
		if(ptr->row > row) break;
	// at this point, prev->row <= row < ptr->row

	// if something is already at this row, replace it.
	if(prev && prev->row == row) {
		prev->pat = pat;
		return;
	}

	entry = new gsynth_seq_entry;

	// insert node after prev
	entry->pat = pat;
	entry->prev = prev;
	entry->next = ptr;
	entry->row = row;

	if(prev) prev->next = entry;
	else head = entry;
	if(ptr) ptr->prev = entry;
	else tail = entry;
}

int gsynth_Sequence::RemoveEntry(int row)
{
	gsynth_seq_entry *ptr, *prev;
	for(ptr = head, prev = head; ptr; prev = ptr, ptr = ptr->next)
		if(ptr->row > row) break;
	// at this point, prev->row <= row < ptr->row
	if(!prev) return 0;
	if(prev->row != row) return 0;

	// thoroughly confusing, isn't it?
	if(prev->prev) prev->prev->next = prev->next;
	else head = prev->next;
	if(ptr) ptr->prev = prev->prev;
	else tail = prev->prev;
	return 1;
}

gsynth_seq_entry *gsynth_Sequence::find_entry_at(int row)
{
	gsynth_seq_entry *ptr, *prev;
	for(ptr = head, prev = head; ptr; prev = ptr, ptr = ptr->next)
		if(ptr->row >= row) break;
	return ptr;
}

int gsynth_Sequence::Tick(void)
{
	int looped = 0;
	if(!head) return 0; // no patterns to play

	if(playing_pat) 
	{
		list<SynthModule*>::iterator i;
		int trk;
		for(trk=0,i=module->modules.begin();i!=module->modules.end();i++,trk++)
		{
			long *dat = playing_pat->pat->GetData(trk, pattern_row);
			if(dat)
			{
				(*i)->ReadRow(dat);
			}
			else { playing_pat = NULL; break; }
		}
		pattern_row++;
		if(playing_pat && (pattern_row >= playing_pat->pat->GetLength()))
			playing_pat = NULL;
	}

	playing_row++;

	// check for loop
	if(playing_row > loop_end && loop_begin != loop_end) 
	{
		cued_pat = find_entry_at(loop_begin);
		playing_row = loop_begin;
		looped = 1;
	}

	// check for song restart
	if(!cued_pat && !playing_pat)
	{ 
		cued_pat = head; 
		playing_row = 0; 
		looped = 1;
	}

	// check for pattern cue and start if at the right row
	if(cued_pat && playing_row == cued_pat->row) 
	{
		playing_pat = cued_pat;
		pattern_row = 0;
		// cue the next pattern
		cued_pat = cued_pat->next;
	}

	return looped;
}


