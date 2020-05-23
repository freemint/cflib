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
 * Ein eigener form_do(), der einige Sondertastentasten kann:
 * 
 * HELP     Button mit Flag11
 * UNDO     Button mit Flag12
 * ^C^V^X   Klemmbrett in Editfeldern
 * ALT-X    MagiC-Shortcut
 *
 */

#include "intern.h"


_WORD cf_form_do(OBJECT *tree, _WORD *ed_start)
{
	_WORD edit_obj, cont, idx, next_obj;
	_WORD m_x, m_y, m_button, breturn, kstate, kreturn, which, msg[8];
	_WORD b, doppel = FALSE;

	wind_update(BEG_MCTRL);
	if ((ed_start == NULL) || (*ed_start == 0) || !edit_valid(tree, *ed_start))
		next_obj = find_edit(tree, 0, FMD_FORWARD);
	else
		next_obj = *ed_start;
	edit_obj = 0;
	cont = TRUE;
	while (cont)
	{
		if (next_obj && (edit_obj != next_obj))
		{
			edit_obj = next_obj;
			next_obj = 0;
			objc_edit(tree, edit_obj, 0, &idx, ED_INIT);
		}
		which = evnt_multi(MU_KEYBD | MU_BUTTON,
				2, 1, 1,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				msg,
#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)
				0, 0,
#else
				0,
#endif
				&m_x, &m_y, &m_button, &kstate, &kreturn, &breturn);

		if (which & MU_KEYBD)
		{
			cont = cf_form_keybd(tree, edit_obj, kstate, &kreturn, &next_obj);
			if (kreturn)
				cf_objc_edit(tree, edit_obj, kreturn, &idx, ED_CHAR, kstate, &b);
		}
		if (which & MU_BUTTON)
		{
			next_obj = cf_objc_find(tree, ROOT, MAX_DEPTH, m_x, m_y);
			if (next_obj == -1)
			{
				Bconout(2, 7);		/* beep */
				next_obj = 0;
			}
			else
			{
				doppel = (breturn == 2);
				cont = form_button(tree, next_obj, breturn, &next_obj);
			}
		}
		if (!cont || (next_obj && (next_obj != edit_obj)))
			objc_edit(tree, edit_obj, 0, &idx, ED_END);
	}

	if (ed_start != NULL)
		*ed_start = edit_obj;
	wind_update(END_MCTRL);

	if (doppel)
		next_obj |= 0x8000;			/* bit 15 fuer Doppelklick */

	return next_obj;
}
