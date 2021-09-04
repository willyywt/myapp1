/*  myapp.h

    This file is part of myapp1.
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

 */

#ifndef myapp_H_INCLUDED
#define myapp_H_INCLUDED

#ifndef __myapp1_INSIDE
#error "Only <myapp1.h> can be included directly. "
/* Be nice to clangd which parses every header file separately, unlike actual compilers */
#include "myapp1.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MY_TYPE_APP my_app_get_type ()
G_DECLARE_FINAL_TYPE (MyApp, my_app, MY, APP, GtkApplication)

MyApp * my_app_new (void);

#ifdef __cplusplus
}
#endif
#endif /* myapp_H_INCLUDED */
