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
 * Globale Variablen fr die cflib.
 */

#include "intern.h"


#ifndef str
# define str(x)		_stringify (x)
# define _stringify(x)	#x
#endif

#define CFLIB_PATCHLEVEL	str(__CFLIB_MAJOR__) "." str(__CFLIB_MINOR__) "." str(__CFLIB_REVISION__) __CFLIB_BETATAG__

/*
 * global identify string for the lib
 */
char const __Ident_cflib[] = "$PatchLevel: CF library: " CFLIB_PATCHLEVEL " $";


_WORD	gl_phys_handle = -1;
GRECT	gl_desk;

_WORD	sys_big_id;
_WORD	sys_big_height;
_WORD	sys_big_pts;
_WORD	sys_sml_id;
_WORD	sys_sml_height;
_WORD	sys_sml_pts;
_WORD	sys_wchar;
_WORD	sys_hchar;
_WORD	sys_wbox;
_WORD	sys_hbox;

_WORD	gl_wchar;
_WORD	gl_hchar;
_WORD	gl_wbox;
_WORD	gl_hbox;

_WORD	gl_gdos = FALSE;
_WORD	gl_font_anz = 1;
_WORD	gl_planes = 1;

_WORD	gl_gem = 0;
_WORD	gl_mint = 0;
_WORD	gl_naes = 0;
long	gl_xaaes = 0;
_WORD	gl_magx = 0;
_WORD	gl_nvdi = 0;

char	gl_appdir[256];
