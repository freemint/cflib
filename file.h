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
 * Funktionen auf das Dateisystem.
 */

#include "intern.h"

#if defined(__PUREC__)
#include <ext.h>
#define S_IFMT		0170000
#ifndef S_IFDIR
#define S_IFDIR		0040000
#endif
#ifndef S_ISDIR
#define S_ISDIR(m)	((m & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#endif

#include <ctype.h>
#include "app.h"
