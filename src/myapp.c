/*  myapp.c

    This file is part of myapp1.
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#include "private.h"

typedef struct _MyAppData MyAppData;
struct _MyAppData
{
  gchar *theme_dir;
};
struct _MyApp
{
  GtkApplication parent_instance;
  MyAppData data;
};
struct _MyAppClass
{
  GtkApplicationClass parent_class;
};

G_DEFINE_TYPE (MyApp, my_app, GTK_TYPE_APPLICATION) 

static void
my_app_activate (GApplication *gapp)
{
  MyApp *app; 
  MyAppWindow *win;
  GtkWidget *pic;
  GtkStyleContext *ctx;
  GtkCssProvider *cssprovider;
  char *picpath, *csspath;
  app = MY_APP (gapp);
  win = my_app_window_new (app);

  picpath = g_strconcat(app->data.theme_dir, "/", "myfile.png", NULL);
  pic = gtk_picture_new_for_filename (picpath);
  g_free(picpath);

  gtk_window_set_child (GTK_WINDOW (win), pic);

  ctx = gtk_widget_get_style_context(GTK_WIDGET (win));
  cssprovider = gtk_css_provider_new();
  csspath = g_strconcat(app->data.theme_dir, "/", "myapp.css", NULL);
  gtk_css_provider_load_from_path(cssprovider, csspath);
  gtk_style_context_add_provider (ctx, GTK_STYLE_PROVIDER(cssprovider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  
  gtk_window_present (GTK_WINDOW (win));
}

static int
my_app_handle_local_options (G_GNUC_UNUSED GApplication *gapp, GVariantDict *options)
{
  /* Already get cmdline data at app->data */
  /* Set default if NULL */
  MyApp *app = MY_APP (gapp);
  if (!g_variant_dict_lookup (options, "theme-dir", "s", &app->data.theme_dir)) {
    app->data.theme_dir = g_strdup("./");
  }

  // Return -1 to continue running the application. See documentation for GApplication "handle_local_options"
  // Don't manually call g_application_activate() here; return -1 will continue calling "activate"
  return -1;
}

static void
my_app_finalize (GObject *gobj)
{
  MyApp *app = MY_APP (gobj);
  g_free (app->data.theme_dir);
}

static void
my_app_class_init (MyAppClass *klass)
{
  GApplicationClass *gappclass = G_APPLICATION_CLASS (klass);
  GObjectClass *gobjclass = G_OBJECT_CLASS (klass);
  gappclass->activate = my_app_activate;
  gappclass->handle_local_options = my_app_handle_local_options;
  gobjclass->finalize = my_app_finalize;
}

static void
my_app_init (G_GNUC_UNUSED MyApp *app)
{
}

MyApp *
my_app_new (void)
{
  MyApp *app;
  app = g_object_new (MY_TYPE_APP, 
                      "application-id", MYAPP1_CONFIG_APPID,
                      "flags", G_APPLICATION_FLAGS_NONE,
                      NULL);
  return app;
}
