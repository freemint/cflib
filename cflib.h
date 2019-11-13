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

#ifndef _cflib_h_
#define _cflib_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__PUREC__)
#include <aes.h>
#include <vdi.h>
#else
#include <gem.h>
#endif

#ifndef _WORD
#  ifdef __GNUC__
#    define _WORD short
#    define _UWORD unsigned short
#  endif
#  ifdef __PUREC__
#    define _WORD int
#    define _UWORD unsigned int
#  endif
#endif
#ifndef _WORD
#  ifdef __GNUC__
#    define _WORD short
#    define _UWORD unsigned short
#  endif
#  ifdef __PUREC__
#    define _WORD int
#    define _UWORD unsigned int
#  endif
#endif

#ifndef _LONG
#  ifdef LONG
#    define _LONG LONG
#  else
#    define _LONG long
#  endif
#endif


#ifndef __CDECL
#  ifdef __PUREC__
#    define __CDECL cdecl
#  else
#    define __CDECL
#  endif
#endif

/* ein paar nicht-standard AES-Messages */
#ifndef WM_SHADED
#define WM_SHADED		0x5758
#define WM_UNSHADED		0x5759
#define WF_SHADE		0x575D
#endif
#ifndef WM_BOTTOMED
#define WM_BOTTOMED		33
#endif
#ifndef WM_ICONIFY
#define WM_ICONIFY      34          /* AES 4.1 */
#define WM_UNICONIFY    35          /* AES 4.1 */
#define WM_ALLICONIFY   36          /* AES 4.1 */
#endif
#ifndef WF_M_BACKDROP
#define WF_M_BACKDROP   100
#endif
#ifndef BACKDROP
#define BACKDROP         0x2000
#endif
#ifndef SMALLER
#define SMALLER          0x4000         /* AES 4.1     */
#endif

#ifndef WF_ICONIFY
#define WF_ICONIFY       26  /* G&S  AES 4.1 */
#define WF_UNICONIFY     27  /* G&S  AES 4.1 */
#define WF_UNICONIFYXYWH 28  /*   S  AES 4.1 */
#endif
#ifndef WF_TOOLBAR
#define WF_TOOLBAR       30  /* G&S */
#endif
#ifndef WF_MENU
#define WF_MENU          33  /* XaAES */
#endif
#ifndef WF_USER_POINTER
#define WF_USER_POINTER 230
#endif
#ifndef WF_WIND_ATTACH
#define WF_WIND_ATTACH  231
#endif

#ifndef G_SHORTCUT
#define G_SHORTCUT		38						/* MagiC 6 */
#endif

#ifndef ED_CRSR
#define ED_CRSR			100					/* MAG!X		  */
#define ED_DRAW			103					/* MAG!X 2.00  */
#endif

/* Object flags */
#ifndef OF_NONE
#  define OF_NONE 0x0000
#endif
#ifndef OF_SELECTABLE
#  define OF_SELECTABLE 0x0001
#endif
#ifndef OF_DEFAULT
#  define OF_DEFAULT 0x0002
#endif
#ifndef OF_EXIT
#  define OF_EXIT 0x0004
#endif
#ifndef OF_EDITABLE
#  define OF_EDITABLE 0x0008
#endif
#ifndef OF_RBUTTON
#  define OF_RBUTTON 0x0010
#endif
#ifndef OF_LASTOB
#  define OF_LASTOB 0x0020
#endif
#ifndef OF_TOUCHEXIT
#  define OF_TOUCHEXIT 0x0040
#endif
#ifndef OF_HIDETREE
#  define OF_HIDETREE 0x0080
#endif
#ifndef OF_INDIRECT
#  define OF_INDIRECT 0x0100
#endif
#ifndef OF_FL3DIND
#  define OF_FL3DIND 0x0200
#endif
#ifndef OF_FL3DBAK
#  define OF_FL3DBAK 0x0400
#endif
#ifndef OF_FL3DACT
#  define OF_FL3DACT 0x0600
#endif
#ifndef OF_MOVEABLE
#  define OF_MOVEABLE 0x0800
#endif
#ifndef OF_POPUP
#  define OF_POPUP 0x1000
#endif
#ifndef OF_FLAG11
#define OF_FLAG11				0x0800
#define OF_FLAG12			 	0x1000
#define OF_FLAG13		 		0x2000
#define OF_FLAG14		 		0x4000
#define OF_FLAG15				0x8000
#endif

