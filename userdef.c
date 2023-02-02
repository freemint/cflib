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
 * userdef.c
 *
 * Ist MagiC nicht vorhanden, werden die neuen Objekttypen von MagiC
 * emuliert.
 * Emuliert werden Radio- und Kreuzknopf, Gruppenrahmen, unterstrichener
 * String sowie die Shortcuts.
 * ZusÑtzlich zu den MagiC-Objekten gibt es noch den Gruppenrahmen und
 * öberschrift in kleiner Schrift (Flags CHECKED).
 *
 */

#include "intern.h"

#ifndef TXT_LIGHT
#define TXT_LIGHT		0x0002
#define TXT_UNDERLINED	0x0008
#endif

#include "inline.rsh"
#include "inline.h"

OBJECT *cf_ascii_tab = NULL;
OBJECT *cf_alert_box = NULL;

static _WORD handle = -1, ud_wchar, ud_hchar, ud_wbox, ud_hbox;
static USERBLK menu_blk, popup_blk;
static _WORD gnva, use_3D, mx_buttons, mx_shortcut;
static _WORD back_col;
static CICONBLK *radio_cib, *check_cib;
static _WORD mask_col[2] = { 0, 0 };
static _WORD icon_col[2] = { 1, 0 };
static _WORD use_all_userdefs = FALSE;

/* -- diverser Kleinkram ----------------------------------------------------- */

static _WORD get_txtlen(char *str)
{
	_WORD pxy[8];

	if (gl_nvdi >= 0x300)
		udef_vqt_real_extent(handle, 0, 0, str, pxy);
	else
		udef_vqt_extent(handle, str, pxy);

	return pxy[2] - pxy[0];
}


/* -- allgemeine Zeichenroutinen --------------------------------------------- */

static void draw_string(OBJECT *tree, _WORD obj, _WORD x, _WORD y, char *str, _WORD effect)
{
	_WORD l, pos;
	char c;

	if (tree[obj].ob_state & OS_DISABLED)
		effect |= TXT_LIGHT;

	udef_vst_effects(handle, effect);

	udef_v_gtext(handle, x, y, str);

	/* Gibts einen Shortcut? */
	pos = get_magx_shortcut(tree, obj, &c);
	if (pos != -1)
	{
		char chr[2] = "A";
		char s2[128];

		/* Start-Pos */
		strcpy(s2, str);
		s2[pos] = '\0';
		l = get_txtlen(s2);

		udef_vswr_mode(handle, MD_TRANS);
		udef_vst_effects(handle, effect | TXT_UNDERLINED);
		chr[0] = c;
		udef_v_gtext(handle, x + l, y, chr);
	}
}


static void draw_bitblk(short *data, _WORD planes, _WORD mode, _WORD *col, _WORD x, _WORD y)
{
	MFDB dst, src;
	_WORD pxy[8];

	src.fd_addr = data;
	src.fd_w = radio_cib->monoblk.ib_wicon;
	src.fd_h = radio_cib->monoblk.ib_hicon;
	src.fd_wdwidth = src.fd_w / 16;
	src.fd_stand = 0;
	src.fd_nplanes = planes;

	dst.fd_addr = NULL;

	pxy[0] = 0;
	pxy[1] = 0;
	pxy[2] = src.fd_w - 1;
	pxy[3] = src.fd_h - 1;
	pxy[4] = x;
	pxy[5] = y;
	pxy[6] = pxy[4] + pxy[2];
	pxy[7] = pxy[5] + pxy[3];

	if (planes == 1)
		udef_vrt_cpyfm(handle, mode, pxy, &src, &dst, col);
	else
		udef_vro_cpyfm(handle, mode, pxy, &src, &dst);
}


/* -- Zeichenroutinen fÅr die Objekte ---------------------------------------- */


