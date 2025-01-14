// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Miscellaneous GUI functions.                                             │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "m_engine.h"
#include "p_player.h"
#include "w_world.h"

alloc_aut(0) stkcall static
void G_cutBox(struct gui_clip const *other, i32 *x, i32 *y, i32 *w, i32 *h) {
   *x = clampi(*x, other->x, other->x + other->w);
   *y = clampi(*y, other->y, other->y + other->h);

   *w = mini(*x + *w, other->x + other->w) - *x;
   *h = mini(*y + *h, other->y + other->h) - *y;
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

void G_Auto(struct gui_state *g, gid_t id, i32 x, i32 y, i32 w, i32 h,
            bool slide) {
   x += g->ox;
   y += g->oy;

   if(g->clip >= 0) {
      G_cutBox(&g->clips[g->clip], &x, &y, &w, &h);
   }

   if(aabb_point(x, y, w - 1, h - 1, g->cx, g->cy)) {
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
   if(dbgflags(dbgf_gui)) {
      PrintLine(x, y, x + w, y + h, 0xFFFF0000);
      PrintLine(x, y, x + w, y, 0xFFFF0000);
      PrintLine(x, y, x, y + h, 0xFFFF0000);
      PrintLine(x, y + h, x + w, y + h, 0xFFFF0000);
      PrintLine(x + w, y, x + w, y + h, 0xFFFF0000);
   }
   #endif
}

void G_UpdateState(struct gui_state *g) {
   bool inverted = CVarGetI(sc_invertmouse);
   k32  curspeed = CVarGetK(sc_gui_curspeed) * 800.0k;
   g->old = g->cur;
                g->cx -= pl.yawv   * curspeed * (g->w / (k32)g->h);
   if(inverted) g->cy += pl.pitchv * curspeed;
   else         g->cy -= pl.pitchv * curspeed;
   g->cx = clamplk(g->cx, 0, g->w);
   g->cy = clamplk(g->cy, 0, g->h);
   g->clicklft = pl.buttons & BT_ATTACK;
   g->clickrgt = pl.buttons & BT_ALTATTACK;
   g->clickany = g->clicklft || g->clickrgt;
   g->defcr = Draw_GetCr(CVarGetI(sc_gui_defcr));
   if(!g->clickany) {
      g->slide = 0;
   } else if(g->slidecount) {
      g->slidetime++;
   }
}

void G_Begin(struct gui_state *g, i32 w, i32 h) {
   if(!w) w = 320;
   if(!h) h = 200;

   g->hot     = 0;
   g->clip    = -1;
   g->tooltip = snil;

   SetSize(g->w = w, g->h = h);
}

void G_End(struct gui_state *g, i32 curs) {
   if(g->tooltip) {
      noinit static struct i32v2 p, s;
      i32 ww;
      p.x = g->cx + 7;
      p.y = g->cy + 2;
      ww = g->w - p.x;
      TextSize(&s, g->tooltip, sf_smallfnt, ww);
      if(s.x < 100) {
         ww = g->cx - 2;
         TextSize(&s, g->tooltip, sf_smallfnt, ww);
         p.x = ww - s.x;
      }
      PrintRect(p.x-1, p.y-1, s.x+2, s.y+2, 0xFFBA8CC6);
      PrintRect(p.x,   p.y,   s.x,   s.y,   0xFF1A141D);
      SetClipW(p.x, p.y, s.x, s.y, ww);
      PrintText_str(g->tooltip, sf_smallfnt, CR_WHITE, p.x,1, p.y,1);
      ClearClip();
   }

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

   if(!g->clickany) {
      g->active = 0;
   }
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

   clip->ww = ww ? mini(ww, clip->w) : clip->w;

   G_setClip(g);

   #ifndef NDEBUG
   if(dbgflags(dbgf_gui)) {
      x = clip->x, y = clip->y, w = clip->w, h = clip->h;
      PrintLine(x,     y,     x + w, y + h, 0xFF00FFFF);
      PrintLine(x,     y,     x + w, y,     0xFF00FFFF);
      PrintLine(x,     y + h, x + w, y + h, 0xFF00FFFF);
      PrintLine(x + w, y,     x + w, y + h, 0xFF00FFFF);
      PrintLine(x,     y,     x,     y + h, 0xFF00FFFF);

      PrintLine(x, y + h / 2, x + clip->ww, y + h / 2, 0xFFFF00FF);
   }
   #endif
}

void G_ClipRelease(struct gui_state *g) {
   g->clip--;
   G_setClip(g);
}

alloc_aut(0) stkcall
bool G_Filler(i32 x, i32 y, struct gui_fil *fil, bool held) {
   if(fil->cur > fil->tic) {
      fil->cur = 0;
      return true;
   }

   if(held) {
      ++fil->cur;
   } else if(fil->cur && ACS_Timer() & 3 == 0) {
      --fil->cur;
   }

   PrintSprite(sa_filler[(fil->cur * 8) / fil->tic], x,1, y,0);

   return false;
}

stkcall
void G_Tooltip(struct gui_state *g, i32 x, i32 y, i32 w, i32 h, cstr id) {
   x += g->ox;
   y += g->oy;

   if(g->clip >= 0) {
      G_cutBox(&g->clips[g->clip], &x, &y, &w, &h);
   }

   if(aabb_point(x, y, w - 1, h - 1, g->cx, g->cy)) {
      str tt = lang(strp(PrintChrLi(LANG "TOOLTIP_"), PrintChrSt(id)));
      if(tt) {
         g->tooltip = tt;
      }
   }
}

i32 G_Tabs(struct gui_state *g, mem_size_t *st, gtab_t const *names, mem_size_t num, i32 x, i32 y, i32 yp) {
   i32 xp = 0;

   for(mem_size_t i = 0; i < num; i++) {
      if(G_Button_HId(g, xp + yp * 6, names[i], gui_p.btntab.w * xp + x,
                      gui_p.btntab.h * yp + y, i == *st, Pre(btntab))) {
         *st = i;
      }

      if(i != num - 1) {
         if(xp == 5) {
            xp = 0;
            yp++;
         } else {
            xp++;
         }
      }
   }

   return yp;
}


/* EOF */
