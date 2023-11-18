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
#ifdef __GNUC__
#include <mint/osbind.h>
#else
#include <tos.h>
#ifndef KEYTAB
#define _KEYTAB KEYTAB
#define caps capslock
#endif
#endif


#define VERSION 0x294

/****************************************************************************
 *
 *	reduced NKCC for the CF-Lib
 *
 ****************************************************************************/

/*  ASCII code translation table for Control key
 *
 *  first entry.B:  modified ASCII code returned by TOS
 *  second entry.B: original ASCII code as stored in key table
 *
 *  The table is terminated with both entries 0
 */
static unsigned short const nkc_asc_trans[] = {
	0x0032,				/* Control '2' becomes ASCII 0 */
	0x1e36, 			/* Control '6' becomes ASCII $1e */
	0x1f2d, 			/* Control '-' becomes ASCII $1f */
	0x0a0d,				/* Control Return/Enter: $d -> $a */
	0					/* terminator */
};



static _KEYTAB nkc_keytab;

static short const xscantab[] =
{
	(NK_UP << 8) |	0x48,	   /* cursor up			    */
	(NK_DOWN << 8) | 0x50,     /* cursor down			*/
	(NK_LEFT << 8) | 0x4b,	   /* cursor left			*/
	(NK_LEFT << 8) | 0x73,	   /* Control cursor left	*/
	(NK_RIGHT << 8) | 0x4d,	   /* cursor right			*/
	(NK_RIGHT << 8) | 0x74,	   /* Control cursor right	*/
	(NK_M_PGUP << 8) | 0x49,   /* Mac: page up           */
	(NK_M_PGDOWN << 8) | 0x51, /* Mac: page down         */
	(NK_M_END << 8) | 0x4f,    /* Mac: end               */
	(NK_INS << 8) | 0x52,	   /* Insert				    */
	(NK_CLRHOME << 8) | 0x47,  /* ClrHome				*/
	(NK_CLRHOME << 8) | 0x77,  /* Control ClrHome		*/
	(NK_HELP << 8) | 0x62,	   /* Help					*/
	(NK_UNDO << 8) | 0x61,	   /* Undo					*/
	(NK_M_F11 << 8) | 0x45,    /* Mac: F11               */
	(NK_M_F12 << 8) | 0x46,    /* Mac: F12               */
	(NK_M_F14 << 8) | 0x37,    /* Mac: F14               */
	-1
};

/****************************************************************************
 *
 *  nkc_init: initialize NKCC
 *
 ****************************************************************************/
_WORD nkc_init(void)
{
	void *pmin = (void *)-1;
	_KEYTAB *tbl;
	
	/* fetch addresses of TOS' key scan code translation tables */
	tbl = Keytbl(pmin, pmin, pmin);
	nkc_keytab.unshift = tbl->unshift;
	nkc_keytab.shift = tbl->shift;
	nkc_keytab.caps = tbl->caps;
	return VERSION;
}

