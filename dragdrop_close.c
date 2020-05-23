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

#include "dragdrop.h"


/*
 * ddclose: close a drag & drop operation
 */
void dd_close(int fd)
{
	Psignal(SIGPIPE, __dragdrop_oldpipesig);
	(void)Fclose(fd);
}
