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


WDIALOG *__wdial_list = NULL;

#ifdef __PUREC__
#include "wdial_call_cb.c"
#include "wdial_change_wdedit.c"
#include "wdial_click.c"
#include "wdial_close.c"
#include "wdial_create.c"
#include "wdial_delete.c"
#include "wdial_draw.c"
#include "wdial_draw_cursor.c"
#include "wdial_get.c"
#include "wdial_iconify.c"
#include "wdial_key.c"
#include "wdial_message.c"
#include "wdial_open.c"
#include "wdial_redraw_wdicon.c"
#include "wdial_redraw_wdobj.c"
#include "wdial_uniconify.c"
#endif
