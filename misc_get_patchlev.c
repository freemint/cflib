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

#include "intern.h"
#include <ctype.h>


int get_patchlev(const char *id_str, char *pl)
{
	const char *p;
	int len;
	int ret = FALSE;

	len = (int)strlen(id_str);
	if (id_str[0] == '$' && id_str[len - 1] == '$')
	{
		p = id_str;
		while (!isdigit(*p))
			p++;
		while (*p != '$' && !isspace(*p))
			*pl++ = *p++;
		*pl = '\0';
		ret = TRUE;
	}

	return ret;
}
