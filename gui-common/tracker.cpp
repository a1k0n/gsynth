#include "tracker.h"
#include "gui-common.h"

#include <stdio.h>

static char *hexdigit="0123456789abcdef";
static char *notename="C-C#D-D#E-F-F#G-G#A-A#B-";

#define TRACKER_PAGE_SIZE 16

TrackEditor::TrackEditor()
{
	editpat = NULL;
	editbuf = NULL;
	hlrow = hlcol = hltrack = 0;
	rowoffset = 0;
	default_octave = 1;
}

TrackEditor::~TrackEditor()
{
}

void TrackEditor::render_tracker_cell(int col, int row, SynthParameter *fmt, long dat, int style)
{
	char buf[80];
	int i;
	if(!fmt->digits) {
		printf("render_tracker_cell: digits=0!\n");
		return;
	}
	if(fmt->type == 0) {
		if(dat == -1) { for(i=0;i<fmt->digits;i++) buf[i]='.'; }
		else {
			for(i=0;i<fmt->digits;i++) {
				buf[(fmt->digits)-1-i] = hexdigit[dat&0x0f]; 
				dat>>=4;
			}
		}
		buf[fmt->digits]=0;
	} else if(fmt->type == 1) {
		int note, oct;
		for(i=0;i<fmt->digits;i++) buf[i]='.';
		buf[fmt->digits]=0;
		if(dat!=-1) {
			note = dat%12; oct = dat/12;
			buf[0]=notename[note*2];
			buf[1]=notename[note*2+1];
			buf[2]=oct+'0';
		}
	}
	else buf[0] = 0;

	tracker_draw_text(col, row, buf, style);
}

void TrackEditor::Redraw()
{
	int i, j, k, col, n, colwidth;
	int row;
	int style = 0;
	long *patdata;
	SynthParameter **fmt;

	tracker_clear();

	if(!editpat) {
		tracker_flush();
		return;
	}

	fmt = editpat->GetFormat();
	for(i=0,colwidth=0;fmt[i];i++) colwidth+=fmt[i]->digits+1;
	if(!colwidth) return;
	colwidth--;

	tracker_get_size(NULL, &n);
	win_height = n-1;
	if(n > (editpat->GetLength()+1)) n = editpat->GetLength()+1;

	tracker_draw_box(4, 0, editpat->GetTracks() * (colwidth+1) - 1, 1, 1);
	for(j=0;j<editpat->GetTracks();j++) {
		for(k=0,col=0;fmt[k];k++) {
			tracker_draw_text(4+j*(colwidth+1) + col, 0, fmt[k]->name, 1);
			col+=fmt[k]->digits+1;
		}
	}

	for(i=1,row=rowoffset;i<n;i++,row++) {
		char buf[5];
		int s=0;
		sprintf(buf, "%03X", row);
		if((row&3) == 0) s = 3;
		if(row == hlrow) s = 2;
		tracker_draw_box(0,i, 3,1, s);
		tracker_draw_text(0,i, buf, s);
	}

	for(j=0;j<editpat->GetTracks();j++) {
		row = rowoffset;
		for(i=1;i<n;i++,row++) {
			style = 0;
			if((row&3)==0) style = 3;

			tracker_draw_box(4+j*(colwidth+1), i, colwidth, 1, style);

			patdata = editpat->GetData(j, row);
			if(!patdata) continue;
			for(k=0,col=0;fmt[k];k++) {
				int s;
				if(row == hlrow && k == hlcol) {
					// draw cursor highlight
					s = 2;
					tracker_draw_box(4+j*(colwidth+1) + col, i, fmt[k]->digits,1, s);
				}
				else s = style;
				render_tracker_cell(4+j*(colwidth+1) + col, i, fmt[k],
						patdata[k], s);
				col+=fmt[k]->digits+1;
			}
		}
	}
	tracker_flush();
}

void TrackEditor::redraw_cell(int track, int row, int col)
{
	int textcol, textrow, colwidth, coloffset = 0;
	int s,i;
	long *patdata;
	SynthParameter **fmt;
	fmt = editpat->GetFormat();
	
	patdata = editpat->GetData(track, row);
	if(!patdata) return;

	// one of these days i'll precalculate these into an array every time the
	// pattern format changes so we won't have to do this every time a cell is
	// redrawn.
	for(i=0,colwidth=0;fmt[i];i++) {
		if(i == col) coloffset = colwidth;
		colwidth+=fmt[i]->digits+1;
	}
	if(!colwidth) return;

	textcol = 4+track*colwidth + coloffset;
	textrow = row - rowoffset + 1;

	s = 0; 
	if((row&3)==0) s = 3;
	if(row == hlrow && col == hlcol) s = 2;

	tracker_draw_box(textcol, textrow, fmt[col]->digits,1, s);
	render_tracker_cell(textcol, textrow, fmt[col], patdata[col], s);
}

void TrackEditor::redraw_rowcell(int row)
{
	char buf[5];
	int s;
	int textrow = row - rowoffset + 1;
	s = 0; 
	if((row&3)==0) s = 3;
	if(row == hlrow) s = 2;

	sprintf(buf, "%03X", row);
	tracker_draw_box(0, textrow, 3,1, s);
	tracker_draw_text(0,textrow, buf, s);
}

