/*  myappwindow.c

    This file is part of myapp1.
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#include "private.h"

struct _MyAppWindow
{
  GtkWindow parent_instance;
};
struct _MyAppWindowClass
{
  GtkWindowClass parent_class;
};

G_DEFINE_TYPE (MyAppWindow, my_app_window, GTK_TYPE_WINDOW) 

static void
my_app_window_class_init (MyAppWindowClass *klass)
{
}

static void
my_app_window_init (MyAppWindow *app_window)
{
}

MyAppWindow *
my_app_window_new (MyApp *app)
{
  MyAppWindow *app_window;
  app_window = g_object_new (GTK_TYPE_WINDOW,
                             "application", G_APPLICATION (app),
                             NULL);
  return app_window;
}
