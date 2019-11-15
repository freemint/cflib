/*
 * Drag & drop sample code.
 * Copyright 1992 Atari Corporation
 *
 * global variables used:
 * gl_apid: our AES application id
 *
 * BUGS/CAVEATS:
 * This code is not re-entrant (it uses a static
 * variable for the pipe name and for saving the
 * SIGPIPE signal handler).
 *
 * While doing the drag and drop, the SIGPIPE
 * signal (write on an empty pipe) is ignored
 */

#include "intern.h"
#include "dragdrop.h"


char __dragdrop_pipename[] = "u:\\pipe\\dragdrop.aa";

__mint_sighandler_t __dragdrop_oldpipesig;

#ifdef __PUREC__
#include "ddclose.c"
#include "ddcreate.c"
#include "ddopen.c"
#include "ddreply.c"
#include "ddrtry.c"
#include "ddstry.c"
#endif
