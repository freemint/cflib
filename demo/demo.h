/*
 * resource set indices for demo
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        154
 * Number of Bitblks:        1
 * Number of Iconblks:       1
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       11
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        172
 * Number of Trees:          8
 * Number of Userblks:       0
 * Number of Images:         3
 * Total file size:          6656
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "demo"
#endif
#undef RSC_ID
#ifdef demo
#define RSC_ID demo
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 154
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 3
#define NUM_BB 1
#define NUM_FRIMG 0
#define NUM_IB 1
#define NUM_CIB 0
#define NUM_TI 11
#define NUM_OBS 172
#define NUM_TREE 8
#endif



#define MENUTREE           0 /* menu */
#define TDESK              3 /* TITLE in tree MENUTREE */
#define TDATEI             4 /* TITLE in tree MENUTREE */
#define TDIALS             5 /* TITLE in tree MENUTREE */
#define TSCUT              7 /* TITLE in tree MENUTREE */
#define MABOUT            10 /* STRING in tree MENUTREE */
#define MFSEL             19 /* STRING in tree MENUTREE */
#define MENDE             21 /* STRING in tree MENUTREE */
#define MDIAL1            23 /* STRING in tree MENUTREE */
#define MDIAL2            24 /* STRING in tree MENUTREE */
#define MALERT            26 /* STRING in tree MENUTREE */
#define MFONTSEL          28 /* STRING in tree MENUTREE */
#define MPDLG             30 /* STRING in tree MENUTREE */
#define MASCII            32 /* STRING in tree MENUTREE */
#define MMODAL            34 /* STRING in tree MENUTREE */
#define MFENSTER          35 /* STRING in tree MENUTREE */

#define ABOUT              1 /* form/dialog */
#define A_PL               2 /* STRING in tree ABOUT */

#define OBJDIAL            2 /* form/dialog */
#define TITEL              3 /* STRING in tree OBJDIAL */
#define RADIO1             4 /* BUTTON in tree OBJDIAL */
#define RADIO2             5 /* BUTTON in tree OBJDIAL */
#define SC_STRING          6 /* STRING in tree OBJDIAL */
#define CHECKBUT           7 /* BUTTON in tree OBJDIAL */
#define EXITBUT            8 /* BUTTON in tree OBJDIAL */
#define GRUPPE2           10 /* BUTTON in tree OBJDIAL */
#define TITEL2            11 /* STRING in tree OBJDIAL */
#define HELP              12 /* BOXTEXT in tree OBJDIAL */ /* max len 5 */
#define UNDO              13 /* BOXTEXT in tree OBJDIAL */ /* max len 4 */
#define EDIT1             14 /* FTEXT in tree OBJDIAL */ /* max len 8 */
#define EDIT2             15 /* FTEXT in tree OBJDIAL */ /* max len 8 */
#define ENDE              16 /* BUTTON in tree OBJDIAL */
#define EDIT3             17 /* FTEXT in tree OBJDIAL */ /* max len 8 */

#define POPDIAL            3 /* form/dialog */
#define P1STR              2 /* STRING in tree POPDIAL */
#define P1TEXT             3 /* FTEXT in tree POPDIAL */ /* max len 20 */
#define P1BOX              4 /* BOXCHAR in tree POPDIAL */
#define P2STR              5 /* STRING in tree POPDIAL */
#define P2BOX              6 /* BOXTEXT in tree POPDIAL */ /* max len 4 */
#define P3STR              7 /* STRING in tree POPDIAL */
#define P3COL              8 /* BOX in tree POPDIAL */
#define P3BOX              9 /* BOXTEXT in tree POPDIAL */ /* max len 10 */
#define P4BOX             11 /* STRING in tree POPDIAL */
#define P5STR             12 /* STRING in tree POPDIAL */
#define P5BOX             13 /* BOX in tree POPDIAL */

#define PRN_SUB            4 /* form/dialog */
#define PS_WPNAME          2 /* STRING in tree PRN_SUB */
#define PS_WPSEL           3 /* BUTTON in tree PRN_SUB */
#define PS_WPSL            5 /* FTEXT in tree PRN_SUB */ /* max len 3 */
#define PS_WPNLQ           6 /* BUTTON in tree PRN_SUB */
#define PS_WPZL            8 /* FTEXT in tree PRN_SUB */ /* max len 3 */
#define PS_WPDSTR          9 /* STRING in tree PRN_SUB */
#define PS_WPDPOP         10 /* BOXTEXT in tree PRN_SUB */ /* max len 10 */
#define PS_GFNAME         12 /* STRING in tree PRN_SUB */
#define PS_GFSEL          13 /* BUTTON in tree PRN_SUB */
#define PS_GFPTS          14 /* STRING in tree PRN_SUB */

#define POPUPS             5 /* form/dialog */
#define DICHTEPOP          1 /* BOX in tree POPUPS */
#define DICHTEPICA         2 /* STRING in tree POPUPS */
#define DICHTEELITE        3 /* STRING in tree POPUPS */
#define DICHTECOND         4 /* STRING in tree POPUPS */
#define FUNCPOP            5 /* BOX in tree POPUPS */
#define POPF1              6 /* STRING in tree POPUPS */
#define COLPOP            26 /* BOX in tree POPUPS */
#define CPWHITE           43 /* STRING in tree POPUPS */
#define POPUP4            59 /* BOX in tree POPUPS */

#define WINICON            6 /* form/dialog */
#define WIBOX              0 /* BOX in tree WINICON */
#define WIBILD             1 /* IMAGE in tree WINICON */

#define PDLGICON           7 /* form/dialog */
#define PRN_ICON           1 /* ICON in tree PDLGICON */ /* max len 12 */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD demo_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD demo_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD demo_rsc_free(void);
#endif
