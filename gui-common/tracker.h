#ifndef __GUI_COMMON_TRACKER_H
#define __GUI_COMMON_TRACKER_H

#include <libgsyn/buffer.h>
#include <libgsyn/pattern.h>

class TrackEditor
{
private:
	gsynth_Pattern *editpat;
	SBuffer *editbuf;
	int hlrow, hlcol, hltrack; // highlighted column/row/track
	int rowoffset; // top displayed row (for scrolling)
	int win_height;
	int pat_cols;
	int default_octave;

	void redraw_cell(int track, int row, int col);
	void redraw_rowcell(int row);
	void render_tracker_cell(int,int,SynthParameter*,long,int);
	void regenerate_tracker_plist(void);
	int process_tracker_key(int key, int state);
	void update_highlight(int oldrow, int oldcol);
public:
	TrackEditor();
	~TrackEditor();

	void Redraw();
//	void OnMouseButton(int btn, int down, int x, int y);  these will eventually be used 
//	void OnMouseMove(int x, int y); for mouse selection and stuff.
	int OnKey(int key, int state);
	void OnModuleSelect(SBuffer *mod);
	void OnPatternSelect(gsynth_Pattern *pat);
	void OnCreatePattern(const char *name, int len);
};

#endif