/*
*  In:   D0.L           key code in TOS format:
*                                   0                    1
*                       bit 31:     ignored              ignored
*                       bit 30:     ignored              ignored
*                       bit 29:     ignored              ignored
*                       bit 28:     no CapsLock          CapsLock
*                       bit 27:     no Alternate         Alternate pressed
*                       bit 26:     no Control           Control pressed
*                       bit 25:     no left Shift key    left Shift pressed
*                       bit 24:     no right Shift key   right Shift pressed
*
*                       bits 23...16: scan code
*                       bits 15...08: ignored
*                       bits 07...00: ASCII code (or rubbish in most cases
*                          when Control or Alternate is pressed ...)
*
*  Out:  D0.W           normalized key code:
*                       bits 15...08: flags:
*                                   0                    1
*                       NKF?_FUNC   printable char       "function key"
*                       NKF?_RESVD  ignore it            ignore it
*                       NKF?_NUM    main keypad          numeric keypad
*                       NKF?_CAPS   no CapsLock          CapsLock
*                       NKF?_ALT    no Alternate         Alternate pressed
*                       NKF?_CTRL   no Control           Control pressed
*                       NKF?_LSH    no left Shift key    left Shift pressed
*                       NKF?_RSH    no right Shift key   right Shift pressed
*
*                       bits 07...00: key code
*                       function (NKF?_FUNC set):
*                          < 32: special key (NK_...)
*                          >=32: printable char + Control and/or Alternate
*                       no function (NKF?_FUNC not set):
*                          printable character (0...255!!!)
*/
unsigned short nkc_tos2n(long toskey)
{
	const short *tab;
	const unsigned char *ktab;
	unsigned short state, scan, ascii;
	short nkc;

	ascii = toskey & 0xff;
	state = (unsigned short)(toskey >> 16);
	scan = state & 0xff;
	
	if (scan == 0)
	{
		/* special handling for key codes created by TOS' 3.06 ASCII input */
		state &= NKF_CAPS;
		return ascii | state;
	}
	
	state &= (NKF_CAPS|NKF_SHIFT|NKF_CTRL|NKF_ALT);
	
	if (state & NKF_SHIFT)
		ktab = (const unsigned char *)nkc_keytab.shift;
	else if (state & NKF_CAPS)
		ktab = (const unsigned char *)nkc_keytab.caps;
	else
		ktab = (const unsigned char *)nkc_keytab.unshift;

	/*
	 *------------- check if scan code is out of range
	 *
	 * Illegal scancodes can be used to produce 'macro key codes'. Their format is:
	 *
	 * - the scancode must be $84 or larger (should be $ff to work properly with old
	 *   versions of Mag!x)
	 * - the ASCII code must be in the range $20...$ff (values below are set to $20
	 *   by NKCC)
	 * - Alternate and Control are not used for the normalized key code. However,
	 *   if at least one of them is non-zero, then the numeric keypad flag will be
	 *   set in the resulting key code.
	 */
	if (scan >= 0x84)
	{
		/* Alternate or Control? */
		if (state & (NKF_ALT | NKF_CTRL))
		{
			/* yes: set numeric keypad flag */
			ascii |= NKF_NUM;
			/* mask off both flags */
			state &= NKF_CAPS | NKF_SHIFT;
		}
		/* set function and resvd */
		state |= NKF_FUNC | NKF_RESVD;
		/* ASCII code in range? */
		if ((unsigned char)ascii < 0x20)
		{
			/* no: use minimum */
			ascii = 0x20;
		}
		/* combine with ASCII code */
		return ascii | state;
	}
	
	/*
	 * check if Alternate + number: they have simulated scan codes
	 */
	if (scan >= 0x78)
	{
		ascii = ktab[scan - 0x76];
		state |= NKF_ALT;
	} else
	{
		/*
		 * check if exception scan code from cursor keypad
		 */
		/* no NKC found yet */
		nkc = 0;
		tab = xscantab;
		while (*tab > 0)
		{
			if ((*tab & 0xff) == scan)
			{
				nkc = (*tab >> 8) & 0xff;
				/* mark: key code found */
				ascii = 0;
				break;
			}
			tab++;
		}
		
		/*
		 * check if rubbish ASCII code and erase it, if so
		 */
		if (ascii <= 0x20 && ktab[scan] != ascii)
			ascii = 0;
		
		/*
		 * check if ASCII code could only be produced via Alternate key
		 * combination
		 */
		if (ascii != 0 && ktab[scan] != ascii)
			state &= ~NKF_ALT;
		
		/*
		 * check if ASCII code found yet, and set it, if not
		 */
		if (ascii == 0)
		{
			/* not found, use code from exception table */
			ascii = nkc;
			if (ascii == 0)
				ascii = ktab[scan];
		}
		
		/*
		 * check special case: delete key
		 */
		if (ascii == 0x7f)
			ascii = NK_DEL;
		
		/*
		 * check if key is on numeric keypad (via scan code)
		 */
		if (scan == 0x4a || scan == 0x4e || (scan >= 0x63 && scan <= 0x72))
			state |= NKF_NUM;

		/*
		 * check if "function key" and set bit accordingly
		 */
		if (ascii < 0x20)
		{
			state |= NKF_FUNC;
			/* check special case: Return or Enter key */
			if (ascii == 0x0d && (state & NKF_NUM))
				ascii = NK_ENTER;
		}
		
		/*
		 * check if function key (F1-F10) via scan code
		 */
		if (scan >= 0x54 && scan <= 0x5d)
		{
			/* yes: scan code for unshifted key */
			scan -= 0x54 - 0x3b;
			if (state & NKF_SHIFT)
				state |= NKF_SHIFT;
		}
		/* (unshifted) function key? */
		if (scan >= 0x3b && scan <= 0x44)
		{
			/* yes: calc NKC */
			ascii = scan - 0x2b;
		}
	}
	
	/*
	 * final flag handling; mix key code (low byte) and flag byte
	 */
	ktab = (const unsigned char *)nkc_keytab.shift;
	nkc = ktab[scan];
	if ((state & NKF_FUNC) || (state & (NKF_CTRL|NKF_ALT)) == 0)
	{
		ktab = (const unsigned char *)nkc_keytab.unshift;
		if (ktab[scan] != nkc)
			return ascii | state;
	} else
	{
		state |= NKF_FUNC;
		ktab = (const unsigned char *)nkc_keytab.caps;
		if (ktab[scan] != nkc)
			return ascii | state;
		ascii = nkc;
	}
	state |= NKF_RESVD;
	return ascii | state;
}


