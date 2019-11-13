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

#ifndef _cfl_intern_
#define _cfl_intern_

/*
 * interne Definitionen der CFLib
 */
#include "cflib.h"

#ifndef SC_CHANGED
#define SC_CHANGED		80
#endif

#ifndef SM_M_SPECIAL
#define SM_M_SPECIAL		101
#endif

#ifndef K_SHIFT
#define K_SHIFT			(K_RSHIFT|K_LSHIFT)
#endif

/* vst_alignment() */
#ifndef TA_LEFT
#define TA_LEFT			0
#define TA_CENTER		1
#define TA_TOP 			5
#endif

/* boolean */
#ifdef TRUE
#undef TRUE
#undef FALSE
#endif

#define TRUE			1
#define FALSE			0

#ifdef __GNUC__
#include <mint/osbind.h>
#include <mint/mintbind.h>
#include <gemx.h>
#else
#include <tos.h>
#endif

#ifdef __GNUC__
/* ist in osbind.h komisch definiert. */
typedef struct
{
	unsigned char	*unshift;
	unsigned char	*shift;
	unsigned char	*capslock;
} KEYTAB;
#endif

void *cf_malloc(long size, char *who, int global);

/* cicon.c */
CICON *fix_cicon(CICONBLK *cicnblk, _WORD screen_planes, _WORD handle);


/* form_do.c */
#define FMD_BACKWARD 1
#define FMD_FORWARD	2

_WORD edit_valid(OBJECT *tree, _WORD obj);
_WORD find_edit(OBJECT *tree, _WORD obj, _WORD mode);
_WORD cf_objc_find(OBJECT *tree, _WORD start, _WORD depth, _WORD x, _WORD y);
_WORD find_shortcut(OBJECT *tree, _WORD kstate, _WORD kreturn);
_WORD cf_form_keybd(OBJECT *tree, _WORD edit_obj, _WORD kstate, _WORD *kreturn, _WORD *next_obj);
void cf_objc_edit(OBJECT *tree, _WORD obj, _WORD kreturn, _WORD *idx, _WORD mode, _WORD kstate, _WORD *ctrl);


/* mdial.c */
void handle_mdial_msg(_WORD *msg);


/* userdef.c */
extern OBJECT	*cf_ascii_tab;
extern OBJECT	*cf_alert_box;

void init_userdef(void);
void exit_userdef(void);

/* popup */
_WORD popup_valid_item(OBJECT *tree, _WORD obj);


#if defined(__GNUC__) && defined(__MINT__)

#include <support.h>
#define ltoa(a,b,c)	_ltoa(a,b,c)
#define ultoa(a,b,c)	_ultoa(a,b,c)

#define DTA _DTA

#endif

/* FIXME: should be in public api */
#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)
void vqt_real_extent(_WORD handle, _WORD x, _WORD y, const char *string, _WORD extent[]);
#endif

#undef min
#undef max
#undef abs

#define min(a,b)	  (((a) < (b)) ? (a) : (b))
#define max(a,b)	  (((a) > (b)) ? (a) : (b))
#define abs(a)  ((a) < 0 ? -(a) : (a))

#endif
