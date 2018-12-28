// Copyright © 2016-2017 Alison Sanderson, all rights reserved.
#include "common.h"
#include "p_player.h"

k64 Lith_GUI_Slider_Impl(struct gui_state *g, u32 id, struct gui_arg_sld const *a)
{
   struct gui_pre_sld const *pre = a->preset ? a->preset : &gui_p.slddef;

   k64 w = pre->w - (pre->pad * 2);

   i32 x = a->x + pre->pad;
   i32 y = a->y;

   Lith_GUI_Auto(g, id, x, y, w, pre->h);

   x += g->ox;
   y += g->oy;

   // get a normalized value
   k64 aval;

   aval = (a->val - a->minima) / (a->maxima - a->minima);
   aval = minmax(aval, 0, 1);

   k64 val;

   // move scroll notch
   if(g->active == id)
   {
      val  = g->cx - x;
      val  = minmax(val, 0, w);
      val /= w;

      // play sound
      if(pre->snd && g->cx != g->old.cx && g->cx >= x && g->cx < x + w)
         ACS_LocalAmbientSound(pre->snd, 60);
   }
   else
      val = aval;

   // get result-normalized value
   k64 norm = val * (a->maxima - a->minima) + a->minima;

   if(a->integ) norm = roundlk(norm, 10);

   // draw graphic
   ifauto(str, gfx, Lith_GUI_Prefix1(g, pre, gfx))
      PrintSprite(gfx, x - pre->pad,1, y + pre->h / 2,0);

   // draw notch
   __with(str graphic;)
   {
      if(g->hot == id || g->active == id)
         graphic = Lith_GUI_Prefix1(g, pre, notchhot);
      else
         graphic = Lith_GUI_Prefix1(g, pre, notch);

      if(graphic)
         PrintSprite(graphic, x + val * w - 1,1, y,1);
   }

   // draw value
   if(pre->font)
   {
      char const *suf = a->suf ? a->suf : "";
      k64 amt = roundlk(norm * 100.lk, 10) / 100.lk;
      if(a->integ) PrintTextFmt("%i%s",     (i32)amt, suf);
      else         PrintTextFmt("%.1k%s", (k32)amt, suf);

      PrintText(pre->font, CR_WHITE, x + pre->w/2,4, y + pre->h/2,0);
   }

   // if we've moved it, we return a difference
   if(g->active == id && !g->clicklft && !CloseEnough(aval, val))
      return norm - a->val;
   else
      return 0;
}

// EOF