/*
 *  normalized key code -> scan code translation table
 *  for unshifted key codes
 *  indexed by function code (NK_...)
 */
static unsigned char const n_to_scan_u[] = {
	0x00,                  /* invalid key code */
	0x48,                  /* cursor up */
	0x50,                  /* cursor down */
	0x4d,                  /* cursor right */
	0x4b,                  /* cursor left */
	0x49,                  /* Mac: page up */
	0x51,                  /* Mac: page down */
	0x4f,                  /* Mac: end */
	0x0e,                  /* Backspace */
	0x0f,                  /* Tab */
	0x72,                  /* Enter */
	0x52,                  /* Insert */
	0x47,                  /* ClrHome */
	0x1c,                  /* Return */
	0x62,                  /* Help */
	0x61,                  /* Undo */
	0x3b,                  /* function key #1 */
	0x3c,                  /* function key #2 */
	0x3d,                  /* function key #3 */
	0x3e,                  /* function key #4 */
	0x3f,                  /* function key #5 */
	0x40,                  /* function key #6 */
	0x41,                  /* function key #7 */
	0x42,                  /* function key #8 */
	0x43,                  /* function key #9 */
	0x44,                  /* function key #10 */
	0x45,                  /* Mac: F11 */
	0x01,                  /* Esc */
	0x46,                  /* Mac: F12 */
	0x37,                  /* Mac: F14 */
	0x00,                  /* reserved! */
	0x53                   /* Delete */
};

/*
 *  normalized key code -> scan code translation table
 *  for shifted key codes
 *  indexed by function code (NK_...)
 */
static unsigned char const n_to_scan_s[] = {
	0x00,                  /* invalid key code */
	0x48,                  /* cursor up */
	0x50,                  /* cursor down */
	0x4d,                  /* cursor right */
	0x4b,                  /* cursor left */
	0x49,                  /* Mac: page up */
	0x51,                  /* Mac: page down */
	0x4f,                  /* Mac: end */
	0x0e,                  /* Backspace */
	0x0f,                  /* Tab */
	0x72,                  /* Enter */
	0x52,                  /* Insert */
	0x47,                  /* ClrHome */
	0x1c,                  /* Return */
	0x62,                  /* Help */
	0x61,                  /* Undo */
	0x54,                  /* function key #1 */
	0x55,                  /* function key #2 */
	0x56,                  /* function key #3 */
	0x57,                  /* function key #4 */
	0x58,                  /* function key #5 */
	0x59,                  /* function key #6 */
	0x5a,                  /* function key #7 */
	0x5b,                  /* function key #8 */
	0x5c,                  /* function key #9 */
	0x5d,                  /* function key #10 */
	0x45,                  /* Mac: F11 */
	0x01,                  /* Esc */
	0x46,                  /* Mac: F12 */
	0x37,                  /* Mac: F14 */
	0x00,                  /* reserved! */
	0x53                   /* Delete */
};


/****************************************************************************
 *
 *  nk_findscan: find scan code
 *
 ****************************************************************************/

static unsigned char nk_findscan(const unsigned char *tab, unsigned short kcode)
{
	unsigned char scan;
	
	/* on numeric keypad? */
	if (kcode & NKF_NUM)
	{
		/* yes: try all numeric keypad scan codes first */
		scan = 0x4a;
		if ((unsigned char) kcode == tab[scan])
			return scan;
		scan = 0x4e;
		if ((unsigned char) kcode == tab[scan])
			return scan;
		scan = 0x63;
		do {
			if ((unsigned char) kcode == tab[scan])
				return scan;
		} while (++scan < 0x73);	
	}
	scan = 0x1;
	do {
		if ((unsigned char) kcode == tab[scan])
			return scan;
	} while (++scan < 0x78);
	return 0;
}


/****************************************************************************
 *
 *  nkc_n2tos: convert normalized key codes back to TOS format
 *
 ****************************************************************************/

