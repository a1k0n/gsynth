#include <gui-common/gui-common.h>
#include "tracker.h"
#include "gladesrc.h"
#include "support.h"

#define PAD_MARGIN 5

/* Backing pixmap for drawing area */
static GdkPixmap *pixmap = NULL;
static GtkWidget *drawarea = NULL;
static GdkFont *fixed_font = NULL;
static GtkOptionMenu *mlist_menu = NULL;
static GtkOptionMenu *plist_menu = NULL;
static GtkWidget *create_dlg = NULL;

static int window_width, window_height;
static int font_width, font_height;
static int mlist_items = 0;
static int plist_items = 0;

static int style_tbl[] = {
  GTK_STATE_NORMAL,
  GTK_STATE_PRELIGHT,
  GTK_STATE_SELECTED,
  GTK_STATE_ACTIVE
};

void
init_tracker (GtkWidget * mainwindow)
{
  drawarea = lookup_widget (mainwindow, "tracker_drawarea");
  mlist_menu = GTK_OPTION_MENU (lookup_widget (mainwindow,
					    "tracker_module_menu"));
  plist_menu = GTK_OPTION_MENU (lookup_widget (mainwindow,
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

void
tracker_get_size (int *width, int *height)
{
  if(width) *width = (window_width-2*PAD_MARGIN)/font_width;
  if(height) *height = (window_height-2*PAD_MARGIN)/font_height;
}

void
tracker_clear (void)
{
  if(!pixmap) return;
  gdk_draw_rectangle (pixmap, drawarea->style->bg_gc[GTK_STATE_NORMAL], 1,
		      0, 0, window_width, window_height);
}

void
tracker_draw_box (int x, int y, int w, int h, int style)
{
  int state = style_tbl[style];

  x = x*font_width + PAD_MARGIN;
  y = y*font_height + PAD_MARGIN;
  w = w*font_width;
  h = h*font_height;

  if(!pixmap) return;

  gdk_draw_rectangle (pixmap, drawarea->style->bg_gc[state], 1,
		      x, y, w, h);

}

void
tracker_draw_text (int col, int row, const char *text, int style)
{
  int x, y;
  int state = style_tbl[style];
  x = PAD_MARGIN + col * font_width;
  y = PAD_MARGIN + font_height * row;

  if(!pixmap) return;

  gdk_draw_string (pixmap, fixed_font, drawarea->style->fg_gc[state],
		   x, y + font_height - 1, text);
}

void
tracker_flush (void)
{
  if(!pixmap) return;

  gdk_draw_pixmap (drawarea->window, drawarea->style->fg_gc[GTK_STATE_NORMAL],
		   pixmap, 0, 0, 0, 0, window_width, window_height);
}

static int do_list_add(GtkOptionMenu *mlist_menu, int mlist_items, const char *name, void *data, 
	void (*func)(void*))
{
  GtkWidget *menu, *menuitem;
  /* if there are no previous items, nuke the old menu and create a new one */

  menu = gtk_option_menu_get_menu (mlist_menu);
  if (!mlist_items) {
      if (menu)
	gtk_widget_destroy (menu);
      menu = gtk_menu_new ();
  }

  /* create new menu item, append to gtk_menu */
  g_assert (menu);
  menuitem = gtk_menu_item_new_with_label (name);
  gtk_signal_connect_object (GTK_OBJECT (menuitem), "activate",
			     GTK_SIGNAL_FUNC (func),
			     (gpointer) data);
  gtk_object_set_data (GTK_OBJECT (menuitem), "moduledata", data);
  gtk_widget_show (menuitem);
  gtk_menu_append (GTK_MENU (menu), menuitem);
  gtk_option_menu_set_menu (mlist_menu, menu);
  mlist_items++;
  return mlist_items;
}

int
tracker_mlist_add (const char *name, void *data)
{
  int old_items = mlist_items;
  mlist_items = do_list_add(mlist_menu, mlist_items, name, data, cb_tracker_module_select);
  if(!old_items) cb_tracker_module_select(data);
  return mlist_items;
}

int
tracker_plist_add (const char *name, void *data)
{
  int old_items = plist_items;
  plist_items = do_list_add(plist_menu, plist_items, name, data, cb_tracker_pattern_select);
  if(!old_items) cb_tracker_pattern_select(data);
  return plist_items;
}

void
tracker_mlist_clear (void)
{
  /* assuming an add will be done soon after, this is all that's necessary */
  mlist_items = 0;
}

void
tracker_plist_clear (void)
{
  plist_items = 0;
}

/********************************************************************/
/* Gldae-generated callbacks                                        */
/********************************************************************/

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

  w = lookup_widget (create_dlg, "create_pattern_name");
  name = gtk_entry_get_text (GTK_ENTRY (w));
  w = lookup_widget (create_dlg, "create_pattern_length");
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
