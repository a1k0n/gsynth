#include <images/gsyn.splash.xpm>
#include <sys/stat.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <glib.h>

static GtkWidget *create_splash()
{
  GtkWidget *winwidget;
  GtkWidget *pixwidget;
  GdkPixmap *gdkpixmap;
  GdkBitmap *mask;
  GdkColormap *colormap;

  winwidget = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_position(GTK_WINDOW(winwidget), GTK_WIN_POS_CENTER);
  gtk_window_set_policy(GTK_WINDOW(winwidget), FALSE, FALSE, TRUE);

  colormap = gtk_widget_get_colormap(winwidget);
  gdkpixmap = gdk_pixmap_colormap_create_from_xpm_d(NULL, colormap, &mask, 
  	NULL, gsyn_splash_xpm);
  pixwidget = gtk_pixmap_new(gdkpixmap, mask);
  gdk_pixmap_unref(gdkpixmap);
  gdk_bitmap_unref(mask);
  gtk_widget_show(pixwidget);

  gtk_container_add(GTK_CONTAINER(winwidget), pixwidget);

  return winwidget;
}

static gboolean nuke_splash(GtkWidget *splashwindow)
{
  gtk_widget_destroy(splashwindow);
  return FALSE;
}

void splash_init(void)
{
  GtkWidget *splash = create_splash();
  gtk_widget_show(splash);
  g_timeout_add(2000, (GSourceFunc) nuke_splash, (gpointer)splash);
}

