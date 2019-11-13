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
#include "app.h"
#include "inline.h"

static _WORD last_ascii;
static _WORD x_off, y_off;

#define X_FAC (gl_wchar * 2)
#define Y_FAC (gl_hchar + 2)

static void set_numbers(_WORD code)
{
	char str[16];

	sprintf(str, "%03d", code);
	set_string(cf_ascii_tab, AT_DEZ, str);
	redraw_obj(cf_ascii_tab, AT_DEZ);

	sprintf(str, "0x%02X", code);
	set_string(cf_ascii_tab, AT_HEX, str);
	redraw_obj(cf_ascii_tab, AT_HEX);
}

static void draw_ascii(_WORD id, _WORD pts)
{
	_WORD wc, hc, wb, hb, d, i, x, y, x0;
	GRECT r;
	char c[2] = "x";

	vst_font(cf_vdi_handle, id);
	vst_point(cf_vdi_handle, pts, &wc, &hc, &wb, &hb);
	vst_alignment(cf_vdi_handle, TA_LEFT, TA_TOP, &d, &d);
	vswr_mode(cf_vdi_handle, MD_TRANS);
	vst_color(cf_vdi_handle, G_BLACK);

	objc_offset(cf_ascii_tab, AT_BOX, &r.g_x, &r.g_y);
	r.g_w = cf_ascii_tab[AT_BOX].ob_width;
	r.g_h = cf_ascii_tab[AT_BOX].ob_height;
	x_off = (cf_ascii_tab[AT_BOX].ob_width - X_FAC * 16) / 2 + 1;
	y_off = (cf_ascii_tab[AT_BOX].ob_height - Y_FAC * 16) / 2 + 1;
	set_clipping(cf_vdi_handle, r.g_x, r.g_y, r.g_w, r.g_h, TRUE);
	x0 = r.g_x + x_off + (X_FAC - wb) / 2;
	x = x0;
	y = r.g_y + y_off;
	for (i = 0; i < 256; i++)
	{
		c[0] = i;
		v_gtext(cf_vdi_handle, x, y, c);
		x += X_FAC;
		if (((i + 1) % 16) == 0)
		{
			y += Y_FAC;
			x = x0;
		}
	}
	set_clipping(cf_vdi_handle, r.g_x, r.g_y, r.g_w, r.g_h, FALSE);
}

static _WORD get_ascii(_WORD x, _WORD y)
{
	_WORD x1, y1;

	objc_offset(cf_ascii_tab, AT_BOX, &x1, &y1);
	x -= x1 + x_off;
	y -= y1 + y_off;
	if (x < 0 || y < 0)
		return -1;

	x /= X_FAC;
	y /= Y_FAC;
	if (x < 16 && y < 16)
		return x + y * 16;
	return -1;
}

static void select_ascii(_WORD obj)
{
	_WORD pxy[4], x, y;

	objc_offset(cf_ascii_tab, AT_BOX, &x, &y);
	pxy[0] = x + (obj % 16) * X_FAC + x_off;
	pxy[1] = y + (obj / 16) * Y_FAC + y_off;
	pxy[2] = pxy[0] + X_FAC - 1;
	pxy[3] = pxy[1] + Y_FAC - 1;

	graf_mouse(M_OFF, NULL);
	vsf_color(cf_vdi_handle, G_BLACK);
	vsf_interior(cf_vdi_handle, FIS_SOLID);
	vswr_mode(cf_vdi_handle, MD_XOR);
	v_bar(cf_vdi_handle, pxy);
	graf_mouse(M_ON, NULL);
}

static void jump_to(_WORD new)
{
	if (last_ascii >= 0)
		select_ascii(last_ascii);
	select_ascii(new);
	last_ascii = new;
	set_numbers(new);
}

_WORD ascii_table(_WORD id, _WORD pts)
{
	_WORD event, mx, my, kstate, kreturn, breturn, bclicks;
	_WORD d, obj, code = -1;
	MFDB background;
	GRECT r;
	int quit;

	set_string(cf_ascii_tab, AT_DEZ, "000");
	set_string(cf_ascii_tab, AT_HEX, "0x00");

	wind_update(BEG_UPDATE);
	wind_update(BEG_MCTRL);
	form_center_grect(cf_ascii_tab, &r);
	save_background(&r, &background);
	form_dial_grect(FMD_START, &r, &r);
	graf_mouse(M_OFF, NULL);
	objc_draw_grect(cf_ascii_tab, ROOT, MAX_DEPTH, &r);
	draw_ascii(id, pts);
	graf_mouse(M_ON, NULL);

	last_ascii = -1;
	quit = FALSE;
	graf_mkstate(&mx, &my, &d, &d);
	do
	{
		event = evnt_multi(MU_BUTTON | MU_KEYBD | MU_M1,
				1, 1, 1,
				1, mx, my, 1, 1,
				0, 0, 0, 0, 0,
				NULL,
#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)
				0, 0,
#else
				0,
#endif
				&mx, &my, &breturn, &kstate, &kreturn, &bclicks);
		if (event & MU_M1)
		{
			obj = get_ascii(mx, my);
			if (obj >= 0 && obj != last_ascii)
				jump_to(obj);
		}
		if (event & MU_BUTTON)
		{
			obj = get_ascii(mx, my);
			if (obj >= 0)
			{
				code = obj;
				quit = TRUE;
			}
			else
			{
				if (objc_find(cf_ascii_tab, ROOT, MAX_DEPTH, mx, my) == AT_ABBRUCH)
				{
					code = -1;
					quit = TRUE;
				}
				else
					Bconout(2, 7);
			}
		}
		if (event & MU_KEYBD)
		{
			_WORD scan;

			scan = kreturn >> 8;
			switch (scan)
			{
			case 0x61:	/* UNDO */
				code = -1;
				quit = TRUE;
				break;

			case 0x1C:	/* Return */
			case 0x72:	/* Enter */
				code = last_ascii;
				quit = TRUE;
				break;

			case 0x47:	/* HOME */
				if (kstate & K_SHIFT)
					jump_to(255);
				else
					jump_to(0);
				break;

			case 0x4F:	/* MagiCMac: anti-HOME */
				jump_to(255);
				break;

			case 0x4B:	/* Left */
				if (last_ascii < 0)
					jump_to(0);
				else
				{
					if (kstate & K_SHIFT)
					{
						d = last_ascii % 16;
						jump_to(last_ascii - d);
					}
					else if (last_ascii > 0)
						jump_to(last_ascii - 1);
				}
				break;

			case 0x4D:	/* Right */
				if (last_ascii < 0)
					jump_to(0);
				else
				{
					if (kstate & K_SHIFT)
					{
						d = last_ascii % 16;
						jump_to(last_ascii + (15 - d));
					}
					else if (last_ascii < 255)
						jump_to(last_ascii + 1);
				}
				break;

			case 0x48:	/* Up */
				if (last_ascii < 0)
					jump_to(0);
				else if (last_ascii > 15)
					jump_to(last_ascii - 16);
				break;
			case 0x50:	/* Down */
				if (last_ascii < 0)
					jump_to(0);
				else if (last_ascii < 240)
					jump_to(last_ascii + 16);
				break;
			}
		}
	} while (!quit);

	form_dial_grect(FMD_FINISH, &r, &r);
	restore_background(&r, &background);
	wind_update(END_MCTRL);
	wind_update(END_UPDATE);

	return code;
}


#ifdef __PUREC__
#include "ascii_set_asciitable_strings.c"
#endif
