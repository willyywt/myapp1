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
  status = g_application_run (G_APPLICATION (my_app_new ()), argc, argv);
  return status;
}