void TrackEditor::update_highlight(int oldrow, int oldcol)
{
	if(hlrow == oldrow && hlcol == oldcol) return;
	redraw_cell(hltrack, oldrow, oldcol);
	if(oldrow != hlrow) redraw_rowcell(oldrow);
	if(hlrow<rowoffset) { rowoffset = hlrow; Redraw(); } // FIXME: scroll properly
	else if(hlrow >= rowoffset+win_height) { rowoffset = hlrow-win_height+1; Redraw(); }
	else {
		redraw_cell(hltrack, hlrow, hlcol);
		if(oldrow != hlrow) redraw_rowcell(hlrow);
		tracker_flush();
	}
}

int TrackEditor::OnKey(int key, int state)
{
	int oldrow = hlrow;
	int oldcol = hlcol;
	switch(key) {
		case GUI_KEY_UP:
			if(hlrow) hlrow--;
			update_highlight(oldrow, oldcol);
			return 1;
		case GUI_KEY_DOWN:
			if(hlrow < (editpat->GetLength() - 1)) hlrow++;
			update_highlight(oldrow, oldcol);
			return 1;
		case GUI_KEY_LEFT:
			if(hlcol) hlcol--;
			update_highlight(oldrow, oldcol);
			return 1;
		case GUI_KEY_RIGHT:
			if(hlcol < (pat_cols-1)) hlcol++;
			update_highlight(oldrow, oldcol);
			return 1;
		case GUI_KEY_PGUP:
			hlrow -= TRACKER_PAGE_SIZE; if(hlrow < 0) hlrow = 0;
			update_highlight(oldrow, oldcol);
			return 1;
		case GUI_KEY_PGDN:
			hlrow += TRACKER_PAGE_SIZE; 
			if(hlrow >= editpat->GetLength()) 
				hlrow = editpat->GetLength() - 1;
			update_highlight(oldrow, oldcol);
			return 1;
	}
	if(key >= '.' && key <= 'z') { // or something
		if(process_tracker_key(key, state)) {
			redraw_cell(hltrack, hlrow, hlcol);
			tracker_flush();
			return 1;
		}
	}
	printf("OnKey(%x, %d)\n", key, state);
	return 0;
}


void TrackEditor::OnModuleSelect(SBuffer *mod)
{
	editbuf = mod;
	editpat = NULL;
	regenerate_tracker_plist();
	if(!editpat) Redraw();
}

void TrackEditor::OnPatternSelect(gsynth_Pattern *pat)
{
	SynthParameter **fmt;
	if(editpat == pat) return;
	editpat = pat;
	if(editpat) {
		fmt = editpat->GetFormat();
		for(pat_cols = 0;fmt[pat_cols];pat_cols++);
	}
	hltrack = hlrow = hlcol = 0;
	Redraw();
}

void TrackEditor::OnCreatePattern(const char *name, int len)
{
	gsynth_Pattern *new_pattern;
	if(!editbuf) return;
	new_pattern = editbuf->new_pattern(len, name);
	regenerate_tracker_plist();
	// select correct item in plist here... otherwise regenerate() just
	// reselects first one in the list.
	OnPatternSelect(new_pattern);
}

void TrackEditor::regenerate_tracker_plist(void)
{
	list<gsynth_Pattern*>::iterator i;

	tracker_plist_clear();
	if(!editbuf->patterns.size()) {
		tracker_plist_add("(no patterns)", NULL);
		return;
	}

	for(i=editbuf->patterns.begin(); i!=editbuf->patterns.end(); i++)
		tracker_plist_add((*i)->GetName(), (*i));
}

int TrackEditor::process_tracker_key(int key, int)
{
	SynthParameter **fmt;
	int min, max;
	fmt = editpat->GetFormat();
	if(key == '.') {
		editpat->SetData(hltrack, hlrow, hlcol, -1);
		return 1;
	}
	if(fmt[hlcol]->type == 0) {
		min = fmt[hlcol]->min;
		max = fmt[hlcol]->max;
		// use 0-9 as a range between min and max values
		if(key < '0' || key > '9') return 0;
		long val = key-'0';
		if(min == 0 && max <= 9) {
			editpat->SetData(hltrack, hlrow, hlcol, val);
			return 1;
		}
		val = min + (max-min)*val/9;
		editpat->SetData(hltrack, hlrow, hlcol, val);
		return 1;
	}
	if(fmt[hlcol]->type == 1) {
		int notenum, octave;
		long *data = editpat->GetData(hltrack, hlrow);
		if(data[hlcol] == -1) {
			notenum = 0;
			octave = default_octave;
		}
		else {
			notenum = data[hlcol]%12;
			octave = data[hlcol]/12;
		}
		switch(key) {
			case 'y': /* for German keyboards */
			case 'z': notenum = 0; break;
			case 's': notenum = 1; break;
			case 'x': notenum = 2; break;
			case 'd': notenum = 3; break;
			case 'c': notenum = 4; break;
			case 'v': notenum = 5; break;
			case 'g': notenum = 6; break;
			case 'b': notenum = 7; break;
			case 'h': notenum = 8; break;
			case 'n': notenum = 9; break;
			case 'j': notenum = 10; break;
			case 'm': notenum = 11; break;
			case ',': notenum = 0; octave++; break;
			default: 
				if(key >= '0' && key <= '9') octave = key - '0';
				else return 0;
		}
		editpat->SetData(hltrack, hlrow, hlcol, octave*12 + notenum);
		default_octave = octave;
		return 1;
	}

	return 0;
}

