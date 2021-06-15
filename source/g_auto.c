/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Miscellaneous GUI functions.
 *
 * ---------------------------------------------------------------------------|
 */

#include "common.h"
#include "p_player.h"
#include "w_world.h"

/* Static Functions -------------------------------------------------------- */

static
void G_cutBox(struct gui_clip *other, i32 *x, i32 *y, i32 *w, i32 *h) {
   *x = minmax(*x, other->x, other->x + other->w);
   *y = minmax(*y, other->y, other->y + other->h);

   *w = min(*x + *w, other->x + other->w) - *x;
   *h = min(*y + *h, other->y + other->h) - *y;
}

static
void G_setClip(struct gui_state *g) {
   if(g->clip >= 0) {
      struct gui_clip *clip = &g->clips[g->clip];
      SetClipW(clip->x, clip->y, clip->w, clip->h, clip->ww);
   } else {
      ClearClip();
   }
}

/* Extern Functions -------------------------------------------------------- */

void G_Auto(struct gui_state *g, u32 id, i32 x, i32 y, i32 w, i32 h,
            bool slide) {
   x += g->ox;
   y += g->oy;

   if(g->clip >= 0) {
      G_cutBox(&g->clips[g->clip], &x, &y, &w, &h);
   }

   if(aabb_point(x, y, w, h, g->cx, g->cy)) {
      g->hot = id;

      if(g->active == 0 && g->clicklft) {
         g->active = id;
      }
   }

   /* check slide state */
   if(slide && g->slide != id && g->active == id) {
      g->slide      = id;
      g->slidetime  = 1;
      g->slidecount = 0;
   }

   #ifndef NDEBUG
   if(get_bit(dbgflags, dbgf_gui)) {
      PrintLine(x, y, x + w, y + h, 0xFF0000);
      PrintLine(x, y, x + w, y, 0xFF0000);
      PrintLine(x, y, x, y + h, 0xFF0000);
      PrintLine(x, y + h, x + w, y + h, 0xFF0000);
      PrintLine(x + w, y, x + w, y + h, 0xFF0000);
   }
   #endif
}

void G_UpdateState(struct gui_state *g) {
   /* Due to ZDoom being ZDoom, GetUserCVar with invertmouse does
    * nothing. This breaks network sync so we can only do it in
    * single-player.
    */
   bool inverted = CVarGetI(sc_player_invertmouse);

   Str(sc_invertmouse, s"sc_invertmouse");
   if(singleplayer) inverted |= CVarGetI(sc_invertmouse);

   g->old = g->cur;

   k32 xmul = CVarGetK(sc_gui_xmul);
   k32 ymul = CVarGetK(sc_gui_ymul);

                g->cx -= pl.yawv   * (800.0lk * xmul);
   if(inverted) g->cy += pl.pitchv * (800.0lk * ymul);
   else         g->cy -= pl.pitchv * (800.0lk * ymul);

   g->cx = minmax(g->cx, 0, g->w);
   g->cy = minmax(g->cy, 0, g->h);

   g->clicklft = pl.buttons & BT_ATTACK;
   g->clickrgt = pl.buttons & BT_ALTATTACK;
   g->clickany = g->clicklft || g->clickrgt;

   g->defcr = Draw_GetCr(CVarGetI(sc_gui_defcr));

   if(!g->clickany)
      g->slide = 0;
   else if(g->slidecount)
      g->slidetime++;
}

void G_Begin(struct gui_state *g, i32 w, i32 h) {
   if(!w) w = 320;
   if(!h) h = 200;

   g->hot = 0;
   g->clip = -1;

   SetSize(g->w = w, g->h = h);
}

void G_End(struct gui_state *g, enum cursor curs) {
   str cgfx;

   switch(curs) {
   default:
   case gui_curs_green:       cgfx = sp_UI_Cursor;            break;
   case gui_curs_pink:        cgfx = sp_UI_CursorPink;        break;
   case gui_curs_blue:        cgfx = sp_UI_CursorBlue;        break;
   case gui_curs_orange:      cgfx = sp_UI_CursorOrange;      break;
   case gui_curs_red:         cgfx = sp_UI_CursorRed;         break;
   case gui_curs_white:       cgfx = sp_UI_CursorWhite;       break;
   case gui_curs_outline:     cgfx = sp_UI_CursorOutline;     break;
   case gui_curs_outline2:    cgfx = sp_UI_CursorOutline2;    break;
   case gui_curs_outlineinv:  cgfx = sp_UI_CursorOutlineInv;  break;
   case gui_curs_outline2inv: cgfx = sp_UI_CursorOutline2Inv; break;
   }

   PrintSprite(cgfx, g->cx,1, g->cy,1);

   if(!g->clickany)
      g->active = 0;
}

void G_Clip(struct gui_state *g, i32 x, i32 y, i32 w, i32 h, i32 ww) {
   struct gui_clip *clip, *other;

   if(g->clip >= 0) {
      other = &g->clips[g->clip];
   } else {
      other = nil;
   }

   clip = &g->clips[++g->clip];

   clip->x = x;
   clip->y = y;
   clip->w = w;
   clip->h = h;

   if(other) {
      G_cutBox(other, &clip->x, &clip->y, &clip->w, &clip->h);
   }

   clip->ww = ww ? min(ww, clip->w) : clip->w;

   G_setClip(g);

   #ifndef NDEBUG
   if(get_bit(dbgflags, dbgf_gui)) {
      x = clip->x, y = clip->y, w = clip->w, h = clip->h;
      PrintLine(x,     y,     x + w, y + h, 0x00FFFF);
      PrintLine(x,     y,     x + w, y,     0x00FFFF);
      PrintLine(x,     y + h, x + w, y + h, 0x00FFFF);
      PrintLine(x + w, y,     x + w, y + h, 0x00FFFF);
      PrintLine(x,     y,     x,     y + h, 0x00FFFF);

      PrintLine(x, y + h / 2, x + clip->ww, y + h / 2, 0xFF00FF);
   }
   #endif
}

void G_ClipRelease(struct gui_state *g) {
   g->clip--;
   G_setClip(g);
}

void G_TypeOn(struct gui_state *g, struct gui_typ *typeon, str text) {
   typeon->txt = text;
   typeon->len = ACS_StrLen(text);
   typeon->pos = 0;
}

struct gui_typ const *G_TypeOnUpdate(struct gui_state *g,
                                     struct gui_typ *typeon) {
   i32 num = ACS_Random(2, 15);

   if((typeon->pos += num) > typeon->len)
      typeon->pos = typeon->len;

   return typeon;
}

bool G_Filler(i32 x, i32 y, u32 *fill, u32 tics, bool held) {
   if(*fill > tics) {
      *fill = 0;
      return true;
   }

   if(held) {
      *fill += 1;
   } else if(*fill && ticks % 4 == 0) {
      *fill -= 1;
   }

   PrintSprite(StrParam(":UI:Filler%i", (*fill * 8) / tics), x,1, y,0);

   return false;
}

i32 G_Tabs(struct gui_state *g, u32 *st, char const (*names)[20],
           mem_size_t num, i32 x, i32 y, i32 yp) {
   i32 xp = 0;

   for(i32 i = 0; i < num; i++) {
      if(G_Button_HId(g, xp + yp * 6, names[i], gui_p.btntab.w * xp + x,
                      gui_p.btntab.h * yp + y, i == *st, Pre(btntab))) {
         *st = i;
      }

      if(xp == 5) {
         xp = 0;
         yp++;
      } else {
         xp++;
      }
   }

   return yp;
}


/* EOF */
