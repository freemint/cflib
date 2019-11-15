/*
 * ExtObFix programm for InterFace
 * Copyright (C) 1998, 1999, 2000 Christian Felsch
 * 
 * Modified for FreeMiNT CVS by Frank Naumann <fnaumann@freemint.de>
 * 
 * Please send suggestions, patches or bug reports to me or
 * the MiNT mailing list.
 * 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <cflib.h>

#ifndef FALSE
#define FALSE	0
#define TRUE	1
#endif

#include "exthelp.rsh"
#include "exthelp.h"

/****** Functions ************************************************************/

long pinit_obfix(_WORD status);

static void fix_objs(OBJECT *tree, _WORD is_dialog);
static void test_form(OBJECT *tree, _WORD editobj, void (*get_n) (void *, _WORD, char *), void *window);
static _WORD test_alert(_WORD def, char *str);

/****** Variables ************************************************************/

#ifdef __GNUC__
__attribute__((__used__))
#endif
static long routines[] =
{
	(long)fix_objs,
	(long)test_alert,
	(long)test_form,
	(long)&rs_object[0],
	30363130L, 31393635L,
	(long)pinit_obfix,
};

/*****************************************************************************/

static void fix_objs(OBJECT *tree, _WORD is_dialog)
{
	if (is_dialog)
		fix_dial(tree);
	else
		fix_menu(tree);
}

/*****************************************************************************/

static void test_form(OBJECT *tree, _WORD editobj, void (*get_n) (void *, _WORD, char *), void *window)
{
	_WORD x, y, w, h, but, zw;
	char name[20], buf[80];

	fix_objs(tree, TRUE);

	wind_update(BEG_UPDATE);
	form_center(tree, &x, &y, &w, &h);
	form_dial(FMD_START, 0, 0, 0, 0, x, y, w, h);

	do
	{
		objc_draw(tree, ROOT, MAX_DEPTH, x, y, w, h);
		but = cf_form_do(tree, &editobj);

		/* keine TOUCHEXITs deselektieren */
		if (get_flag(tree, but, OF_EXIT))
			set_state(tree, but, OS_SELECTED, FALSE);

		(*get_n)(window, but, name);
		sprintf(buf, rs_frstr[EXITALERT], but, name);
		zw = do_alert(1, 2, buf);
	} while (zw == 1);

	form_dial(FMD_FINISH, 0, 0, 0, 0, x, y, w, h);
	wind_update(END_UPDATE);
	but = 0;
}

/*****************************************************************************/
static _WORD test_alert(_WORD def, char *str)
{
	return do_alert(def, 0, str);
}

/*****************************************************************************/

int main(void)
{
	char pl[10];
	char str[80];

	appl_init();
	get_patchlev(__Ident_cflib, pl);
	sprintf(str, "[0][ExtObFix|used by InterFace. |CF-Lib PL%s|%s][OK]", pl, __DATE__);
	form_alert(1, str);
#ifndef __GNUC__
	{
	static long dummy;
	dummy = routines[6];	/* Sonst wird die Struktur von Pure C wegoptimiert... */
	}
#endif
	appl_exit();
	(void) rs_trindex;
	return 0;
}

/*****************************************************************************/

long pinit_obfix(_WORD status)
{
	_WORD i;

	if (status == TRUE)
	{
		init_app(NULL);

		/* Inline-RSC anpassen */
		for (i = 0; i < NUM_OBS; i++)
			rsrc_obfix(&rs_object[i], 0);
		return ((long)routines);
	}
	else
	{
		exit_gem();
	}
	return 0;
}
