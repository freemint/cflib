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
 * Verwaltung die Men�leiste der Applikation.
 * ACHTUNG: Nicht re-entrant, d.h. jede Applikation nur ein Kreuz, �h Men�!
 */

#include "menu.h"


_WORD create_menu(OBJECT *t)
{
	if (__menu_keys == NULL)
		__menu_keys = (KEYTAB *)Keytbl((void *)-1, (void *)-1, (void *)-1);

	if (__menu_tree == NULL)
	{
		__menu_tree = t;
		__menu_disabled = FALSE;
		__menu_dis_cnt = 0;

		fix_menu(t);
		menu_bar(t, 1);
		return TRUE;
	}
	else
		form_alert(1, "[3][CF-Lib Panic: create_menu()|Kein zweites Men� m�glich!][Abbruch]");
	return FALSE;
}
