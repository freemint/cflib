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

#include "wdial.h"


/*
 * Einzelne Objekte neu zeichnen.
 */
void redraw_wdobj(WDIALOG *wd, _WORD obj)
{
	GRECT r;

	if (wd != NULL && (wd->mode & WD_OPEN) != 0)
	{
		get_objframe(wd->tree, obj, &r);
		draw_wdial(wd, ROOT, MAX_DEPTH, r.g_x, r.g_y, r.g_w, r.g_h);

		/* neu zeichnen, wenn obj das Editfeld bzw. ein Child von obj ist */
		/* FIXME: logical order is not neccessarily physical order */
		/* FIXME: should not draw cursor when iconified */
		if ((obj == wd->edit_obj) || (wd->edit_obj >= wd->tree[obj].ob_head && wd->edit_obj <= wd->tree[obj].ob_tail))
			wdial_draw_cursor(wd, ED_INIT);
	}
}
