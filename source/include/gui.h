/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * GUI functions and data.
 *
 * ---------------------------------------------------------------------------|
 */

#ifndef gui_h
#define gui_h

#include "m_cps.h"

#define Pre(name) .preset = &gui_p.name

/* Fixed ID */
#define G_Args(ty, ...) &(struct gui_arg_##ty const){__VA_ARGS__}
#define G_Button_FId(g, id, ...) G_Button_Imp(g, id, G_Args(btn, __VA_ARGS__))
#define G_ChkBox_FId(g, id, ...) G_ChkBox_Imp(g, id, G_Args(cbx, __VA_ARGS__))
#define G_ScrBeg_FId(g, id, ...) G_ScrBeg_Imp(g, id, G_Args(scr, __VA_ARGS__))
#define G_Slider_FId(g, id, ...) G_Slider_Imp(g, id, G_Args(sld, __VA_ARGS__))
#define G_TxtBox_FId(g, id, ...) G_TxtBox_Imp(g, id, G_Args(txt, __VA_ARGS__))
#define G_WinBeg_FId(g, id, ...) G_WinBeg_Imp(g, id, G_Args(win, __VA_ARGS__))

/* Hashed ID */
#define G_HId(id) (id) + LineHash
#define G_Button_HId(g, id, ...) G_Button_FId(g, G_HId(id), __VA_ARGS__)
#define G_ChkBox_HId(g, id, ...) G_ChkBox_FId(g, G_HId(id), __VA_ARGS__)
#define G_ScrBeg_HId(g, id, ...) G_ScrBeg_FId(g, G_HId(id), __VA_ARGS__)
#define G_Slider_HId(g, id, ...) G_Slider_FId(g, G_HId(id), __VA_ARGS__)
#define G_TxtBox_HId(g, id, ...) G_TxtBox_FId(g, G_HId(id), __VA_ARGS__)
#define G_WinBeg_HId(g, id, ...) G_WinBeg_FId(g, G_HId(id), __VA_ARGS__)

/* Hashed */
#define G_Button(g, ...) G_Button_HId(g, 0, __VA_ARGS__)
#define G_ChkBox(g, ...) G_ChkBox_HId(g, 0, __VA_ARGS__)
#define G_ScrBeg(g, ...) G_ScrBeg_HId(g, 0, __VA_ARGS__)
#define G_Slider(g, ...) G_Slider_HId(g, 0, __VA_ARGS__)
#define G_TxtBox(g, ...) G_TxtBox_HId(g, 0, __VA_ARGS__)
#define G_WinBeg(g, ...) G_WinBeg_HId(g, 0, __VA_ARGS__)

#define G_GenPreset(type, def) \
   type pre; \
   if(a->preset) pre = *a->preset; \
   else          pre = def

#define G_Prefix(g, gfx, pre, mem) \
   do { \
           if(!(pre)->mem)     (gfx)[0] = '\0'; \
      else if((pre)->external) strcpy(gfx, (pre)->mem); \
      else                     lstrcpy2(gfx, (g)->gfxprefix, (pre)->mem); \
   } while(0)

#define G_ScrollReset(g, st) \
   (*(st) = (struct gui_scr){})

#define G_TxtBoxRes(st) ((st)->tbptr = 0)

#define G_TxtBoxEvt(st) \
   __with(cstr txt_buf = Cps_Expand(st->txtbuf, 0, st->tbptr);) \
      ifauto(cstr, _c, strchr(txt_buf, '\n')) \
         __with(size_t txt_len = _c - txt_buf; G_TxtBoxRes(st);)

/* Types ------------------------------------------------------------------- */

struct gui_fil {
   u32 *ptr;
   u32  tic;
};

struct gui_scr {
   i32  ox, oy;
   i32  occludeS;
   i32  occludeE;
   k64  y;
   k64  grabpos;
   bool grabbed;
   i32  nextsnd;
};

struct gui_typ {
   str txt;
   i32 len;
   i32 pos;
};

struct gui_txt {
   Cps_Decl(txtbuf, 128);
   i32 tbptr;
};

struct gui_win {
   bool init, grabbed;
   i32 x, y;
   i32 ox, oy;
   i32 nextsnd;
};

struct gui_delta {
   k64 cx, cy;

   bool clicklft;
   bool clickrgt;
   bool clickany;
};

struct gui_clip {
   i32 x, y, w, h, ww;
};

struct gui_state {
   anonymous
   struct gui_delta cur;
   struct gui_delta old;

   i32 defcr;

   i32 ox, oy;
   i32 w, h;

   u32 active, hot;

   u32 slide;
   i32 slidecount, slidetime;

   u32 dbl;
   i32 dbltime;

   i32 clip;
   struct gui_clip clips[16];

   void *state;

   cstr gfxprefix;
};

struct gui_pre_btn {
   cstr gfx;
   cstr hot;
   cstr cdef;
   cstr cact;
   cstr chot;
   cstr cdis;
   str  font;
   str  snd;
   i32  ax;
   i32  ay;
   i32  w;
   i32  h;
   bool external;
};

