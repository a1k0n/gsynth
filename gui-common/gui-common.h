/* Common GUI functions.  Reimplement these to create a new GUI for gsynth. */
/* These are all C functions, by the way.  Keep that in mind for C++ code.  */

#ifndef __GUI_COMMON_H
#define __GUI_COMMON_H

#include <gui-common/keys.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Called initially from main or WinMain or what-have-you */
extern void init_common_gui(void);

/*
 * Top level GUI stuff
 */

extern void gui_set_statusbar(const char *status);

/********************************************************************/
/* Machine Editor GUI stuff                                         */
/********************************************************************/

enum {
	MACHINE_MASTER = 0,
	MACHINE_EFFECT,
	MACHINE_GENERATOR,
	MACHINE_PRELIGHT,
	MACHINE_SELECTED,

	NUM_MACHINE_STATES
};

/* Converts positions within window to floats in [0..1] and back.   */
extern void machine_position_abs_to_rel(int x, int y, float *fx, float *fy);
extern void machine_position_rel_to_abs(float x, float y, int *ix, int *iy);

/* Tests to see if coordinate x, y is inside a machine named 'name' at 
   mx, my */
extern int machine_hit_test(const char *name, float mx, float my, 
	float x, float y);

/* Call clear, draw, draw, draw, etc, flush to redraw. */
extern void clear_machine_window(void);
extern void draw_machine_box(const char *label, float x, float y, int state);
extern void draw_machine_connection(float x1, float y1, float x2, float y2);
extern void flush_machine_window(void);

/* Add machine names to the "Add machine..." dialog box */
extern void add_machine_name(const char *name, void *modinfo);

/*****************************************************************/
/* Callbacks from machine editor - implemented in gui-common.cpp */
/*****************************************************************/
#define GUI_STATE_BUTTON_DOWN 1
#define GUI_STATE_SHIFT 2
extern void cb_machine_repaint();
extern void cb_machine_mouse_button(int btn, int state, int x, int y);
extern void cb_machine_mouse_move(int x, int y);
extern void cb_machine_add(void *modinfo);
extern void cb_machine_remove(void); /* gui-common knows which machine was
										selected when this was called */
extern void cb_machine_disconnect(void);
extern void cb_machine_rename(const char *newname);
extern const char *cb_machine_get_modulename(void);
extern const char *cb_machine_about_module(void);

/* returns which dialog to pop up */
/* which=0: "Add machine" dialog; 1: "Remove machine"/machine properties; 
 * 2: volume/pan/disconnect dialog */
extern int cb_machine_popup_dialog();


/*****************************************************************/
/* Tracker GUI stuff                                             */
/*****************************************************************/

/* 
 * assumes a fixed-width font and that textwidth=colsize,
 * textheight=rowsize  
 *
 * styles: 0 = normal (black on grey), 1 = highlight (black on bright grey),
 * 2 = select (white on blue), 3 = dark (black on darker grey) 
 */
extern void tracker_get_size(int *width, int *height);
extern void tracker_clear(void);
extern void tracker_draw_box(int x, int y, int w, int h, int style);
extern void tracker_draw_text(int col, int row, const char *text, int style);
extern void tracker_scroll_text(int startrow, int endrow, int delta);
extern void tracker_flush(void);

/* add/remove elements to the module listbox */
extern int tracker_mlist_add(const char *name, void *data);
extern void tracker_mlist_clear(void);

/* add/remove elements from the pattern listbox */
extern int tracker_plist_add(const char *name, void *data);
extern void tracker_plist_clear();

/*****************************************************************/
/* Callbacks from track editor - implemented in gui-common.cpp   */
/*****************************************************************/

extern void cb_tracker_module_select(void *module);
extern void cb_tracker_pattern_select(void *pattern);
extern void cb_tracker_repaint();
extern int cb_tracker_key(int keycode, int state);
extern void cb_tracker_addtrack(void);
extern void cb_tracker_removetrack(void);
extern void cb_tracker_create_pattern(const char *name, int len);


/*****************************************************************/
/* Sequencer GUI stuff                                           */
/*****************************************************************/

extern void seq_get_size(int *width, int *height);
extern void seq_clear(void);
extern void seq_draw_box(int x, int y, int w, int h, int style);
extern void seq_draw_text(int col, int row, const char *text, int style);
extern void seq_scroll_text(int startrow, int endrow, int delta);
extern void seq_flush(void);

/*****************************************************************/
/* Callbacks from track editor - implemented in gui-common.cpp   */
/*****************************************************************/

extern void cb_seq_repaint();
extern int cb_seq_key(int keycode, int state);

/*****************************************************************/
/* Callbacks from menubar - implemented in gui-common.cpp        */
/*****************************************************************/

extern int cb_new_module(void);
extern int cb_load_module(const char *name);
extern int cb_save_module(const char *name);

#ifdef __cplusplus
}
#endif

#endif

