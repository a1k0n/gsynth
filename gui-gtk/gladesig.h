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

#include <gtk/gtk.h>


gboolean
on_mainwindow_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_File_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_New_Song_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_OpenSong                            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_SavePat_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Save_Song_As_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_load_node_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Load_Machine_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Save_Machine_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Help_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_machine_drawarea_expose_event       (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_machine_drawarea_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_machine_drawarea_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_machine_drawarea_button_release_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_machine_drawarea_motion_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

gboolean
on_dialog1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_quit_yes_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_quit_no_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_add_module_dialog_delete_event      (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_add_module_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_add_module_cancel_clicked           (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_about_dialog_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_about_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_Add_Module_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_Remove_Module_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Module_Properties_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_add_module_clist_select_row         (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_tracker_drawarea_configure_event    (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_tracker_drawarea_key_press_event    (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_tracker_drawarea_expose_event       (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

void
on_Rename_Module_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_rename_dlg_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_rename_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_rename_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_rename_entry_activate               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_About_this_module_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_about_module_dialog_delete_event    (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_about_module_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_tracker_create_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_create_pattern_dialog_delete_event  (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_create_pattern_ok_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_create_pattern_cancel_clicked       (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_tracker_key_press_event             (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_tracker_key_press_event             (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_mainwindow_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

void
on_Show_me_the_elite_splash_screen_again__activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_notebook1_switch_page               (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        gint             page_num,
                                        gpointer         user_data);

void
on_adjust_volume_panning1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_disconnect1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

