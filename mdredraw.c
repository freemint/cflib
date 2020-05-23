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

#include "mdial.h"


/* FIXME: identical to draw_wdial? */
void redraw_mdial(MDIAL *dial, _WORD start, _WORD depth, _WORD x, _WORD y, _WORD w, _WORD h)
{
	GRECT r, r1;

	r.g_x = x;
	r.g_y = y;
	r.g_w = w;
	r.g_h = h;
	wind_update(BEG_UPDATE);
	hide_mouse_if_needed(&r);
	wind_get_grect(dial->win_handle, WF_FIRSTXYWH, &r1);
	while (r1.g_w != 0 && r1.g_h != 0)
	{
		if (rc_intersect(&r, &r1))
			objc_draw_grect(dial->tree, start, depth, &r1);
		wind_get_grect(dial->win_handle, WF_NEXTXYWH, &r1);
	}
	show_mouse();
	wind_update(END_UPDATE);
}