long nkc_n2tos(long kcode)
{
	unsigned short kstate;
	const unsigned char *tab;
	const unsigned char *scantab;
	unsigned char scan;
	unsigned long key;
	const unsigned short *asctab;
	
	kstate = (unsigned short)(kcode & (NKF_FUNC|NKF_ALT|NKF_CTRL));
	/* only function flag set? */
	if (kstate == NKF_FUNC && (unsigned char)kcode < 0x20)
	{
		/* macro key */
		
		/* isolate usable flags */
		kstate = (unsigned char) kcode;
		kcode &= NKF_CAPS | NKF_SHIFT;
		if (kstate & NKF_ALT)
			kcode |= NKF_ALT | NKF_CTRL;
		/* scan code always $ff */
		kcode |= 0xff;
		return ((unsigned long)kcode << 16) | kstate;
	}
	
	/* select system key table to use */
	kstate = kcode & NKF_SHIFT;
	if (kstate)
	{
		scantab = n_to_scan_s;
		tab = (const unsigned char *)nkc_keytab.shift;
	} else if (kcode & NKF_CAPS)
	{
		scantab = n_to_scan_s;
		tab = (const unsigned char *)nkc_keytab.caps;
	} else
	{
		scantab = n_to_scan_u;
		tab = (const unsigned char *)nkc_keytab.unshift;
	}
	
	if ((unsigned char)kcode >= 0x20)
	{
		/* handling for ASCII codes >= 32 */
		scan = nk_findscan(tab, kcode);
		if (scan == 0)
		{
			if (kcode & NKF_FUNC)
			{
				kcode = nkc_tolower(kcode);
				scan = nk_findscan(tab, kcode);
			}
		}
		if (scan == 0)
		{
			/* unknown source: treat key code as it was entered using the */
			/* TOS 3.06 direct ASCII input */
			kstate = (unsigned char)kcode;
			/* keep shift flags only */
			kcode &= NKF_SHIFT | NKF_CTRL | NKF_ALT | NKF_CAPS;
			return ((unsigned long)kcode << 16) | kstate;
		}
	} else
	{
		/* handling for ASCII codes < 32 */
		if (!(kcode & NKF_FUNC))
		{
			/* clear all flags except CapsLock */
			kcode &= NKF_CAPS | 0xff;
			kstate = (unsigned char)kcode;
			return ((unsigned long)kcode << 16) | kstate;
		}
		scan = scantab[(unsigned char)kcode];
		if (scan == 0)
			/* invalid key code!! return 0 */
			return 0;
		/* get scan code from unshifted tab. */
		scantab = n_to_scan_u;
		kstate = scantab[(unsigned char)kcode];
		/* get ASCII from system's table */
		kcode &= 0xff00;
		kcode |= scantab[(unsigned char) kstate];
	}
	
	/* flags and ASCII code */
	kstate = kcode;
	/* isolate shift flags */
	kcode &= NKF_SHIFT | NKF_CTRL | NKF_ALT | NKF_CAPS;
	/* merge with scan code */
	kcode |= scan;
	
	key = ((unsigned long)kcode << 16) | ((unsigned char)kstate);
	
	/* handling for Control key flag */
	if (kstate & NKF_CTRL)
	{
		if (scan == 0x4b)
		{
			/* scan code = "cursor left", change scan code to $73 */
			key += 0x280000l;
			key &= 0xffffff00l;
			return key;
		}
		if (scan == 0x4d)
		{
			/* scan code = "cursor right", change scan code to $74 */
			key += 0x270000l;
			key &= 0xffffff00l;
			return key;
		}
		if (scan == 0x47)
		{
			/* scan code = "ClrHome", change scan code to $77 */
			key += 0x300000l;
			return key;
		}
		asctab = nkc_asc_trans;
		while ((kstate = *asctab++) != 0)
		{
			if ((unsigned char)kstate == (unsigned char)kcode)
			{
				key = ((unsigned long)kcode << 16) | ((unsigned char)(kstate >> 8));
				return key;
			}
		}
		/* mask off upper 3 bits */
		key &= 0xffff001fUL;
		return key;
	}
	
	/* handling for Alternate key flag */
	if (kcode & NKF_ALT)
	{
		if (scan >= 0x02 && scan < 0x0d)
		{
			/* top row on main keyboard, change scan code */
			key += 0x760000l;
			/* and erase ASCII code */
			key &= 0xffffff00l;
			return key;
		}
		
		if (((unsigned char) kcode >= 'A' && (unsigned char) kcode <= 'Z') ||
			((unsigned char) kcode >= 'a' && (unsigned char) kcode <= 'z'))
			;
		else
			key &= 0xffffff00l;
	}
	
	return key;
}
