#include "stdafx.h"
extern "C"
{
#include <gui-common/gui-common.h>
}
#include "wtracker.h"

#define PAD_MARGIN 40

/* Backing pixmap for drawing area */
/*
static GdkPixmap *pixmap = NULL;
static GtkWidget *drawarea = NULL;
static GdkFont *fixed_font = NULL;
static GtkOptionMenu *mlist_menu = NULL;
static GtkOptionMenu *plist_menu = NULL;
static GtkWidget *create_dlg = NULL;

static int style_tbl[] = {
  GTK_STATE_NORMAL,
  GTK_STATE_PRELIGHT,
  GTK_STATE_SELECTED,
  GTK_STATE_ACTIVE
};
*/

extern int window_width, window_height;
extern int font_width, font_height;
extern int mlist_items;
extern int plist_items;
extern CDC *Tpdc;
extern CBitmap *Toldbitmap;
extern CBitmap *Tpbitmap;
extern CPen **Tpenlist;
extern CBrush **Tbrushlist;
extern CWnd *childwnd;

/*this should be called in main, when all the other init_ functions are called.

void
init_tracker (GtkWidget * mainwindow)
{
  drawarea = get_widget (mainwindow, "tracker_drawarea");
  mlist_menu = GTK_OPTION_MENU (get_widget (mainwindow,
					    "tracker_module_menu"));
  plist_menu = GTK_OPTION_MENU (get_widget (mainwindow,
					    "tracker_pattern_menu"));
  fixed_font = gdk_font_load ("fixed");
  g_assert (fixed_font);
  font_width = gdk_char_width (fixed_font, 'm');
  font_height = gdk_char_height (fixed_font, 'W') + 3;
}

void
destroy_tracker (void)
{
  if (fixed_font)
    gdk_font_unref (fixed_font);
  if (pixmap)
    gdk_pixmap_unref (pixmap);
}
*/
void
tracker_get_size (int *width, int *height)
{
  if(width) *width = (window_width-2*PAD_MARGIN)/font_width;
  if(height) *height = (window_height-2*PAD_MARGIN)/font_height;
}

void
tracker_clear (void)
{
	if(!Tpdc) return;
	Tpdc->FillSolidRect(0,0,window_width, window_height, RGB(192,192,192));
}

void
tracker_draw_box (int x, int y, int w, int h, int style)
{
	x = x*font_width + PAD_MARGIN;
	y = y*font_height + PAD_MARGIN;
	w = w*font_width;
	h = h*font_height;
	Tpdc->FillRect(CRect(x,y,x+w,y+h), Tbrushlist[style]);
}

void
tracker_draw_text (int col, int row, const char *text, int style)
{
	int x, y;
	x = PAD_MARGIN + col * font_width;
	y = PAD_MARGIN + font_height * row;
	Tpdc->SelectObject(Tpenlist[style]);
	Tpdc->SelectObject(Tbrushlist[style]);
//	Tpdc->SetTextAlign(TA_LEFT | TA_BASELINE);
	LOGPEN pencol;
	Tpenlist[style]->GetLogPen(&pencol);
	Tpdc->SetTextColor(pencol.lopnColor);
	Tpdc->TextOut(x,y,text, strlen(text));
}

void
tracker_flush (void)
{
	childwnd->Invalidate(FALSE);
}

/********************************************************************/
/* Gldae-generated callbacks                                        */
/********************************************************************/
/*
gboolean
on_tracker_drawarea_configure_event (GtkWidget * widget,
				     GdkEventConfigure * event,
				     gpointer user_data)
{
  if (pixmap)
    gdk_pixmap_unref (pixmap);

  window_width = widget->allocation.width;
  window_height = widget->allocation.height;
  pixmap = gdk_pixmap_new (widget->window, widget->allocation.width,
			   widget->allocation.height, -1);

  cb_tracker_repaint ();

  return TRUE;
}


gboolean
on_tracker_key_press_event         (GtkWidget * widget,
				     GdkEventKey * event,
				     gpointer user_data)
{
  return cb_tracker_key(event->keyval, event->state);
}


gboolean
on_tracker_drawarea_expose_event (GtkWidget * widget,
				  GdkEventExpose * event,
				  gpointer user_data)
{
  gdk_draw_pixmap (widget->window, widget->style->fg_gc[GTK_STATE_NORMAL], pixmap,
		   event->area.x, event->area.y,
		   event->area.x, event->area.y,
		   event->area.width, event->area.height);

  return TRUE;
}

void
on_tracker_create_button_clicked (GtkButton * button,
				  gpointer user_data)
{
  if (!create_dlg)
    create_dlg = create_create_pattern_dialog ();
  gtk_widget_show(create_dlg);
}


gboolean
on_create_pattern_dialog_delete_event (GtkWidget * widget,
				       GdkEvent * event,
				       gpointer user_data)
{
  if (create_dlg)
    gtk_widget_destroy (create_dlg);
  create_dlg = NULL;
  return TRUE;
}


void
on_create_pattern_ok_clicked (GtkButton * button,
			      gpointer user_data)
{
  const char *name;
  int len;
  GtkWidget *w;

  w = get_widget (create_dlg, "create_pattern_name");
  name = gtk_entry_get_text (GTK_ENTRY (w));
  w = get_widget (create_dlg, "create_pattern_length");
  len = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (w));

  cb_tracker_create_pattern (name, len);
  if (create_dlg)
    gtk_widget_destroy (create_dlg);
  create_dlg = NULL;
}


void
on_create_pattern_cancel_clicked (GtkButton * button,
				  gpointer user_data)
{
  if (create_dlg)
    gtk_widget_destroy (create_dlg);
  create_dlg = NULL;
}
*/