static _WORD draw_ciconblk(PARMBLK *p, CICONBLK *cib)
{
	char *str;
	_WORD wchar, d, i_m;
	short *m_d, *i_d;

	set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);
	udef_vswr_mode(handle, MD_TRANS);

	if (p->pb_currstate & OS_SELECTED)
	{
		i_d = (short *)cib->mainlist->sel_data;
		m_d = (short *)cib->mainlist->sel_mask;
	}
	else
	{
		i_d = (short *)cib->mainlist->col_data;
		m_d = (short *)cib->mainlist->col_mask;
	}

	/* Maske */
	draw_bitblk(m_d, 1, MD_TRANS, mask_col, p->pb_x, p->pb_y);

	/* Icon */
	if (cib->mainlist->num_planes > 1)
	{
		if (cib->mainlist->num_planes > 8)
			i_m = S_AND_D;
		else
			i_m = S_OR_D;
	}
	else
		i_m = MD_TRANS;

	draw_bitblk(i_d, cib->mainlist->num_planes, i_m, icon_col, p->pb_x, p->pb_y);

	/* Text nur beim objc_draw ausgeben */
	if (p->pb_prevstate == p->pb_currstate)
	{
		str = (char *)p->pb_parm;
		udef_vst_font(handle, sys_big_id);
		udef_vst_height(handle, sys_big_height, &wchar, &d, &d, &d);
		draw_string(p->pb_tree, p->pb_obj, p->pb_x + p->pb_h + wchar, p->pb_y, str, 0);
	}

	return p->pb_currstate & ~(OS_SELECTED | OS_DISABLED);
}


static _WORD __CDECL draw_radiobutton(PARMBLK *p)
{
	return draw_ciconblk(p, radio_cib);
}


static _WORD __CDECL draw_checkbutton(PARMBLK *p)
{
	return draw_ciconblk(p, check_cib);
}


static _WORD __CDECL draw_underline(PARMBLK *p)
{
	char *str;
	_WORD d, len, pxy[8], wBox, hBox;

	set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);
	udef_vswr_mode(handle, MD_TRANS);

	/* Font */
	udef_vst_font(handle, sys_big_id);
	udef_vst_height(handle, sys_big_height, &d, &d, &wBox, &hBox);

	/* Text ausgeben */
	str = (char *)p->pb_parm;
	draw_string(p->pb_tree, p->pb_obj, p->pb_x, p->pb_y, str, 0);

	/* Linie */
	len = p->pb_w;

	if (use_3D)
		udef_vsl_color(handle, G_LBLACK);
	else
		udef_vsl_color(handle, G_BLACK);

	pxy[0] = p->pb_x;
	pxy[1] = p->pb_y + hBox;
	pxy[2] = p->pb_x + len;
	pxy[3] = pxy[1];
	udef_v_pline(handle, 2, pxy);

	if (use_3D)
	{
		udef_vsl_color(handle, G_WHITE);
		pxy[1]--;
		pxy[3]--;
		udef_v_pline(handle, 2, pxy);
	}

	return p->pb_currstate & ~(OS_CHECKED | OS_DISABLED);
}