struct gui_arg_btn {
   cstr label;
   i32  x, y;
   bool disabled;
   cstr color;
   bool slide;
   struct gui_fil const fill;
   struct gui_pre_btn const *preset;
};

struct gui_pre_cbx {
   cstr gfx;
   cstr hot;
   cstr dis;
   cstr chkgfx;
   cstr chkhot;
   cstr chkact;
   cstr chkdis;
   str  sndup;
   str  snddn;
   bool external;
   i32  w;
   i32  h;
   i32  cx;
   i32  cy;
};

struct gui_arg_cbx {
   bool on;
   i32  x, y;
   bool disabled;
   struct gui_pre_cbx const *preset;
};

struct gui_pre_scr {
   cstr capS;
   cstr capE;
   cstr scrl;
   cstr notchgfx;
   cstr notchhot;
   bool external;
   i32  scrlw;
   i32  scrlh;
};

struct gui_arg_scr {
   struct gui_scr *st;
   i32 x;
   i32 y;
   i32 w;
   i32 h;
   i32 contenth;
   i32 ww;
   struct gui_pre_scr const *preset;
};

struct gui_pre_sld {
   cstr gfx;
   cstr notch;
   cstr notchhot;
   str  snd;
   str  font;
   bool external;
   i32  pad;
   i32  w;
   i32  h;
};

struct gui_arg_sld {
   i32  x;
   i32  y;
   k64  minima;
   k64  maxima;
   k64  val;
   bool integ;
   cstr suf;
   struct gui_pre_sld const *preset;
};

struct gui_arg_txt {
   struct gui_txt *st;
   i32 x, y;
   struct player *p;
};

struct gui_pre_win {
   cstr bg;
   i32  w;
   i32  h;
   k32  a;
   i32  bx;
   i32  by;
   bool external;
};

struct gui_arg_win {
   struct gui_win *st;
   struct gui_pre_win const *preset;
};

struct gui_presets {
   struct gui_pre_btn btndef;
   struct gui_pre_btn btntab;
   struct gui_pre_btn btnexit;
   struct gui_pre_btn btnnext;
   struct gui_pre_btn btnprev;
   struct gui_pre_btn btnnexts;
   struct gui_pre_btn btnprevs;
   struct gui_pre_btn btnlist;
   struct gui_pre_btn btnlistsel;
   struct gui_pre_btn btnlistactive;
   struct gui_pre_btn btnlistactivated;
   struct gui_pre_btn btnbipmain;
   struct gui_pre_btn btnbipback;
   struct gui_pre_btn btndlgsel;
   struct gui_pre_btn btnclear;
   struct gui_pre_btn btnnote;
   struct gui_pre_cbx cbxdef;
   struct gui_pre_cbx cbxsmall;
   struct gui_pre_scr scrdef;
   struct gui_pre_sld slddef;
   struct gui_pre_win windef;
};

enum cursor {
   gui_curs_green,
   gui_curs_pink,
   gui_curs_blue,
   gui_curs_orange,
   gui_curs_red,
   gui_curs_white,
   gui_curs_outline,
   gui_curs_outline2,
   gui_curs_outlineinv,
   gui_curs_outline2inv,
   gui_curs_max,
};

/* Extern Objects ---------------------------------------------------------- */

extern struct gui_presets const gui_p;

/* Extern Functions -------------------------------------------------------- */

optargs(1)
void G_Auto(struct gui_state *g, u32 id, i32 x, i32 y, i32 w, i32 h, bool slide);

optargs(1)
void G_Init(struct gui_state *g, void *state);

void G_UpdateState(struct gui_state *g, struct player *p);

optargs(2)
void G_Begin(struct gui_state *g, i32 w, i32 h);
void G_End(struct gui_state *g, enum cursor curs);

optargs(1)
void G_Clip(struct gui_state *g, i32 x, i32 y, i32 w, i32 h, i32 ww);
void G_ClipRelease(struct gui_state *g);

void G_TypeOn(struct gui_state *g, struct gui_typ *typeon, str text);
struct gui_typ const *G_TypeOnUpdate(struct gui_state *g, struct gui_typ *typeon);

bool G_Filler(i32 x, i32 y, u32 *fill, u32 tics, bool held);

i32 G_Tabs(struct gui_state *g, u32 *st, char const (*names)[20], size_t num,
           i32 x, i32 y, i32 yp);

void G_ScrEnd(struct gui_state *g, struct gui_scr *scr);
optargs(1)
bool G_ScrOcc(struct gui_state *g, struct gui_scr const *scr, i32 y, i32 h);

void G_WinEnd(struct gui_state *g, struct gui_win *win);

#define G_ImpArgs(ty) struct gui_state *g, u32 id, struct gui_arg_##ty const *a
bool            G_Button_Imp(G_ImpArgs(btn));
bool            G_ChkBox_Imp(G_ImpArgs(cbx));
void            G_ScrBeg_Imp(G_ImpArgs(scr));
k64             G_Slider_Imp(G_ImpArgs(sld));
struct gui_txt *G_TxtBox_Imp(G_ImpArgs(txt));
void            G_WinBeg_Imp(G_ImpArgs(win));

#endif
