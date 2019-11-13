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

# ifndef _magx_h
# define _magx_h

/*
 * Der MagX-Cookie
 */

#ifndef _DOSVARS
#define _DOSVARS
typedef struct
{
	char		*in_dos;				/* Address of DOS flags       */
	short		*dos_time;				/* Address of DOS time        */
	short		*dos_date;				/* Address of DOS date        */
	long		dos_stack;				/* NULL since Mag!X 2.00      */
	long		pgm_superst;			/* user pgm. super stk        */
	void        **memlist;				/* address of 3 MD lists      */
	void		*act_pd;				/* Running program            */
	void		*fcbx;					/* files                      */
	short		fcbn;					/* length of fcbx[]           */
	void		*dmdx;					/* DMDs                       */
	void		*imbx;					/* Internal DOS-memory list   */
	void		(*resv_intmem)(void);	/* Extend DOS memory          */
	long      __CDECL (*etv_critic)(short err);         /* etv_critic of GEMDOS      */
	char *	((*err_to_str)(signed char e));	/* Conversion code->plaintext */
	void		*xaes_appls;
	void		*mem_root;
	void		*ur_pd;
} DOSVARS;
#endif


#ifndef _AESVARS
#define _AESVARS
typedef struct
{
	long	magic;							/* Must be $87654321               */
	void	*membot;						/* End of the AES-variables        */
	void	*aes_start;						/* Start address                   */
	long	magic2;							/* Is 'MAGX'                       */
	long	date;							/* Creation date ddmmyyyy          */
    void    (*chgres)(int res, int txt);    /* Change resolution               */
    long    (**shel_vector)(void);          /* Resident desktop                */
	char    *aes_bootdrv;                   /* Booting took place from here    */
	short *vdi_device;                      /* VDI-driver used by AES          */
	void	**nvdi_workstation;				/* workstation used by AES         */
	short	*shelw_doex;					/* last <doex> for APP #0          */
	short	*shelw_isgr;					/* last <isgr> for APP #0          */
	short	version;						/* e.g. $0201 is V2.1              */
	short	release;						/* 0=alpha..3=release              */
	void	*_basepage;						/* basepage of AES                 */
	short	*moff_cnt;						/* global mouse off counter        */
	unsigned long shel_buf_len;				/* length of the shell buffer      */
	void	*shel_buf;						/* pointer to shell buffer         */
	void	**notready_list;				/* waiting applications            */
	void	**menu_app;						/* application owning the menu     */
	void	**menutree;						/* active menu tree                */
	void	**desktree;						/* active desktop background       */
	short	*desk_1stob;					/*   its first object              */
	void	*dos_magic;
	void	*windowinfo;
	short	(**fsel)(char *path, char *name, short *button, const char *title);
	long (*ctrl_timeslice) (long settings);
	void	**topwind_app;					/* app. of topmost window          */
	void	**mouse_app;					/* app. owning the mouse           */
	void	**keyb_app;						/* app. owning the keyboard        */
	long	dummy;
} AESVARS;
#endif


#ifndef _MAGX_COOKIE
#define _MAGX_COOKIE
typedef struct
{
	long		config_status;
	DOSVARS	*dosvars;
	AESVARS	*aesvars;
	void		*res1;
	void		*hddrv_functions;
    long         status_bits;               /* MagiC 3 from 24.5.95 on         */
} MAGX_COOKIE;
#endif


# endif /* _magx_h */
