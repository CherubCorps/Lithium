#include "lith_common.h"
#include "lith_player.h"


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Lith_GUI_Auto
//
void Lith_GUI_Auto(gui_state_t *g, id_t id, int x, int y, int w, int h)
{
   x += g->ox;
   y += g->oy;
   
   // check clip versus cursor (if clipping), then check control versus cursor
   if(!g->useclip || bpcldi(g->clpxS, g->clpyS, g->clpxE, g->clpyE, g->cx, g->cy))
      if(bpcldi(x, y, x + w, y + h, g->cx, g->cy))
   {
      g->hot = id;
      
      if(g->active == 0 && g->clicklft)
         g->active = id;
   }
}

//
// Lith_GUI_Init
//
void Lith_GUI_Init(gui_state_t *g, size_t maxst)
{
   if(g->st) free(g->st);
   g->st = calloc(maxst, sizeof(gui_stateitem_t));
}

//
// Lith_GUI_UpdateState
//
void Lith_GUI_UpdateState(gui_state_t *g, player_t *p)
{
   bool inverted = Lith_GetPCVarInt(p, "lith_player_invertmouse");
   
   // Due to ZDoom being ZDoom, GetUserCVar with invertmouse does nothing.
   // This breaks network sync so we can only do it in singleplayer.
   if(ACS_GameType() == GAME_SINGLE_PLAYER)
      inverted |= ACS_GetCVar("invertmouse");
   
   g->old = g->cur;
   
                g->cx -= p->yawv   * 800.0f;
   if(inverted) g->cy += p->pitchv * 800.0f;
   else         g->cy -= p->pitchv * 800.0f;
   
   g->cx = minmax(g->cx, 0, g->w);
   g->cy = minmax(g->cy, 0, g->h);
   
   g->clicklft = p->buttons & BT_ATTACK;
   g->clickrgt = p->buttons & BT_ALTATTACK;
   g->clickany = g->clicklft || g->clickrgt;
}

//
// Lith_GUI_Begin
//
void Lith_GUI_Begin(gui_state_t *g, int basehid, int w, int h)
{
   if(!w) w = 320;
   if(!h) h = 200;
   
   g->hid = basehid;
   g->hot = 0;
   
   ACS_SetHudSize(g->w = w, g->h = h);
}

//
// Lith_GUI_End
//
void Lith_GUI_End(gui_state_t *g)
{
   DrawSpritePlain("lgfx/UI/Cursor.png", g->hid--, (int)g->cx + 0.1, (int)g->cy + 0.1, TICSECOND);
   
   if(!g->clickany)
      g->active = 0;
}

//
// Lith_GUI_Clip
//
void Lith_GUI_Clip(gui_state_t *g, int x, int y, int w, int h)
{
   g->useclip = true;
   g->clpxE = x + w;
   g->clpyE = y + h;
   
   ACS_SetHudClipRect(g->clpxS = x, g->clpyS = y, w, h);
}

//
// Lith_GUI_ClipRelease
//
void Lith_GUI_ClipRelease(gui_state_t *g)
{
   g->useclip = g->clpxS = g->clpyS = g->clpxE = g->clpyE = 0;
   ACS_SetHudClipRect(0, 0, 0, 0);
}

// EOF
