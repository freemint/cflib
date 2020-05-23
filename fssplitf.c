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
#include "file.h"


void split_filename(char *fullname, char *path, char *name)
{
	char *str;
	char *str2;

	str = strrchr(fullname, '\\');
	str2 = strrchr(fullname, '/');
	/* check for mixed name */
	if (str == NULL || str2 > str)
		str = str2;

	if (path != NULL)
		path[0] = '\0';		/* schadet nix */
	if (name != NULL)
		name[0] = '\0';		/* ditto */

	if (str != NULL)
	{
		/* Dateinamen holen */
		if (name != NULL)
			strcpy(name, str + 1);

		/* Pfad mit Laufwerk bestimmen */
		if (path != NULL)
		{
			int len = (short)(str - (char *)fullname + 1);

			strncpy(path, fullname, len);
			path[len] = '\0';
		}
	}
}