/* Object states */
#ifndef OS_NORMAL
#  define OS_NORMAL 0x0000
#endif
#ifndef OS_SELECTED
#  define OS_SELECTED 0x0001
#endif
#ifndef OS_CROSSED
#  define OS_CROSSED 0x0002
#endif
#ifndef OS_CHECKED
#  define OS_CHECKED 0x0004
#endif
#ifndef OS_DISABLED
#  define OS_DISABLED 0x0008
#endif
#ifndef OS_OUTLINED
#  define OS_OUTLINED 0x0010
#endif
#ifndef OS_SHADOWED
#  define OS_SHADOWED 0x0020
#endif
#ifndef OS_WHITEBAK
#  define OS_WHITEBAK 0x0040
#endif
#ifndef OS_DRAW3D
#  define OS_DRAW3D 0x0080
#endif

/* Object colors - default pall. */
#ifndef G_WHITE
#define G_WHITE			0
#define G_BLACK			1
#define G_RED			2
#define G_GREEN			3
#define G_BLUE			4
#define G_CYAN			5
#define G_YELLOW		6
#define G_MAGENTA		7
#define G_LWHITE		8
#define G_LBLACK		9
#define G_LRED			10
#define G_LGREEN		11
#define G_LBLUE			12
#define G_LCYAN			13
#define G_LYELLOW		14
#define G_LMAGENTA		15
#endif

/* Sorting modes */

#ifndef SORTBYNAME
#define SORTBYNAME  0
#define SORTBYDATE  1
#define SORTBYSIZE  2
#define SORTBYTYPE  3
#define SORTBYNONE  4
#define SORTDEFAULT (-1)              /* MagiC 6.10 */
#endif

/* Flags for file selection */

#ifndef DOSMODE
#define DOSMODE     1
#define NFOLLOWSLKS 2
#define GETMULTI    8
#endif

/*******************************************************************************
 * Version
 *******************************************************************************/

/* Major and minor version number of the GEMLib package.  Use these macros 
   to test for features in specific releases.  */
#define __CFLIB__		__CFLIB_MAJOR__
#define	__CFLIB_MAJOR__		0
#define	__CFLIB_MINOR__		21
#define __CFLIB_REVISION__	0
#define __CFLIB_BETATAG__	""
extern char const __Ident_cflib[];

/*******************************************************************************
 * spezielle Typen der CF-Lib
 *******************************************************************************/

typedef enum
{
	null, Con, TCon, Datei, Terminal,
	Modem1, Modem2, Seriell1, Seriell2, Prn
} DEBUGDEV;


typedef struct popup POPUP;
struct popup
{
	OBJECT	*tree;			/* der Objektbaum */
	_WORD	max_item;		/* maximum possible count */
	_WORD	akt_item;		/* aktuelle Anzahl */
	_WORD	item_len;		/* length of an entry */
};

typedef void (*MDIAL_WCB)(_WORD *msg);

typedef struct mdial MDIAL;
struct mdial
{
	OBJECT	*tree;
	_WORD	win_handle;
	char	*win_name;
	_WORD	edit_obj;
	_WORD	edit_idx;
	_WORD	next_obj;
	_WORD	save_frame;
	_WORD	delta_y;
	_WORD	is_shaded;
	MDIAL	*next;
};


typedef struct wdialog WDIALOG;

typedef void (*WDIAL_OCB)(WDIALOG *dial);
typedef int  (*WDIAL_XCB)(WDIALOG *dial, _WORD exit_obj); /* FIXME: why int? */

#define WOCB_NULL	((WDIAL_OCB)NULL)
#define WXCB_NULL	((WDIAL_XCB)NULL)

struct wdialog
{
	WDIALOG	*next;

	OBJECT	*tree;			/* Objektbaum */
	OBJECT	*icon;			/* Icon fr Iconify */
	_WORD	mode;			/* aktueller Status */
	_WORD	win_handle;		/* Fensterhandle */
	char	win_name[80];	/* Fenstertitel */
	_WORD	win_kind;		/* Elemente */
	GRECT	work;			/* Fenstergr”že */
	_WORD	title_obj;		/* Objektnummer des Titelobjektes */
	_WORD	cancel_obj;		/*       "      des Abbruchbuttons */
	_WORD	delta_y;		/* Offset bis zum Titelobjekt */
	_WORD	edit_idx;		/* Objektnummern fr die Editfelder */
	_WORD	next_obj;
	_WORD	edit_obj;

