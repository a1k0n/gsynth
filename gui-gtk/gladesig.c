/*  gsynth
 *  Copyright (C) 1999 Andy Sloane <andy@guildsoftware.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "gladesrc.h"
#include "gladesig.h"

#include "machbuild.h"
#include "tracker.h"
#include "splash.h"
#include <gui-common/gui-common.h>

static GtkWidget *really_quit_dialog;
static GtkWidget *about_dialog;
static int notebook_page = 0;
int files_unsaved = 0;

int
main (int argc, char *argv[])
{
  GtkWidget *mainwindow;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  splash_init();

  mainwindow = create_mainwindow ();
  really_quit_dialog = create_really_quit_dialog ();
  about_dialog = create_about_dialog ();

  init_machinebuilder (mainwindow);
  init_tracker(mainwindow);

  init_common_gui();

  gtk_widget_show (mainwindow);
  gtk_widget_set_usize (mainwindow, 0, 0);

  gtk_main ();

  destroy_machinebuilder();
  destroy_tracker();

  return 0;
}


void
on_File_activate (GtkMenuItem * menuitem,
		  gpointer user_data)
{

}


void
on_New_Song_activate (GtkMenuItem * menuitem,
		      gpointer user_data)
{
  cb_new_module();
}

void do_open(GtkWidget *w, GtkFileSelection *fs)
{
  cb_load_module(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  gtk_widget_destroy(GTK_WIDGET(fs));
}

void do_save(GtkWidget *w, GtkFileSelection *fs)
{
  cb_save_module(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  gtk_widget_destroy(GTK_WIDGET(fs));
}

void
on_OpenSong                            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *filew = gtk_file_selection_new ("Open Song");

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
                      "clicked", (GtkSignalFunc) do_open, filew);

  gtk_signal_connect_object (GTK_OBJECT(filew), "delete_event", 
                      (GtkSignalFunc)gtk_widget_destroy, GTK_OBJECT(filew));
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
                             "clicked", (GtkSignalFunc) gtk_widget_destroy,
							 GTK_OBJECT (filew));
  gtk_widget_show(filew);

}

void
on_Save_Song_As_activate (GtkMenuItem * menuitem,
			  gpointer user_data)
{
  GtkWidget *filew = gtk_file_selection_new ("Save Song");

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
                      "clicked", (GtkSignalFunc) do_save, filew);

  gtk_signal_connect_object (GTK_OBJECT(filew), "delete_event", 
                      (GtkSignalFunc)gtk_widget_destroy, GTK_OBJECT(filew));
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
                             "clicked", (GtkSignalFunc) gtk_widget_destroy,
							 GTK_OBJECT (filew));
  gtk_widget_show(filew);
}


void
on_Quit_activate (GtkMenuItem * menuitem,
		  gpointer user_data)
{
  if (files_unsaved)
    gtk_widget_show (really_quit_dialog);
  else
    gtk_main_quit ();
}


void
on_load_node_activate (GtkMenuItem * menuitem,
		       gpointer user_data)
{

}


void
on_Load_Machine_activate (GtkMenuItem * menuitem,
			  gpointer user_data)
{

}


void
on_Save_Machine_activate (GtkMenuItem * menuitem,
			  gpointer user_data)
{

}


void
on_Help_activate (GtkMenuItem * menuitem,
		  gpointer user_data)
{

}


void
on_About_activate (GtkMenuItem * menuitem,
		   gpointer user_data)
{
  gtk_widget_show(about_dialog);
}


gboolean
on_mainwindow_delete_event (GtkWidget * widget,
			    GdkEvent * event,
			    gpointer user_data)
{
  if (files_unsaved)
    gtk_widget_show (really_quit_dialog);
  else
    gtk_main_quit ();
  return TRUE;
}


gboolean
on_dialog1_delete_event (GtkWidget * widget,
			 GdkEvent * event,
			 gpointer user_data)
{
  gtk_widget_hide (really_quit_dialog);
  return TRUE;
}


void
on_quit_yes_clicked (GtkButton * button,
		     gpointer user_data)
{
  gtk_main_quit ();
}


void
on_quit_no_clicked (GtkButton * button,
		    gpointer user_data)
{
  gtk_widget_hide (really_quit_dialog);
}



gboolean
on_about_dialog_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide(about_dialog);
  return TRUE;
}


void
on_about_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(about_dialog);
}



void
on_rename_entry_activate               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  on_rename_ok_clicked(NULL, user_data);
}




gboolean
on_about_module_dialog_delete_event    (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_destroy(widget);
    return TRUE;
}


void
on_about_module_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(button))));
}



gboolean
on_mainwindow_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
  int stop = 0;
  switch(notebook_page) {
    case 1:
      stop = on_tracker_key_press_event(widget, event, user_data);
  }
  if(stop) gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");
  return stop;
}


void
on_Show_me_the_elite_splash_screen_again__activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  splash_init();
}


void
on_notebook1_switch_page               (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        gint             page_num,
                                        gpointer         user_data)
{
  notebook_page = page_num;
}


