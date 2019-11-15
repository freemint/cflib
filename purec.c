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


#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)

_WORD gl_apid;

#include <wdlgpdlg.h>
#include <wdlgfslx.h>

/*
 * Kennt die Pure/GEM-Lib nicht.
 */
_WORD rc_intersect(const GRECT *r1, GRECT *r2)
{
	_WORD tx, ty, tw, th, ret;

	tx = max(r2->g_x, r1->g_x);
	tw = min(r2->g_x + r2->g_w, r1->g_x + r1->g_w) - tx;
	ret = tw > 0;
	if (ret)
	{
		ty = max(r2->g_y, r1->g_y);
		th = min(r2->g_y + r2->g_h, r1->g_y + r1->g_h) - ty;
		ret = th > 0;
		if (ret)
		{
			r2->g_x = tx;
			r2->g_y = ty;
			r2->g_w = tw;
			r2->g_h = th;
		}
	}
	return ret;
}

_WORD *grect_to_array(const GRECT *g, _WORD *pxy)
{
	pxy[0] = g->g_x;
	pxy[1] = g->g_y;
	pxy[2] = g->g_x + g->g_w - 1;
	pxy[3] = g->g_y + g->g_h - 1;
	return pxy;
}

GRECT *array_to_grect(const _WORD *pxy, GRECT *g)
{
	g->g_x = pxy[0];
	g->g_y = pxy[1];
	g->g_w = pxy[2] - pxy[0] + 1;
	g->g_h = pxy[3] - pxy[1] + 1;
	return g;
}

#include <stdarg.h>

/** size of the aes_control[] array */
#define AES_CTRLMAX		5
/** size of the aes_global[] array */
#define AES_GLOBMAX		16
/** size of the aes_intin[] array */
#define AES_INTINMAX 		16
/** size of the aes_intout[] array */
#define AES_INTOUTMAX		16
/** size of the aes_addrin[] array */
#define AES_ADDRINMAX		4
/** size of the aes_addrout[] array */
#define AES_ADDROUTMAX		1

#define CHECK_NULL_PTR 0

#define AES_PARAMS(opcode,nintin,nintout,naddrin,naddrout) \
	static _WORD aes_control[AES_CTRLMAX] = { opcode, nintin, nintout, naddrin, naddrout }; \
	_WORD aes_intin[AES_INTINMAX]; \
	_WORD aes_intout[AES_INTOUTMAX]; \
	void *aes_addrin[AES_ADDRINMAX]; \
	void *aes_addrout[AES_ADDROUTMAX]; \
	AESPB aes_params; \
 \
	aes_params.contrl  = aes_control;			/* AES Control Array */ \
	aes_params.global  = _GemParBlk.global;		/* AES Global Array */ \
	aes_params.intin   = aes_intin;				/* input integer array */ \
	aes_params.intout  = aes_intout;			/* output integer array */ \
	aes_params.addrin  = (void *)aes_addrin;	/* input address array */ \
	aes_params.addrout = (void *)aes_addrout	/* output address array */

#define AES_TRAP(pb) _crystal(&pb)

#define aes_intout_long(n)  *((long *)(aes_intout+(n)))
#define aes_intin_long(n)  *((long *)(aes_intin+(n)))
#define aes_intout_ptr(n, t)  *((t *)((void **)(aes_intout+(n))))
#define aes_intin_ptr(n, t)  *((t *)((void **)(aes_intin+(n))))

/*
 * The original implementation of wind_get() uses
 * a builtin table to look up the number of
 * output parameters depending on type,
 * and we cannot call it safely with newer modes.
 * So we must replace that, too.
 */
