// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
#include "lith_common.h"
#include "lith_player.h"

#include <math.h>

//
// Lith_GUI_ScrollBegin_Impl
//
// This function is excessively commented in case I need to change it.
// Reading this function will cause a 5x1d20 hit to your SAN stat. Beware!
//
void Lith_GUI_ScrollBegin_Impl(gui_state_t *g, id_t id, gui_scroll_args_t const *a)
{
   gui_scroll_preset_t const *pre = a->preset ? a->preset : &scrdefault;
   gui_scroll_state_t *scr = &g->st[a->st].scrl;

   // sizes
   int const blockh  = pre->scrlh;          // height of graphical block
   int const blocks  = (a->h / blockh) - 1; // height in graphical blocks -caps
   int const caph    = blockh / 2;          // size of cap
   int const caps    = blocks * 2;          // height in caps, -caps
   int const h       = blocks * blockh;     // height in pixels -caps
   int const realh   = h + (caph * 2);      // height in pixels +caps

   // positions
   int x = a->x + pre->scrlw; // base x to draw from
   int y = a->y;              // base y to draw from

   // check collision - height is minus caps, and y is offset by the top cap
   Lith_GUI_Auto(g, id, x - pre->scrlw, y + caph, pre->scrlw, h);

   // add offset
   x += g->ox;
   y += g->oy;

   // get height of scroller
   int notches; // height of scroller in caps

   if(a->contenth > realh) notches = (a->h / (double)a->contenth) * caps;
   else                    notches = caps;

   int    const scrlh = notches * caph;          // height in pixels of scroller
   double const maxy  = (h - scrlh) / (double)h; // normalized maximum y value

   // move scroller
   __with(double supposedy = scr->y * h;)
   {
      if(g->active == id)
      {
         double const cy = (g->cy - y) - caph;

         // if it isn't grabbed and the cursor is over the scroller,
         // set the grab position to where the cursor is relative to it
         if(!scr->grabbed && cy > supposedy && cy < supposedy + scrlh)
         {
            scr->grabbed = true;
            scr->grabpos = cy - supposedy;
         }

         // if the scroller is grabbed we set the position relative to where
         // we grabbed it from, otherwise we just use the middle of it
         if(scr->grabbed) supposedy = cy - scr->grabpos;
         else             supposedy = cy - (scrlh / 2);
      }
      else
         scr->grabbed = false;

      // finally, normalize and clamp
      scr->y = supposedy / (double)h;
      scr->y = minmax(scr->y, 0, maxy);
   }

   // get offset of scroller
   __with(int vofs = 0;) // offset in pixels of the content
   {
      if(a->contenth > realh)
         vofs = round((a->contenth - realh) * (scr->y / maxy));

      // set the scrollbar's offset
      scr->ox = a->x + pre->scrlw; // offset by scrollbar width
      scr->oy = a->y - vofs;       // offset by scroller pos

      // set the top and bottom for occlusion
      scr->occludeS = vofs;
      scr->occludeE = vofs + realh;
   }

   // draw top cap
   __with(__str cap = Lith_GUI_Prefix1(g, pre, capS);)
      if(cap) DrawSpritePlain(cap, g->hid--, x + 0.2, y + 0.1, TICSECOND);
   y += caph;

   // draw middle of bar
   __with(__str scrl = Lith_GUI_Prefix1(g, pre, scrl);)
   for(int i = 0; i < blocks; i++) {
      if(scrl) DrawSpritePlain(scrl, g->hid--, x + 0.2, y + 0.1, TICSECOND);
      y += blockh;
   }

   // draw bottom cap
   __with(__str cap = Lith_GUI_Prefix1(g, pre, capE);)
      if(cap) DrawSpritePlain(cap, g->hid--, x + 0.2, y + 0.1, TICSECOND);

   // get base Y
   int const ory = a->y + g->oy;

   // draw scroller
   __with(__str graphic;)
   {
      if(g->hot == id || g->active == id)
         graphic = Lith_GUI_Prefix1(g, pre, notchhot);
      else
         graphic = Lith_GUI_Prefix1(g, pre, notchgfx);

      if(graphic)
         for(int i = 0; i < notches; i++)
      {
         int const npos = round(caph + (h * scr->y) + (caph * i));
         DrawSpritePlain(graphic, g->hid--, x + 0.2, ory + npos + 0.1,
            TICSECOND);
      }
   }

   // setup offsets
   g->ox += scr->ox;
   g->oy += scr->oy;

   // setup clip
   Lith_GUI_Clip(g, x, ory, a->w, realh, a->ww);
}

//
// Lith_GUI_ScrollEnd
//
void Lith_GUI_ScrollEnd(gui_state_t *g, size_t st)
{
   gui_scroll_state_t *scr = &g->st[st].scrl;

   // reset offsets
   g->ox -= scr->ox;
   g->oy -= scr->oy;

   // reset clip
   Lith_GUI_ClipRelease(g);
}

//
// Lith_GUI_ScrollOcclude
//
bool Lith_GUI_ScrollOcclude(gui_state_t *g, size_t st, int y, int h)
{
   gui_scroll_state_t const *scr = &g->st[st].scrl;
   return y > scr->occludeE || (h && (y + h) - scr->occludeS < 0);
}

// EOF

