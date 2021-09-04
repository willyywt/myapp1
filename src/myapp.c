/*  myapp.c

    This file is part of myapp1.
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#include "private.h"

struct _MyApp
{
  GtkApplication parent_instance;
};
struct _MyAppClass
{
  GtkApplicationClass parent_class;
};

G_DEFINE_TYPE (MyApp, my_app, GTK_TYPE_APPLICATION) 

static void
my_app_activate (GApplication *gapp)
{
  g_assert (MY_IS_APP (gapp));
  MyApp *app = MY_APP (gapp);
  MyAppWindow *win = my_app_window_new (app);
  gtk_window_set_decorated (GTK_WINDOW (win), FALSE);
  gtk_window_present (GTK_WINDOW (win));
}

static void
my_app_class_init (MyAppClass *klass)
{
  GApplicationClass *gappclass = G_APPLICATION_CLASS (klass);
  gappclass->activate = my_app_activate;
}

static void
my_app_init (MyApp *app)
{
}

MyApp *
my_app_new (void)
{
  MyApp *app;
  app = g_object_new (MY_TYPE_APP, NULL);
  return app;
}
