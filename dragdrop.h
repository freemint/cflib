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

#ifndef _dragdrop_h
#define _dragdrop_h

#include "intern.h"

extern char __dragdrop_pipename[];

#include <errno.h>

/*
 * we need the MiNT definitions for those, not any library definitions
 */
#ifndef __mint_sighandler_t_defined
#define __mint_sighandler_t_defined 1
typedef void __CDECL (*__mint_sighandler_t) (long signum);
#endif

extern __mint_sighandler_t __dragdrop_oldpipesig;

#undef SIG_IGN
#define SIG_IGN ((__mint_sighandler_t) 1L)
#undef SIGPIPE
#define SIGPIPE 13

#undef EACCES
#undef EPERM
#define EACCES -36
#define EPERM  -38


#endif /* _dragdrop_h */
