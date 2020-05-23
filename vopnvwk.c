/*
 * CFLIB, a GEM library for ATARI/TOS
 * Copyright (C) 1999, 2000 Christian Felsch
 * 
 * Modified for FreeMiNT CVS by Frank Naumann <fnaumann@freemint.de>
 * 
 * Please send suggestions, patches or bug reports to me or
 * the MiNT mailing list.
 * 
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 * 
 */
/*
 * vdi.c
 * Hilfs-Routinen rund um das VDI.
 */

#include "intern.h"


_WORD open_vwork(_WORD *w_out)
{
	_WORD handle;
	_WORD work_in[11];
	_WORD i;
	
	for (i = 0; i < 10; i++)
		work_in[i] = 1;
	work_in[10] = 2;
	handle = gl_phys_handle;
	v_opnvwk(work_in, &handle, w_out);

	return handle;
}