	WDIAL_OCB open_cb;	
	WDIAL_XCB exit_cb;	
};

/*******************************************************************************
 * globals.c
 *******************************************************************************/

extern _WORD	gl_phys_handle;
extern GRECT	gl_desk;	

extern _WORD	sys_big_id, sys_big_height, sys_big_pts;
extern _WORD	sys_sml_id, sys_sml_height, sys_sml_pts;
extern _WORD	sys_wchar, sys_hchar, sys_wbox, sys_hbox;

extern _WORD	gl_wchar, gl_hchar, gl_wbox, gl_hbox;

extern _WORD	gl_gdos;
extern _WORD	gl_planes;

extern _WORD	gl_gem;
extern _WORD	gl_mint;
extern _WORD	gl_naes;
extern long	gl_xaaes;
extern _WORD	gl_magx;
extern _WORD	gl_nvdi;

extern char 	gl_appdir[];

/*******************************************************************************
 * alerts functions
 *******************************************************************************/

_WORD do_alert	(_WORD def, _WORD undo, char *str);
_WORD do_walert	(_WORD def, _WORD undo, char *str, char *win_title);

/*******************************************************************************
 * app functions
 *******************************************************************************/

extern _WORD gl_apid;

void	init_app(char *rsc);
void	exit_gem(void);
void	exit_app(int ret);
void	hide_mouse(void);
_WORD  	hide_mouse_if_needed(GRECT *rect);
void	show_mouse(void);
#ifndef appl_xgetinfo
_WORD	appl_xgetinfo(_WORD type, _WORD *out1, _WORD *out2, _WORD *out3, _WORD *out4);
#endif

/*******************************************************************************
 * asciitable functions
 *******************************************************************************/

_WORD ascii_table(_WORD id, _WORD pts);
void set_asciitable_strings(char *title, char *button);

/*******************************************************************************
 * colorpop functions
 *******************************************************************************/

#define MAX_COLORPOP 10 /* Maximale Anzahl von Farb-Objekten in einem Programm */

_WORD init_colorpop(_WORD maxplanes);
void exit_colorpop(void);

void fix_colorpopobj(OBJECT *tree, _WORD obj, _WORD color);
void set_popobjcolor(OBJECT *tree, _WORD obj, _WORD color);
_WORD get_popobjcolor(OBJECT *tree, _WORD obj);

_WORD do_colorpop(_WORD x, _WORD y, _WORD item, _WORD planes, _WORD show_noncolor);
_WORD handle_colorpop(OBJECT *dial, _WORD dial_obj, _WORD mode, _WORD planes, _WORD show_noncolor);

/*******************************************************************************
 * comm functions
 *******************************************************************************/

void send_m_special(int mode, int app); /* FIXME: why int? */
void send_vastart(int id, char *str); /* FIXME: why int? */
void send_scchanged(void);

/*******************************************************************************
 * cookie functions
 *******************************************************************************/

int getcookie(const char *cookie, long *value);

/*******************************************************************************
 * debug functions
 *******************************************************************************/

extern int gl_debug;

void debug_init(char *prgName, DEBUGDEV dev, char *file);
void debug_exit(void);
void debug(char *FormatString, ...);

/*******************************************************************************
 * dragdrop functions
 *******************************************************************************/

#ifndef DD_OK
#define DD_OK		0
#define DD_NAK		1
#define DD_EXT		2
#define DD_LEN		3
#define DD_TIMEOUT	4000
#define DD_NUMEXTS	8
#define DD_EXTSIZE	32
#define DD_NAMEMAX	128
#define DD_HDRMAX	(8+DD_NAMEMAX)
#endif

int	dd_create(int apid, int winid, int msx, int msy, int kstate, char *exts); /* FIXME: why int? */
int	dd_stry(int fd, char *ext, char *name, long size);
void dd_close(int fd);
int dd_open(int ddnam, char *ext);
int dd_rtry(int fd, char *name, char *ext, long *size);
int dd_reply(int fd, int ack);

/*******************************************************************************
 * file functions
 *******************************************************************************/

int	file_exists	(char *filename);
int	path_exists	(char *pathname);

int	get_path	(char *path, char drive);
int	set_path	(char *path);

void	split_filename(char *fullname, char *path, char *name);
void 	split_extension(char *filename, char *name, char *extension);
void 	set_extension(char *filename, char *new_ext);

