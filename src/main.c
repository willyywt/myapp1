/*  main.c

    This file is part of myproject.
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#include "private.h"

int
main (int argc, char *argv[])
{
  int status;
  MyApp *app;
  GApplication *gapp;
  app = my_app_new();
  GOptionEntry entries[] = {
    {"theme-dir", 't', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING, NULL, "Theme directory", NULL}
  };
  gapp = G_APPLICATION (app);

  g_application_add_main_option_entries (gapp, entries);
  status = g_application_run (gapp, argc, argv);
  return status;
}

