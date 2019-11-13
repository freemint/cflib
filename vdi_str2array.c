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


#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)

_WORD vdi_str2array(const char *src, _WORD *des)
{
	_WORD len = 0;
	const unsigned char *c = (const unsigned char *) src;

	while (*c != '\0')
	{
		*des++ = *c++;
		len++;
	}

	return len;
}

#endif