static _WORD __CDECL draw_groupbox(PARMBLK *p)
{
	_WORD pxy[12], len, wBox, hBox, d;
	_WORD x, y, w, h;
	char *str;

	set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);

	/* Font */
	if (p->pb_tree[p->pb_obj].ob_state & OS_CHECKED)		/* kleine Schrift */
	{
		udef_vst_font(handle, sys_sml_id);
		udef_vst_height(handle, sys_sml_height, &d, &d, &wBox, &hBox);
	}
	else
	{
		udef_vst_font(handle, sys_big_id);
		udef_vst_height(handle, sys_big_height, &d, &d, &wBox, &hBox);
	}

	/* Text */
	udef_vswr_mode(handle, MD_TRANS);
	str = (char *)p->pb_parm;

	x = p->pb_x + wBox;
	y = p->pb_y + (sys_hbox - hBox) / 2 + 1;
	draw_string(p->pb_tree, p->pb_obj, x, y, str, 0);
	len = get_txtlen(str);

	/* Rahmen */
	x = p->pb_x;
	y = p->pb_y + sys_hbox / 2;
	if (use_3D)
	{
		/* weiûer Rahmen wird auûen gemalt -> 1 Pixel schmaler */
		w = p->pb_w - 1;
		h = p->pb_h - 1 - sys_hbox / 2;
	}
	else
	{
		w = p->pb_w;
		h = p->pb_h - sys_hbox / 2;
	}

	/* Frame */
	udef_vswr_mode(handle, MD_REPLACE);
	if (use_3D)
		udef_vsl_color(handle, G_LBLACK);
	else
		udef_vsl_color(handle, G_BLACK);

	pxy[0] = x + wBox;
	pxy[1] = y;
	pxy[2] = x;
	pxy[3] = y;
	pxy[4] = x;
	pxy[5] = y + h - 1;
	pxy[6] = x + w - 1;
	pxy[7] = pxy[5];
	pxy[8] = pxy[6];
	pxy[9] = y;
	pxy[10] = x + wBox + len;
	pxy[11] = y;
	udef_v_pline(handle, 6, pxy);

	if (use_3D)
	{
		udef_vsl_color(handle, G_WHITE);
		pxy[1]++;
		pxy[2]++;
		pxy[3]++;
		pxy[4]++;
		pxy[5]--;
		udef_v_pline(handle, 3, pxy);

		pxy[0] = x;
		pxy[1] = y + h;
		pxy[2] = x + w;
		pxy[3] = pxy[1];
		pxy[4] = pxy[2];
		pxy[5] = y;
		udef_v_pline(handle, 3, pxy);

		pxy[0] = x + w - 2;
		pxy[1] = y + 1;
		pxy[2] = pxy[10];
		pxy[3] = pxy[1];
		udef_v_pline(handle, 2, pxy);
	}

	return 0;
}


static _WORD __CDECL draw_scstring(PARMBLK *p)
{
	char *str;
	_WORD d;

	set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);
	udef_vswr_mode(handle, MD_TRANS);

	udef_vst_font(handle, sys_big_id);
	udef_vst_height(handle, sys_big_height, &d, &d, &d, &d);

	/* Text ausgeben */
	str = (char *)p->pb_parm;
	draw_string(p->pb_tree, p->pb_obj, p->pb_x, p->pb_y, str, 0);

	return 0;
}


/*
 * MenÅ-Tuning: Trennlinien als Linie (fÅr HauptmenÅs)
 * (ST-Computer 3/92, Seite 87)
 */
static _WORD __CDECL draw_menuline(PARMBLK *p)
{
	_WORD pxy[4];

	/* Unter TOS und N.AES 1.x gibt es fÅr das HauptmenÅ falsche Clippingdaten.
	 * Unter Geneva (Tear-Away-MenÅs) sowie MagiC stimmen sie
	 */
	if (gnva || gl_magx)
		set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);
	else
		udef_vs_clip(handle, 0, pxy);

	pxy[0] = p->pb_x;
	pxy[1] = p->pb_y + (p->pb_h / 2) - 1;
	pxy[2] = p->pb_x + p->pb_w - 1;
	pxy[3] = p->pb_y + (p->pb_h / 2);

	if (use_3D)
	{
		udef_vsf_interior(handle, FIS_SOLID);
		udef_vsf_color(handle, G_LBLACK);
	}
	else
	{
		udef_vsf_interior(handle, FIS_PATTERN);
		udef_vsf_style(handle, 4);
		udef_vsf_color(handle, G_BLACK);
	}
	udef_vr_recfl(handle, pxy);

	return 0;
}

/*
 * DÅnne Trennlinien fÅr PopupmenÅs.
 * Von Joachim Fornallaz <jfornall@stud.ee.ethz.ch>
 */
static _WORD __CDECL draw_popupline(PARMBLK *p)
{
	_WORD pxy[4];

	set_clipping(handle, p->pb_xc, p->pb_yc, p->pb_wc, p->pb_hc, TRUE);

	pxy[0] = p->pb_x;
	pxy[1] = p->pb_y + (p->pb_h / 2) - 1;
	pxy[2] = p->pb_x + p->pb_w - 1;
	pxy[3] = pxy[1];	/* p->pb_y + (p->pb_h / 2); */

	if (use_3D)
	{
		udef_vsl_color(handle, G_LBLACK);
		udef_v_pline(handle, 2, pxy);

		pxy[1] = p->pb_y + (p->pb_h / 2);
		pxy[3] = pxy[1];

		udef_vsl_color(handle, G_WHITE);
		udef_v_pline(handle, 2, pxy);
	}
	else
	{
		udef_vsf_interior(handle, FIS_PATTERN);
		udef_vsf_style(handle, 4);
		udef_vsf_color(handle, G_BLACK);
		udef_vr_recfl(handle, pxy);
	}

	return 0;
}