int make_normalpath(char *path);
void make_shortpath	(char *path, char *shortpath, int maxlen);

#define NO_CASE		0	/* TOS-FS */
#define HALF_CASE	1	/* VFAT/Mac */
#define FULL_CASE	2	/* Minix */
int fs_long_name(char *filename);
int fs_case_sens(char *filename);

/*******************************************************************************
 * filesel functions
 *******************************************************************************/

typedef int (*FSEL_CB)(char *path, char *name); /* FIXME: why int? */
#define FSCB_NULL	((FSEL_CB)NULL)

int select_file(char *path, char *name, char *mask, char *title, FSEL_CB open_cb);

/*******************************************************************************
 * fontsel functions
 *******************************************************************************/

#define FS_M_XFSL		1		/* Cookie xFSL */
#define FS_M_FPROT		2		/* Fontprotokoll */
#define FS_M_MAGX		4		/* MagiC fnts_* */
#define FS_M_ALL		(FS_M_XFSL|FS_M_FPROT|FS_M_MAGX)

#define FS_F_MONO		8		/* nur monospaced */

_WORD do_fontsel(_WORD flags, char *title, _WORD *id, _WORD *pts);

/*******************************************************************************
 * form_do functions
 *******************************************************************************/

_WORD	cf_form_do(OBJECT *tree, _WORD *ed_start);
_WORD 	simple_dial(OBJECT *tree, _WORD start_edit);

typedef _WORD (*KEY_CB)(OBJECT *tree, _WORD edit_obj, _WORD kstate, _WORD *kreturn, _WORD *next_obj);

KEY_CB set_formdo_keycb(KEY_CB keycb);

/*******************************************************************************
 * magx functions
 *******************************************************************************/
int get_magx_version(void);

#define MX_NOTXOBJ		0		/* kein MagiC-Objekt */
#define MX_UNDERLINE	1		/* String als šberschrift */
#define MX_RADIO		2		/* Radio ohne Shortcut */
#define MX_SCRADIO		3		/* Radio mit Shortcut */
#define MX_CHECK		4		/* Check ohne Shortcut */
#define MX_SCCHECK		5		/* Check mit Shortcut */
#define MX_SCEXIT		6		/* EXIT mit Shortcut */
#define MX_SCSTRING		7		/* String mit Shortcut */
#define MX_GROUPBOX		8		/* Gruppenrahmen */
#define MX_EDIT3D		9		/* 3D-Editfeld */

#define MX_GROUPBOX2		10		/* Gruppenrahmen, kleine Schrift */

#define MX_UNKNOWN		-1		/* Unbekanntes WHITEBACK-Obj */

_WORD	get_magx_obj(OBJECT *tree, _WORD obj);
_WORD	get_magx_shortcut(OBJECT *tree, _WORD obj, char *c);

/*******************************************************************************
 * mdial functions
 *******************************************************************************/

MDIAL *open_mdial(OBJECT *tree, _WORD edit_start);
void close_mdial(MDIAL *mdial);
_WORD do_mdial(MDIAL *mdial);

_WORD simple_mdial	(OBJECT *tree, _WORD edit_start);

void set_mdial_wincb(MDIAL_WCB cb);

void redraw_mdobj(MDIAL *mdial, _WORD obj);
void change_mdedit(MDIAL *mdial, _WORD new);

/*******************************************************************************
 * menu functions
 *******************************************************************************/

_WORD create_menu(OBJECT *tree);
void delete_menu(void);

void disable_menu(void);
void enable_menu(void);

_WORD is_menu_key(_WORD kreturn, _WORD kstate, _WORD *title, _WORD *item);

/*******************************************************************************
 * misc functions
 *******************************************************************************/

long ts2ol(_WORD i1, _WORD i2);
void ol2ts(long l, _WORD *i1, _WORD *i2);

void save_background(GRECT *box, MFDB *buffer);
void restore_background(GRECT *box, MFDB *buffer);

void *malloc_global(long size);

int get_patchlev(const char *id_str, char *pl);

_WORD *grect_to_array(const GRECT *g, _WORD *pxy);
GRECT *array_to_grect(const _WORD *pxy, GRECT *g);

/*******************************************************************************
 * nkcc functions
 *******************************************************************************/

   /* NKCC key code flags */
