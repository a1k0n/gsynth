#include <string.h>
#include "machbuild.h"
#include "gladesrc.h"
#include "support.h"
#include <math.h>
#include <gui-common/gui-common.h>

/* Backing pixmap for drawing area */
static GdkPixmap *pixmap = NULL;
static GtkWidget *add_module_dialog = NULL;
static GtkWidget *rename_module_dialog = NULL;
static GtkWidget *drawarea = NULL;
static GtkWidget *popup = NULL;
static GtkWidget *popup2 = NULL;
static GtkWidget *popup3 = NULL;
static GtkCList *clist = NULL;
static void *clist_selectiondata = NULL;
static int mwidth = 0, mheight = 0;

void init_machinebuilder(GtkWidget *mainwindow)
		/*GtkDrawingArea *area, GtkWidget *pop, GtkWidget *pop2, GtkCList *cl) */
{
  drawarea = lookup_widget(mainwindow, "machine_drawarea");
  add_module_dialog = create_add_module_dialog();
  rename_module_dialog = create_rename_dlg();
  popup = create_machine_popup();
  popup2 = create_machine_popup_1();
  popup3 = create_machine_popup_2();
  clist = GTK_CLIST(lookup_widget(add_module_dialog, "add_module_clist"));
}

void destroy_machinebuilder(void)
{
}

void machine_position_abs_to_rel(int x, int y, float *fx, float *fy)
{
  *fx = (float) x/mwidth;
  *fy = (float) y/mheight;
}

void machine_position_rel_to_abs(float x, float y, int *ix, int *iy)
{
  x*=mwidth; y*=mheight;
  *ix = x; *iy = y;
}

/* fixed size, regardless of name for now */
int machine_hit_test(const char *name, float mx, float my, float x, float y)
{
  mx *= mwidth;  x *= mwidth;
  my *= mheight; y *= mheight;
  if(x > (mx-50) && x < (mx+50) && y > (my-20) && y < (my+20))
    return 1;
  return 0;
}

int gtk_state_table[NUM_MACHINE_STATES] = {
	GTK_STATE_NORMAL,
	GTK_STATE_NORMAL,
	GTK_STATE_NORMAL,
	GTK_STATE_PRELIGHT,
	GTK_STATE_SELECTED };

void draw_machine_box(const char *label, float x, float y, int state)
{
  int sx, sy, w=100, h=40;
  int labellen = strlen(label);
  sx = (mwidth)*x - (w/2);
  sy = (mheight)*y - (h/2);
  gdk_draw_rectangle (pixmap,
		      drawarea->style->bg_gc[gtk_state_table[state]],
		      TRUE, sx, sy, w, h);
  gdk_draw_rectangle (pixmap,
		      drawarea->style->fg_gc[gtk_state_table[state]],
		      FALSE, sx, sy, w, h);

  w = gdk_text_width(drawarea->style->font, label, labellen);
  h = gdk_text_height(drawarea->style->font, label, labellen);
  sx = (mwidth)*x - w/2;
  sy = (mheight)*y + h/2;
  gdk_draw_text (pixmap,
  		 drawarea->style->font,
		 drawarea->style->fg_gc[gtk_state_table[state]],
		 sx, sy, label, labellen);
}

void draw_machine_connection(float x1, float y1, float x2, float y2)
{
  float vx, vy, px, py, t;
  float dx, dy;
  GdkPoint poly[3];

  x1*=mwidth; y1*=mheight;
  x2*=mwidth; y2*=mheight;

  vx = x2 - x1; vy = y2 - y1; /* make vector from line */
  t = vx*vx+vy*vy; if(t==0) return; /* normalize, store in dx,dy */
  t = sqrt(t); dx=vx/t; dy=vy/t;

  dx*=8; dy*=8;
  px = -dy; py = dx; /* rotate -90 degrees for px,py */

  gdk_draw_line (pixmap, drawarea->style->fg_gc[GTK_STATE_NORMAL],
  	x1, y1, x2, y2);

  x1 += vx/2; y1 += vy/2;
  x1 -= dx; y1 -= dy;
/*
  gdk_draw_line (pixmap, drawarea->style->fg_gc[GTK_STATE_NORMAL],
  	x1+px, y1+py, x1-px, y1-py);
  gdk_draw_line (pixmap, drawarea->style->fg_gc[GTK_STATE_NORMAL],
  	x1+px, y1+py, x1+2*dx, y1+2*dy);
  gdk_draw_line (pixmap, drawarea->style->fg_gc[GTK_STATE_NORMAL],
  	x1-px, y1-py, x1+2*dx, y1+2*dy);
*/
  poly[0].x = x1+px; poly[0].y = y1+py;
  poly[1].x = x1-px; poly[1].y = y1-py;
  poly[2].x = x1+2*dx; poly[2].y = y1+2*dy;
  gdk_draw_polygon (pixmap, drawarea->style->bg_gc[GTK_STATE_PRELIGHT],
    TRUE, poly, 3);
  gdk_draw_polygon (pixmap, drawarea->style->fg_gc[GTK_STATE_PRELIGHT],
    FALSE, poly, 3);
}