/* -- sonstige Hilfsfunktionen ----------------------------------------------- */

static void init_inline_rsc(void)
{
	_WORD i, obj;

	for (i = 0; i < NUM_OBS; i++)
		rsrc_obfix(&rs_object[i], 0);

	cf_ascii_tab = rs_trindex[ASCIITAB];
	fix_dial(cf_ascii_tab);

	cf_alert_box = rs_trindex[ALERTBOX];
	fix_dial(cf_alert_box);

	if (use_3D)
		i = gl_planes;
	else
		i = 1;		/* keine Farbicons unter TOS */

	obj = (gl_hchar > 8) ? BT_RADIO : BT_RLOW;
	radio_cib = (CICONBLK *)get_obspec(rs_trindex[BUTTONS], obj);
	radio_cib->mainlist = fix_cicon(radio_cib, i, handle);

	obj = (gl_hchar > 8) ? BT_CHECK : BT_CLOW;
	check_cib = (CICONBLK *)get_obspec(rs_trindex[BUTTONS], obj);
	check_cib->mainlist = fix_cicon(check_cib, i, handle);
}

static void make_userdef(USERBLK *uPtr, OBJECT *tree, _WORD obj, _WORD __CDECL (*proc)(PARMBLK *p))
{
	uPtr->ub_code = proc;						/* neue Zeichenfunktion */
	uPtr->ub_parm = tree[obj].ob_spec.index;	/* alte obSpec sichern */

	/* alten Typ hochschieben und neuen eintragen */
	tree[obj].ob_type = (tree[obj].ob_type << 8) | G_USERDEF;
	tree[obj].ob_spec.userblk = uPtr;			/* Userblock eintragen */
}

/* -- exportierte Funtkionen ------------------------------------------------- */

void fix_dial(OBJECT *tree)
{
	_WORD mtyp, obj;
	USERBLK *uPtr;
	_WORD count;

	if (!tree)
		return;

	/* first pass: count number of USERBLKs needed */
	obj = -1;
	count = 0;
	do
	{
		obj++;
		mtyp = get_magx_obj(tree, obj);

		/* Ein paar Erweiterungen, die MagiC auch nicht kann */
		switch (mtyp)
		{
			case MX_GROUPBOX2:
				count++;
				break;
		}

		/* den Rest nur, wenn keine WHITEBACK-Buttons verfÅgbar sind */
		if (!mx_buttons || use_all_userdefs)
		{
			switch (mtyp)
			{
				case MX_UNDERLINE:
					count++;
					break;
				case MX_GROUPBOX:
					count++;
					break;
				case MX_RADIO:
				case MX_SCRADIO:
					count++;
					break;
				case MX_CHECK:
				case MX_SCCHECK:
					count++;
					break;
				case MX_SCSTRING:
					count++;
					break;
			}
		}
	} while (!(tree[obj].ob_flags & OF_LASTOB));

	if (count == 0)
		return;

	uPtr = (USERBLK *)cf_malloc(count * sizeof(USERBLK), "fix_dial", FALSE); /* leaked */

	/* 2nd pass: construct USERBLKs */
	obj = -1;
	do
	{
		obj++;
		mtyp = get_magx_obj(tree, obj);

		/* Ein paar Erweiterungen, die MagiC auch nicht kann */
		switch (mtyp)
		{
			case MX_GROUPBOX2:
				make_userdef(uPtr, tree, obj, draw_groupbox);
				uPtr++;
				break;
		}

		/* den Rest nur, wenn keine WHITEBACK-Buttons verfÅgbar sind */
		if (!mx_buttons || use_all_userdefs)
		{
			switch (mtyp)
			{
				case MX_UNDERLINE:
					make_userdef(uPtr, tree, obj, draw_underline);
					uPtr++;
					break;
				case MX_GROUPBOX:
					make_userdef(uPtr, tree, obj, draw_groupbox);
					uPtr++;
					break;
				case MX_RADIO:
				case MX_SCRADIO:
					make_userdef(uPtr, tree, obj, draw_radiobutton);
					uPtr++;
					break;
				case MX_CHECK:
				case MX_SCCHECK:
					make_userdef(uPtr, tree, obj, draw_checkbutton);
					uPtr++;
					break;
				case MX_SCSTRING:
					make_userdef(uPtr, tree, obj, draw_scstring);
					uPtr++;
					break;
			}
		}
	} while (!(tree[obj].ob_flags & OF_LASTOB));
}