#define NKF_FUNC     0x8000         /* function          */
#define NKF_RESVD    0x4000         /* resvd, ignore it! */
#define NKF_NUM      0x2000         /* numeric pad       */
#define NKF_CAPS     0x1000         /* CapsLock          */
#define NKF_ALT      0x0800         /* Alternate         */
#define NKF_CTRL     0x0400         /* Control           */
#define NKF_SHIFT    0x0300         /* any Shift key     */
#define NKF_LSH      0x0200         /* left Shift key    */
#define NKF_RSH      0x0100         /* right Shift key   */

   /* special key codes for keys performing a function */
#define NK_INVALID   0x00           /* invalid key code  */
#define NK_UP        0x01           /* cursor up         */
#define NK_DOWN      0x02           /* cursor down       */
#define NK_RIGHT     0x03           /* cursor right      */
#define NK_LEFT      0x04           /* cursor left       */
#define NK_M_PGUP    0x05           /* Mac: page up      */
#define NK_M_PGDOWN  0x06           /* Mac: page down    */
#define NK_M_END     0x07           /* Mac: end          */
#define NK_BS        0x08           /* Backspace         */
#define NK_TAB       0x09           /* Tab               */
#define NK_ENTER     0x0a           /* Enter             */
#define NK_INS       0x0b           /* Insert            */
#define NK_CLRHOME   0x0c           /* Clr/Home          */
#define NK_RET       0x0d           /* Return            */
#define NK_HELP      0x0e           /* Help              */
#define NK_UNDO      0x0f           /* Undo              */
#define NK_F1        0x10           /* function key #1   */
#define NK_F2        0x11           /* function key #2   */
#define NK_F3        0x12           /* function key #3   */
#define NK_F4        0x13           /* function key #4   */
#define NK_F5        0x14           /* function key #5   */
#define NK_F6        0x15           /* function key #6   */
#define NK_F7        0x16           /* function key #7   */
#define NK_F8        0x17           /* function key #8   */
#define NK_F9        0x18           /* function key #9   */
#define NK_F10       0x19           /* function key #10  */
#define NK_M_F11     0x1a           /* Mac: func key #11 */
#define NK_ESC       0x1b           /* Esc               */
#define NK_M_F12     0x1c           /* Mac: func key #12 */
#define NK_M_F14     0x1d           /* Mac: func key #14 */
#define NK_RVD1E     0x1e           /* reserved!         */
#define NK_DEL       0x1f           /* Delete            */

_WORD		nkc_init	(void);
unsigned short	nkc_tos2n	(long toskey);
long		nkc_n2tos	(long nkcode);
unsigned char	nkc_toupper	(unsigned char chr);
unsigned char	nkc_tolower	(unsigned char chr);

unsigned short	gem_to_norm	(_WORD ks, _WORD kr);
void 		norm_to_gem	(unsigned long norm, _WORD *ks, _WORD *kr);

void 		str_toupper	(char *str);
void 		str_tolower	(char *str);

/*******************************************************************************
 * objc functions
 *******************************************************************************/

void 	set_obspec	(OBJECT *tree, _WORD obj, long spec);
long 	get_obspec	(OBJECT *tree, _WORD obj);
	
_WORD 	get_obtype	(OBJECT *tree, _WORD obj, _WORD *ud);
void 	get_objframe(OBJECT *tree, _WORD obj, GRECT *r);

void 	set_flag	(OBJECT *tree, _WORD obj, _WORD flag, _WORD set);
_WORD	get_flag	(OBJECT *tree, _WORD obj, _WORD flag);
_WORD	find_flag	(OBJECT *tree, _WORD flag);

void	set_state	(OBJECT *tree, _WORD obj, _WORD state, _WORD set);
_WORD	get_state	(OBJECT *tree, _WORD obj, _WORD state);
void	tree_state	(OBJECT *tree, _WORD start_obj, _WORD state, _WORD set);

void	set_string	(OBJECT *tree, _WORD obj, char *text);
void 	get_string	(OBJECT *tree, _WORD obj, char *text);

void 	set_short	(OBJECT *tree, _WORD obj, short i);
short  	get_short	(OBJECT *tree, _WORD obj);

void 	set_long	(OBJECT *tree, _WORD obj, long l);
void 	set_ulong	(OBJECT *tree, _WORD obj, unsigned long l);
long 	get_long	(OBJECT *tree, _WORD obj);

void 	redraw_obj	(OBJECT *tree, _WORD obj);