void clear_machine_window(void)
{
  gdk_draw_rectangle (pixmap,
		      drawarea->style->bg_gc[GTK_STATE_NORMAL],
		      TRUE,
		      0, 0,
		      drawarea->allocation.width,
		      drawarea->allocation.height);
}

void flush_machine_window(void)
{
  gdk_draw_pixmap(drawarea->window,
  		  drawarea->style->fg_gc[GTK_WIDGET_STATE (drawarea)],
		  pixmap, 0, 0, 0, 0, mwidth, mheight);
}

gboolean
on_machine_drawarea_expose_event       (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		  pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return TRUE;
}


gboolean
on_machine_drawarea_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  if (pixmap)
    gdk_pixmap_unref(pixmap);

  mwidth = widget->allocation.width;
  mheight = widget->allocation.height;
  pixmap = gdk_pixmap_new(widget->window,
			  widget->allocation.width,
			  widget->allocation.height,
			  -1);
  cb_machine_repaint();

  return TRUE;
}


gboolean
on_machine_drawarea_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  int mystate;
  if(event->button == 3 && !(event->state & 5)) {
	int which = cb_machine_popup_dialog();
	switch(which) {
		case 0:
			gtk_menu_popup(GTK_MENU(popup), NULL, NULL, NULL, NULL,
				event->button, event->time);
			break;
		case 1:
			gtk_menu_popup(GTK_MENU(popup2), NULL, NULL, NULL, NULL,
				event->button, event->time);
			break;
		case 2:
			gtk_menu_popup(GTK_MENU(popup3), NULL, NULL, NULL, NULL,
				event->button, event->time);
			break;
	}
	return TRUE;
  }
/*  printf("button %d press; state=%04x\n", event->button, event->state); */
  mystate = GUI_STATE_BUTTON_DOWN;
  if(event->state&1) mystate|=GUI_STATE_SHIFT;
  cb_machine_mouse_button(event->button, mystate, event->x, event->y);
  return TRUE;
}


gboolean
on_machine_drawarea_button_release_event (GtkWidget       *widget,
                                          GdkEventButton  *event,
                                          gpointer         user_data)
{
  int mystate=0;
  if(event->state&1) mystate=GUI_STATE_SHIFT;
/*  printf("button %d release; state=%04x\n", event->button, event->state); */
  cb_machine_mouse_button(event->button, mystate, event->x, event->y);
  return TRUE;
}


gboolean
on_machine_drawarea_motion_notify_event (GtkWidget       *widget,
                                         GdkEventMotion  *event,
                                         gpointer         user_data)
{
  cb_machine_mouse_move(event->x, event->y);
  return TRUE;
}

void add_machine_name(const char *name, void *modinfo)
{
  int clist_row;
  gchar *namelist[] = {(char*)name, NULL};
  clist_row = gtk_clist_append(clist, namelist);
  gtk_clist_set_row_data(clist, clist_row, modinfo);
}

gboolean
on_add_module_dialog_delete_event      (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide(add_module_dialog);
  return TRUE;
}


void
on_add_module_cancel_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(add_module_dialog);
}


void
on_add_module_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(add_module_dialog);
  if(!clist_selectiondata) return;
  cb_machine_add(clist_selectiondata);
  clist_selectiondata = NULL;
  gtk_clist_unselect_all(clist);
}


void
on_add_module_clist_select_row         (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  clist_selectiondata = gtk_clist_get_row_data(clist, row);
}


void
on_Remove_Module_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  cb_machine_remove();
}


void
on_Module_Properties_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_Rename_Module_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    const char *mname;
    GtkEntry *edit = GTK_ENTRY(lookup_widget(rename_module_dialog, "rename_entry"));
    mname = cb_machine_get_modulename();
    gtk_entry_set_text(edit, mname);
    gtk_entry_select_region(edit, 0, strlen(mname));
    gtk_widget_grab_focus(GTK_WIDGET(edit));
    gtk_widget_show(rename_module_dialog);
}


void
on_Add_Module_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show(add_module_dialog);
}


gboolean
on_rename_dlg_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide(rename_module_dialog);
  return TRUE;
}


void
on_rename_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkEntry *edit = GTK_ENTRY(lookup_widget(rename_module_dialog, "rename_entry"));
    cb_machine_rename(gtk_entry_get_text(edit));
    gtk_widget_hide(rename_module_dialog);
}


void
on_rename_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(rename_module_dialog);
}

void
on_About_this_module_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkWidget *about_window;
    GtkLabel *label_text;
    const char *stuff = cb_machine_about_module();
    if(!stuff) stuff = "No information available about this module.";
    about_window = create_about_module_dialog();
    label_text = GTK_LABEL(lookup_widget(about_window, "about_module_text"));
    gtk_label_set_text(label_text, stuff);
    gtk_widget_show(about_window);
}

void
on_adjust_volume_panning1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_disconnect1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    cb_machine_disconnect();
}