void fix_menu(OBJECT *tree)
{
	_WORD i = -1;

	do
	{
		i++;
		if (tree[i].ob_type == G_STRING)
		{
			if ((tree[i].ob_state & OS_DISABLED) && (tree[i].ob_spec.free_string[0] == '-'))
			{
				tree[i].ob_type = (tree[i].ob_type << 8) | G_USERDEF;
				tree[i].ob_spec.userblk = &menu_blk;
			}
		}
	} while (!(tree[i].ob_flags & OF_LASTOB));
}


void fix_popup(OBJECT *tree, _WORD thin_line)
{
	_WORD i = -1;

	do
	{
		i++;
		if (tree[i].ob_type == G_STRING)
		{
			if (mx_shortcut)
				tree[i].ob_type = G_SHORTCUT;

			if ((tree[i].ob_state & OS_DISABLED) && (tree[i].ob_spec.free_string[0] == '-'))
			{
				tree[i].ob_type = (tree[i].ob_type << 8) | G_USERDEF;
				if (thin_line)
					tree[i].ob_spec.userblk = &popup_blk;
				else
					tree[i].ob_spec.userblk = &menu_blk;
			}
		}
	} while (!(tree[i].ob_flags & OF_LASTOB));
}


void init_userdef(void)
{
	_WORD work_out[57], d, ap1, ap2, ap3, ap4;

	handle = open_vwork(work_out);

	if (appl_xgetinfo(13, &ap1, &ap2, &ap3, &ap4))
	{
		/* 3D aktiv? und Hintergrundfarbe ermitteln */
		use_3D = ((work_out[13] >= 16) && ap1 == 1);
		if (use_3D)
			objc_sysvar(0, 5, 0, 0, &back_col, &d);

		mx_buttons = (ap4 & 0x4) != 0;		/* WHITEBAK steuert Unterstriche */
		mx_shortcut = (ap4 & 0x8) != 0;		/* type G_SHORTCUT vorhanden? */
	}
	else
	{
		use_3D = FALSE;
		back_col = 0;

		mx_buttons = FALSE;
		mx_shortcut = FALSE;
	}

#if 0
	debug("init_userdef:\n");
	debug("  use_3D:      %d\n", use_3D);
	debug("  back_col:    %d\n", back_col);
	debug("  mx_buttons:  %d\n", mx_buttons);
	debug("  mx_shortcut: %d\n", mx_shortcut);
#endif

	if (gl_gdos)
	{
		vst_load_fonts(handle, 0);
		vst_font(handle, sys_big_id);
		vst_height(handle, sys_big_height, &ud_wchar, &ud_hchar, &ud_wbox, &ud_hbox);
	}
	else
	{
		ud_wchar = gl_wchar;
		ud_hchar = gl_hchar;
	}
	vst_alignment(handle, TA_LEFT, TA_TOP, &d, &d);

	init_inline_rsc();

	menu_blk.ub_parm = 0;
	menu_blk.ub_code = draw_menuline;
	gnva = getcookie("Gnva", NULL);

	popup_blk.ub_parm = 0;
	popup_blk.ub_code = draw_popupline;
}


void exit_userdef(void)
{
	if (handle != -1)
	{
		if (gl_gdos)
			vst_unload_fonts(handle, 0);
		v_clsvwk(handle);
	}
}

/*
 * Nur fÅr internen Gebrauch: bei TRUE werden alle Objekte auch unter
 * MagiC in USERDEFs umgewandelt.
 */
void cf_use_all_userdefs(_WORD all)
{
	use_all_userdefs = all;
}