_WORD wind_get(_WORD WindowHandle, _WORD What, ...)
{
	va_list args;
	_WORD *W;
	AES_PARAMS(104, 2, 5, 0, 0);

	aes_intin[0] = WindowHandle;
	aes_intin[1] = What;

	va_start(args, What);
	W = va_arg(args, _WORD *);

	switch (What)
	{
	case WF_DCOLOR:
	case WF_COLOR:
		aes_intin[2] = *W;
		aes_control[1] = 3;
		break;
	case WF_INFO:
	case WF_NAME:
		*((_WORD **)&aes_intin[2]) = W;
		aes_control[1] = 4;
		break;
	default:
		aes_control[1] = 2;
		break;
	}

	aes_intout[1] = aes_intout[2] = aes_intout[3] = aes_intout[4] = 0;

	AES_TRAP(aes_params);
	
	switch (What)
	{
	/*
	 * functions that return a pointer
	 */
	case WF_SCREEN:
		*((void **)W) = *((void **)&aes_intout[1]);
		W = va_arg(args, _WORD *);
#if CHECK_NULLPTR
		if (W)
#endif
			*((long *)W) = *((long *)&aes_intout[3]);
		break;
	case WF_NEWDESK:
		*((void **)W) = *((void **)&aes_intout[1]);
		W = va_arg(args, _WORD *);
#if CHECK_NULLPTR
		if (W)
#endif
			*W = aes_intout[3];
		break;
	case WF_TOOLBAR:
	case WF_USER_POINTER:
	case WF_WIND_ATTACH:
		*((void **)W) = *((void **)&aes_intout[1]);
		break;
	case WF_INFO:
	case WF_NAME:
		/* special case where W1 shall not be overwritten */
		break;
	default:
		if (W) *W = aes_intout[1];
		W = va_arg(args, _WORD *);
		if (W) *W = aes_intout[2];
		W = va_arg(args, _WORD *);
		if (W) *W = aes_intout[3];
		W = va_arg(args, _WORD *);
		if (W) *W = aes_intout[4];
		break;
	}

	va_end(args);
	
	return aes_intout[0];
}

#define in(r) r->g_x, r->g_y, r->g_w, r->g_h
#define out(r) &r->g_x, &r->g_y, &r->g_w, &r->g_h

_WORD form_center_grect(OBJECT *tree, GRECT *r)
{
	return form_center(tree, out(r));
}

_WORD form_dial_grect(_WORD subfn, const GRECT *lg, const GRECT *bg)
{
	return form_dial(subfn, in(lg), in(bg));
}

_WORD objc_draw_grect(OBJECT *tree, _WORD start, _WORD depth, const GRECT *r)
{
	return objc_draw(tree, start, depth, in(r));
}

_WORD objc_change_grect(OBJECT *tree, _WORD start, _WORD depth, const GRECT *r, _WORD newstate, _WORD redraw)
{
	return objc_change(tree, start, depth, in(r), newstate, redraw);
}

_WORD wind_get_grect(_WORD whl, _WORD srt, GRECT *g)
{
	return wind_get(whl, srt, out(g));
}

_WORD wind_calc_grect(_WORD type, _WORD parts, const GRECT *in, GRECT *out)
{
	return wind_calc(type, parts, in(in), out(out));
}

_WORD wind_create_grect(_WORD parts, const GRECT *r)
{
	return wind_create(parts, in(r));
}

_WORD wind_open_grect(_WORD whl, const GRECT *r)
{
	return wind_open(whl, in(r));
}

_WORD wind_set_grect(_WORD whl, _WORD srt, const GRECT *r)
{
	return wind_set(whl, srt, in(r));
}

_WORD wind_set_str(_WORD whl, _WORD srt, const char *s)
{
	return wind_set(whl, srt, s);
}

_WORD objc_sysvar(_WORD ob_svmode, _WORD ob_svwhich,
                 _WORD ob_svinval1, _WORD ob_svinval2,
                 _WORD *ob_svoutval1, _WORD *ob_svoutval2)
{
	_WORD *ptr;
	AES_PARAMS(48, 4, 3, 0, 0);

	ptr = aes_intin;
	*ptr++ = ob_svmode;
	*ptr++ = ob_svwhich;
	*ptr++ = ob_svinval1;
	*ptr++ = ob_svinval2;

	AES_TRAP(aes_params);
	
	*ob_svoutval1 = aes_intout[1];
	*ob_svoutval2 = aes_intout[2];

	return aes_intout[0];
}

PRN_DIALOG *pdlg_create(_WORD dialog_flags)
{
	AES_PARAMS(200,1,0,0,1);

	aes_intin[0] = dialog_flags;
	aes_addrout[0] = 0;

	AES_TRAP(aes_params);

	return aes_addrout[0];
}

