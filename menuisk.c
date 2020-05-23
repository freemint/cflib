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


static _WORD check_item_key(char *str, _WORD kstate, _WORD kreturn)
{
	_WORD found = FALSE;
	_WORD i, r, ks;
	short len;
	char s[50], ascii;
	unsigned char scan;

	/* do not make shift keys differences */
	if (kstate & K_RSHIFT)
		kstate |= K_LSHIFT;
	/* look only to shift/alt/ctrl modifiers */
	kstate &= K_LSHIFT|K_CTRL|K_ALT;

	scan = (kreturn >> 8) & 0xff;
	if (scan > 127)
		return FALSE;
	if (scan >= 0x78 && scan < 0x80)
		ascii = __menu_keys->capslock[scan - 0x76];
	else
		ascii = __menu_keys->capslock[scan];

	if (str[0] != '\0')
	{
		len = (short)strlen(str);
		i = len - 1;
		r = 0;
		while (str[i] == ' ')
		{
			i--;
			r++;
		}
		if (r < 2)
		{
			str[len - r] = '\0';	/* Blanks am Ende l�schen */

			while ((i >= 0) && (str[i] != ' '))
				i--;
			strcpy(s, str + i + 1);
			str_toupper(s);

			ks = 0;
			i = 0;
			found = FALSE;
			while (!found)
			{
				switch (s[i])
				{
					case '^':
						ks |= K_CTRL;
						i++;
						break;
					case '\007':
						ks |= K_ALT;
						i++;
						break;
					case '\001':
						ks |= K_LSHIFT;
						i++;
						break;
					default:
						found = TRUE;
				}
			}

			found = FALSE;
			if (ks == kstate)			/* Sondertasten m�ssen gleich sein */
			{
				if (s[i + 1] == '\0')	/* nur einzelnes Zeichen */
				{
					if (s[i] == 0x1B && scan == 1)	/*  */
						found = TRUE;
					else if (s[i] == ascii)
						found = TRUE;
				}
				else if ((strcmp(s + i, "DEL") == 0) && (scan == 83))
					found = TRUE;
				else if ((strcmp(s + i, "ESC") == 0) && (scan == 1))
					found = TRUE;
				else if ((strcmp(s + i, "HELP") == 0) && (scan == 98))
					found = TRUE;
				else if ((strcmp(s + i, "HOME") == 0) && (scan == 71))
					found = TRUE;
				else if ((strcmp(s + i, "INS") == 0) && (scan == 82))
					found = TRUE;
				else if ((strcmp(s + i, "TAB") == 0) && (scan == 15))
					found = TRUE;
				else if ((strcmp(s + i, "UNDO") == 0) && (scan == 97))
					found = TRUE;
				else if ((s[i] == 'F') && (s[i + 1] >= '1') && (s[i + 1] <= '9'))
				{
					if (s[i + 2] == '0' && scan == 68)	/* F10 */
						found = TRUE;
					else
					{
						if (ks == K_LSHIFT || ks == K_RSHIFT)
						{
							if ((scan - 25) == (s[i + 1] + 10))
								found = TRUE;
						}
						else
						{
							if (scan == (s[i + 1] + 10))
								found = TRUE;
						}
					}
				}
			}
		}
	}
	return found;
}

_WORD is_menu_key(_WORD kreturn, _WORD kstate, _WORD *title, _WORD *item)
{
	_WORD menu_box, i, t;
	_WORD the_bar;
	_WORD the_title;
	_WORD menu_accbox;
	char str[50];
	_WORD found = FALSE;
	OBJECT *tree = __menu_tree;

	if (tree != NULL && !__menu_disabled)
	{
		the_bar = tree[ROOT].ob_head;
		the_title = tree[the_bar].ob_head;
		menu_box = tree[ROOT].ob_tail;
		menu_accbox = menu_box = tree[menu_box].ob_head;
		t = tree[the_title].ob_head;										/* 3: Desktop-Titel */
		do
		{
			i = tree[menu_box].ob_head;
			do
			{
				if (((tree[i].ob_type == G_STRING) || (tree[i].ob_type == G_SHORTCUT))
				    && !(tree[i].ob_state & OS_DISABLED))
				{
					get_string(tree, i, str);
					if (str[0] != '-')
					{
						found = check_item_key(str, kstate, kreturn);
						if (found)
						{
							*title = t;
							*item = i;
							menu_tnormal(tree, *title, 0);
							return TRUE;
						}
					}
				}
				i = tree[i].ob_next;
				if (menu_accbox == menu_box)   /* stop research for acc menu only the first item should be evaluated */
				{
					i = menu_box;
				}
			} while (i != menu_box);
			menu_box = tree[menu_box].ob_next;
			t = tree[t].ob_next;
		} while (t != the_title);							/* 2: Box, die die Titel umgibt */
	}
	return FALSE;
}