/*******************************************************************************
 * popup functions
 *******************************************************************************/
 
_WORD	create_popup(POPUP *p, _WORD anz, _WORD maxlen, char *item);
_WORD	free_popup(POPUP *p);
_WORD	append_popup(POPUP *p, char *item);

_WORD	do_popup(POPUP *p, _WORD button);
_WORD	cf_menu_popup(MENU *m1, _WORD x, _WORD y, MENU *m2, _WORD button, _WORD offset);

#define POP_OPEN	1
#define POP_CYCLE	2
_WORD	handle_popup(OBJECT *dial, _WORD dial_obj, OBJECT *pop, _WORD pop_obj, _WORD mode);

/*******************************************************************************
 * scrap functions
 *******************************************************************************/

int get_scrapdir(char *scrap);
void scrap_clear(void);
char *scrap_rtxt(char *buf, long *len, long maxlen);
void scrap_wtxt(char *buf);

/*******************************************************************************
 * userdef functions
 *******************************************************************************/

void fix_dial(OBJECT *tree);
void fix_menu(OBJECT *tree);
void fix_popup(OBJECT *tree, _WORD thin_line);
void cf_use_all_userdefs(_WORD all);

/*******************************************************************************
 * vdi functions
 *******************************************************************************/

_WORD open_vwork(_WORD *w_out);
void set_clipping(_WORD handle, _WORD x, _WORD y, _WORD w, _WORD h, _WORD on);
_WORD height2pts(_WORD handle, _WORD f_id, _WORD f_height);
_WORD vdi_str2array(const char *src, _WORD *des);

/*******************************************************************************
 * wdial functions
 *******************************************************************************/

/* wd->mode */
#define WD_OPEN   1
#define WD_ICON   2
#define WD_SHADE  4

#define WD_CLOSER	0xFF

WDIALOG	*create_wdial(OBJECT *tree, OBJECT *icon, _WORD edit_obj, WDIAL_OCB open_cb, WDIAL_XCB exit_cb);
WDIALOG	*create_wdial_kind(OBJECT *tree, OBJECT *icon, _WORD edit_obj, WDIAL_OCB open_cb, WDIAL_XCB exit_cb, _WORD win_kind);
void delete_wdial(WDIALOG *wd);
void open_wdial(WDIALOG *wd, _WORD pos_x, _WORD pos_y);
void close_wdial(WDIALOG *wd);

void redraw_wdobj(WDIALOG *wd, _WORD obj);
void redraw_wdicon(WDIALOG *wd, _WORD obj);
void change_wdedit(WDIALOG *wd, _WORD new);

_WORD	message_wdial(_WORD *msg);
_WORD	click_wdial(_WORD clicks, _WORD x, _WORD y, _WORD kshift, _WORD mbutton);
_WORD	key_wdial(_WORD kreturn, _WORD kstate);

/*
 * functions not available in original PCGEMLIB:
 */
#if defined(__PUREC__) && !defined(_GEMLIB_COMPATIBLE)
_WORD rc_intersect(const GRECT *r1, GRECT *r2);

_WORD form_center_grect(OBJECT *tree, GRECT *r);
_WORD form_dial_grect(_WORD subfn, const GRECT *lg, const GRECT *bg);
_WORD objc_draw_grect(OBJECT *tree, _WORD start, _WORD depth, const GRECT *r);
_WORD objc_change_grect(OBJECT *tree, _WORD start, _WORD depth, const GRECT *clip, _WORD newstate, _WORD redraw);
_WORD wind_get_grect(_WORD whl, _WORD srt, GRECT *g);
_WORD wind_calc_grect(_WORD type, _WORD parts, const GRECT *in, GRECT *out);
_WORD wind_create_grect(_WORD parts, const GRECT *r);
_WORD wind_open_grect(_WORD whl, const GRECT *r);
_WORD wind_set_grect(_WORD whl, _WORD srt, const GRECT *r);
_WORD wind_set_str(_WORD whl, _WORD srt, const char *s);
_WORD objc_sysvar(_WORD ob_svmode, _WORD ob_svwhich,  /* AES 4.0 */
                 _WORD ob_svinval1, _WORD ob_svinval2,
                 _WORD *ob_svoutval1, _WORD *ob_svoutval2); /* AES 4.0 */
#endif

/*******************************************************************************
 * end
 *******************************************************************************/

#endif /* _cflib_h_ */