_WORD pdlg_delete(PRN_DIALOG *prn_dialog)
{
	AES_PARAMS(201,0,1,1,0);

	aes_addrin[0] = prn_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_open(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings, const char *document_name,
             _WORD option_flags, _WORD x, _WORD y)
{
	AES_PARAMS(202,3,1,3,0);

	aes_intin[0] = option_flags;
	aes_intin[1] = x;
	aes_intin[2] = y;
	
	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_addrin[2] = document_name;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_close(PRN_DIALOG *prn_dialog, _WORD *x, _WORD *y)
{
	AES_PARAMS(203,0,3,1,0);

	aes_intout[1] = -1;
	aes_intout[2] = -1;

	aes_addrin[0] = prn_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (x)
#endif
	*x = aes_intout[1];
#if CHECK_NULLPTR
	if (y)
#endif
	*y = aes_intout[2];
	
	return aes_intout[0];
}

long pdlg_get_setsize(void)
{
	AES_PARAMS(204,1,2,0,0);

	aes_intin[0] = 0;
	aes_intout_long(0) = 0;

	AES_TRAP(aes_params);

	return aes_intout_long(0);
}

_WORD pdlg_add_printers(PRN_DIALOG *prn_dialog, DRV_INFO *drv_info)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 0;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = drv_info;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_remove_printers(PRN_DIALOG *prn_dialog)
{
	AES_PARAMS(205,1,1,1,0);

	aes_intin[0] = 1;

	aes_addrin[0] = prn_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_update(PRN_DIALOG *prn_dialog, const char *document_name)
{
	AES_PARAMS(205,1,1,3,0);

	aes_intin[0] = 2;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = 0L;
	aes_addrin[2] = document_name;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_add_sub_dialogs(PRN_DIALOG *prn_dialog, PDLG_SUB *sub_dialogs)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 3;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = sub_dialogs;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_remove_sub_dialogs(PRN_DIALOG *prn_dialog)
{
	AES_PARAMS(205,1,1,1,0);

	aes_intin[0] = 4;

	aes_addrin[0] = prn_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

PRN_SETTINGS *pdlg_new_settings(PRN_DIALOG *prn_dialog)
{
	AES_PARAMS(205,1,0,1,1);

	aes_intin[0] = 5;
	
	aes_addrin[0] = prn_dialog;
	aes_addrout[0] = 0;

	AES_TRAP(aes_params);
	
	return (PRN_SETTINGS*) aes_addrout[0];
}

_WORD pdlg_free_settings(PRN_SETTINGS *settings)
{
	AES_PARAMS(205,1,1,1,0);

	aes_intin[0] = 6;

	aes_addrin[0] = settings;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_dflt_settings(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 7;
	
	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_validate_settings(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 8;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_use_settings(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 9;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_save_default_settings(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings)
{
	AES_PARAMS(205,1,1,2,0);

	aes_intin[0] = 10;

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD pdlg_evnt(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings, EVNT *events, _WORD *button)
{
	AES_PARAMS(206,0,2,3,0);

	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_addrin[2] = events;
	aes_intout[0] = 0;
	aes_intout[1] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (button)
#endif
	*button = aes_intout[1];
	
	return aes_intout[0];
}

_WORD pdlg_do(PRN_DIALOG *prn_dialog, PRN_SETTINGS *settings, const char *document_name,
           _WORD option_flags)
{
	AES_PARAMS(207,1,1,3,0);

	aes_intin[0] = option_flags;
	
	aes_addrin[0] = prn_dialog;
	aes_addrin[1] = settings;
	aes_addrin[3] = document_name;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

FNT_DIALOG *fnts_create(_WORD vdi_handle, _WORD no_fonts, _WORD font_flags, _WORD dialog_flags,
               const char *sample, const char *opt_button)
{
	AES_PARAMS(180,4,0,2,1);
                    
	aes_intin[0] = vdi_handle;
	aes_intin[1] = no_fonts;
	aes_intin[2] = font_flags;
	aes_intin[3] = dialog_flags;
	
	aes_addrin[0] = sample;
	aes_addrin[1] = opt_button;
	aes_addrout[0] = 0;

	AES_TRAP(aes_params);

	return (FNT_DIALOG*)aes_addrout[0];
}

_WORD fnts_delete(FNT_DIALOG *fnt_dialog, _WORD vdi_handle)
{
	AES_PARAMS(181,1,1,1,0);
                    
	aes_intin[0]  = vdi_handle;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fnts_open(FNT_DIALOG *fnt_dialog, _WORD button_flags, _WORD x, _WORD y, long id, long pt, long ratio)
{
	AES_PARAMS(182,9,1,1,0);
                    
	aes_intin[0] = button_flags;
	aes_intin[1] = x;
	aes_intin[2] = y;
	aes_intin_long(3) = id;
	aes_intin_long(5) = pt;
	aes_intin_long(7) = ratio;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fnts_close(FNT_DIALOG *fnt_dialog, _WORD *x, _WORD *y)
{
	AES_PARAMS(183,0,3,1,0);
                    
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;
	aes_intout[1] = -1;	/* ab 6: Position kommt zurÅck */
	aes_intout[2] = -1;

	AES_TRAP(aes_params);
	
	*x = aes_intout[1];
	*y = aes_intout[2];
	
	return aes_intout[0];
}

_WORD fnts_get_no_styles(FNT_DIALOG *fnt_dialog, long id)
{
	AES_PARAMS(184,3,1,1,0);
                    
	aes_intin[0] = 0;
	aes_intin_long(1) = id;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

long fnts_get_style(FNT_DIALOG *fnt_dialog, long id, _WORD idx)
{
	AES_PARAMS(184,4,2,1,0);
                    
	aes_intin[0] = 1;
	aes_intin_long(1) = id;
	aes_intin[3] = idx;
	aes_addrin[0] = fnt_dialog;
	aes_intout_long(0) = 0;

	AES_TRAP(aes_params);

	return aes_intout_long(0);
}

_WORD fnts_get_name(FNT_DIALOG *fnt_dialog, long id, char *full_name, char *family_name,
                 char *style_name)
{
	AES_PARAMS(184,3,1,4,0);
                    
	aes_intin[0] = 2;
	aes_intin_long(1) = id;
	aes_addrin[0] = fnt_dialog;
	aes_addrin[1] = full_name;
	aes_addrin[2] = family_name;
	aes_addrin[3] = style_name;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fnts_get_info(FNT_DIALOG *fnt_dialog, long id, _WORD *mono, _WORD *outline)
{
	AES_PARAMS(184,3,3,1,0);
                    
	aes_intin[0]  = 3;
	aes_intin_long(1) = id;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (mono)
#endif
	*mono    = aes_intout[1];
#if CHECK_NULLPTR
	if (outline)
#endif
	*outline = aes_intout[2];
	
	return aes_intout[0];
}

_WORD fnts_add(FNT_DIALOG *fnt_dialog, FNTS_ITEM *user_fonts)
{
	AES_PARAMS(185,1,1,2,0);
                    
	aes_intin[0] = 0;
	
	aes_addrin[0] = fnt_dialog;
	aes_addrin[1] = user_fonts;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);
	
	return aes_intout[0];
}

void mt_fnts_remove(FNT_DIALOG *fnt_dialog)
{
	AES_PARAMS(185,1,0,1,0);
                    
	aes_intin[0] = 1;

	aes_addrin[0] = fnt_dialog;

	AES_TRAP(aes_params);
}

_WORD fnts_update(FNT_DIALOG *fnt_dialog, _WORD button_flags, long id, long pt, long ratio)
{
	AES_PARAMS(185,8,1,1,0);

	aes_intin[0] = 2;
	aes_intin[1] = button_flags;
	aes_intin_long(2) = id;
	aes_intin_long(4) = pt;
	aes_intin_long(6) = ratio;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fnts_evnt(FNT_DIALOG *fnt_dialog, EVNT *events, _WORD *button, _WORD *check_boxes,
             long *id, long *pt, long *ratio)
{
	AES_PARAMS(186,0,9,2,0);
                    
	aes_addrin[0] = fnt_dialog;
	aes_addrin[1] = events;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (button)
#endif
	*button = aes_intout[1];
#if CHECK_NULLPTR
	if (check_boxes)
#endif
	*check_boxes = aes_intout[2];
#if CHECK_NULLPTR
	if (id)
#endif
	*id = aes_intout_long(3);
#if CHECK_NULLPTR
	if (pt)
#endif
	*pt = aes_intout_long(5);
#if CHECK_NULLPTR
	if (ratio)
#endif
	*ratio = aes_intout_long(7);
	
	return aes_intout[0];
}

_WORD fnts_do(FNT_DIALOG *fnt_dialog, _WORD button_flags, long id_in, long pt_in, 
		   long ratio_in, _WORD *check_boxes, long *id, long *pt, long *ratio)
{
	AES_PARAMS(187,7,8,1,0);

	aes_intin[0] = button_flags;
	aes_intin_long(1) = id_in;
	aes_intin_long(3) = pt_in;
	aes_intin_long(5) = ratio_in;
	aes_addrin[0] = fnt_dialog;
	aes_intout[0] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (check_boxes)
#endif
	*check_boxes = aes_intout[1];
#if CHECK_NULLPTR
	if (id)
#endif
	*id = aes_intout_long(2);
#if CHECK_NULLPTR
	if (pt)
#endif
	*pt = aes_intout_long(4);
#if CHECK_NULLPTR
	if (ratio)
#endif
	*ratio = aes_intout_long(6);
	
	return aes_intout[0];
}

XFSL_DIALOG *fslx_open(const char *title, _WORD x, _WORD y, _WORD *handle, char *path, _WORD pathlen,
			 char *fname, _WORD fnamelen, const char *patterns, XFSL_FILTER filter,
			 char *paths, _WORD sort_mode, _WORD flags)
{
	AES_PARAMS(190,6,1,6,1);
                    
	aes_intin[0] = x;
	aes_intin[1] = y;
	aes_intin[2] = pathlen;
	aes_intin[3] = fnamelen;
	aes_intin[4] = sort_mode;
	aes_intin[5] = flags;
	
	aes_addrin[0] = title;
	aes_addrin[1] = path;
	aes_addrin[2] = fname;
	aes_addrin[3] = patterns;
	aes_addrin[4] = filter;
	aes_addrin[5] = paths;
	aes_addrout[0] = 0;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (handle)
#endif
	*handle = aes_intout[0];
	
	return aes_addrout[0];
}

_WORD fslx_close(XFSL_DIALOG *fsd)
{
	AES_PARAMS(191,0,1,1,0);

	aes_addrin[0] = fsd;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fslx_getnxtfile(XFSL_DIALOG *fsd, char *fname)
{
	AES_PARAMS(192,0,1,2,0);

	aes_addrin[0] = fsd;
	aes_addrin[1] = fname;

	AES_TRAP(aes_params);

	return aes_intout[0];
}

_WORD fslx_evnt(XFSL_DIALOG *fsd, EVNT *events, char *path, char *fname, _WORD *button, 
			 _WORD *nfiles, _WORD *sort_mode, char **pattern)
{
	AES_PARAMS(193,0,4,4,1);

	aes_addrin[0] = fsd;
	aes_addrin[1] = events;
	aes_addrin[2] = path;
	aes_addrin[3] = fname;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (button)
#endif
	*button = aes_intout[1];
#if CHECK_NULLPTR
	if (nfiles)
#endif
	*nfiles = aes_intout[2];
#if CHECK_NULLPTR
	if (sort_mode)
#endif
	*sort_mode = aes_intout[3];
#if CHECK_NULLPTR
	if (pattern)
#endif
	*pattern = (char *)aes_addrout[0];
	
	return aes_intout[0];
}

XFSL_DIALOG *fslx_do(const char *title, char *path, _WORD pathlen, char *fname, _WORD fnamelen, 
		   const char *patterns, XFSL_FILTER filter, char *paths, _WORD *sort_mode,
		   _WORD flags, _WORD *button, _WORD *nfiles, char **pattern)
{
	AES_PARAMS(194,4,4,6,2);

	aes_intin[0] = pathlen;
	aes_intin[1] = fnamelen;
	aes_intin[2] = *sort_mode;
	aes_intin[3] = flags;
	aes_addrin[0] = title;
	aes_addrin[1] = path;
	aes_addrin[2] = fname;
	aes_addrin[3] = patterns;
	aes_addrin[4] = filter;
	aes_addrin[5] = paths;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (button)
#endif
	*button = aes_intout[1];
#if CHECK_NULLPTR
	if (nfiles)
#endif
	*nfiles = aes_intout[2];
#if CHECK_NULLPTR
	if (sort_mode)
#endif
	*sort_mode = aes_intout[3];
#if CHECK_NULLPTR
	if (pattern)
#endif
	*pattern = (char *)aes_addrout[1];
	
	return aes_addrout[0];
}

_WORD fslx_set_flags(_WORD flags, _WORD *oldval)
{
	AES_PARAMS(195,2,2,0,0);

	aes_intin[0] = 0;
	aes_intin[1] = flags;

	AES_TRAP(aes_params);

#if CHECK_NULLPTR
	if (oldval)
#endif
	*oldval = aes_intout[1];
	
	return aes_intout[0];
}


#undef in
#undef out

#endif
