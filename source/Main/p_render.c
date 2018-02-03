// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
// Required for sincos(3).
#define _GNU_SOURCE
#include "lith_common.h"
#include "lith_player.h"
#include "lith_hudid.h"
#include "lith_world.h"

#include <math.h>

// Static Functions ----------------------------------------------------------|

static void HUD_StringStack(player_t *p);
static void HUD_Waves(player_t *p);

// Extern Functions ----------------------------------------------------------|

//
// Lith_PlayerDebugStats
//
[[__call("ScriptS")]]
void Lith_PlayerDebugStats(player_t *p)
{
   if(!(world.dbgLevel & log_devh)) return;

   ACS_SetHudSize(450, 350);

   ACS_SetFont("LTRMFONT");
   ACS_BeginPrint();
   for(int i = 0; i < dbgstatnum; i++)
      ACS_PrintString(dbgstat[i]);
   for(int i = 0; i < dbgnotenum; i++)
      ACS_PrintString(dbgnote[i]);
   ACS_MoreHudMessage();
   HudMessagePlain(hid_debug, 10.1, 20.1, TICSECOND);
}

//
// Lith_PlayerFootstep
//
[[__call("ScriptS")]]
void Lith_PlayerFootstep(player_t *p)
{
   if(Lith_IsPaused) return;

   fixed vol = p->getCVarK("lith_player_footstepvol");
   if(!vol || ACS_Timer() % 10 != 0 || p->z - p->floorz > 16) return;

   fixed dstmul = absk(p->getVel()) / 24.0;
   if(!(vol *= min(dstmul, 1))) return;

   ACS_PlaySound(p->cameratid, p->stepnoise, CHAN_BODY, vol);
}

//
// Lith_PlayerItemFx
//
[[__call("ScriptS")]]
void Lith_PlayerItemFx(player_t *p)
{
   bool hasir = ACS_CheckInventory("PowerLightAmp");

   if(!hasir && p->hadinfrared)
      ACS_LocalAmbientSound("player/infraredoff", 127);

   p->hadinfrared = hasir;
}

//
// Lith_PlayerDamageBob
//
// Update view bobbing when you get damaged.
//
[[__call("ScriptS")]]
void Lith_PlayerDamageBob(player_t *p)
{
   if(Lith_IsPaused) return;

   if(!ACS_CheckInventory("PowerStrength") && p->health < p->oldhealth)
   {
      float angle = RandomFloat(tau, -tau);
      float distance;

      if(p->bobyaw + p->bobpitch > 0.05)
         angle = lerpf(angle, atan2f(p->bobpitch, p->bobyaw), 0.25f);

      distance  = mag2f(p->bobyaw, p->bobpitch);
      distance += (p->oldhealth - p->health) / (float)p->maxhealth;
      distance *= 0.2f;

      float ys, yc;
      sincosf(angle, &ys, &yc);
      p->bobyaw   = ys * distance;
      p->bobpitch = yc * distance;
   }

   p->bobpitch = lerpf(p->bobpitch, 0.0f, 0.1f);
   p->bobyaw   = lerpf(p->bobyaw,   0.0f, 0.1f);
}

//
// Lith_PlayerView
//
// Update additive view.
//
[[__call("ScriptS")]]
void Lith_PlayerView(player_t *p)
{
   if(Lith_IsPaused) return;

   float addp = 0, addy = 0;

   if(p->getCVarI("lith_player_damagebob"))
   {
      float bobmul = p->getCVarK("lith_player_damagebobmul");
      addp += p->bobpitch * bobmul;
      addy += p->bobyaw   * bobmul;
   }

   p->extrpitch = lerpf(p->extrpitch, 0.0f, 0.1f);
   p->extryaw   = lerpf(p->extryaw,   0.0f, 0.1f);

   p->addpitch = addp + p->extrpitch;
   p->addyaw   = addy + p->extryaw;

   ifauto(fixed, mul, p->getCVarK("lith_player_viewtilt") * 0.2) {
      if(p->sidev) p->addroll = lerpf(p->addroll, -p->sidev * mul, 0.05);
      else         p->addroll = lerpf(p->addroll, 0,               0.10);
   }
}

//
// Lith_PlayerStyle
//
[[__call("ScriptS")]]
void Lith_PlayerStyle(player_t *p)
{
   if(p->scopetoken) {
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Subtract);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, p->getCVarK("lith_weapons_scopealpha") * p->alpha);
   } else {
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Translucent);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, p->getCVarK("lith_weapons_alpha") * p->alpha);
   }
}

//
// Lith_PlayerHUD
//
[[__call("ScriptS")]]
void Lith_PlayerHUD(player_t *p)
{
   ACS_SetHudSize(320, 200);

   if(p->old.scopetoken && !p->scopetoken)
   {
      p->hudstrlist.free(free);

      for(int i = hid_scope_clearS; i <= hid_scope_clearE; i++)
      {
         HudMessage("");
         HudMessagePlain(i, 0.0, 0.0, 0.0);
      }
   }

   if(p->scopetoken)
   {
      if(p->pclass == pcl_cybermage)
      {
         int time = (ACS_Timer() % 16) / 4;
         DrawSpriteXX(StrParam("lgfx/HUD_C/ScopeOverlay%i.png", time + 1),
            HUDMSG_ADDBLEND|HUDMSG_FADEOUT|HUDMSG_ALPHA, hid_scope_overlayE + time, 0.1, 0.1, 0.1, 0.25, 0.5);

         for(int i = 0; i < 200; i++)
            DrawSpriteXX("lgfx/HUD/H_D41.png", HUDMSG_ADDBLEND|HUDMSG_FADEOUT|HUDMSG_ALPHA,
               hid_scope_lineE + i, 32, i+.1, 0.1, 0.1, ACS_RandomFixed(0.3, 0.6));
      }
      else if(p->pclass == pcl_informant)
      {
         fixed a = (1 + ACS_Sin(ACS_Timer() / 70.0)) * 0.25 + 0.5;
         DrawSpriteX("lgfx/HUD_I/ScopeOverlay.png",
            HUDMSG_ADDBLEND|HUDMSG_ALPHA, hid_scope_overlayE, -2.1, -2.1,
            TICSECOND, a);
      }
      else
      {
         HUD_Waves(p);
         HUD_StringStack(p);
      }
   }
}

//
// Lith_PlayerLevelup
//
[[__call("ScriptS")]]
void Lith_PlayerLevelup(player_t *p)
{
   if(p->old.attr.level && p->old.attr.level < p->attr.level) {
      ACS_LocalAmbientSound("player/levelup", 127);
      p->logH(Language("LITH_TXT_LOG_LevelUp%S", p->discrim), p->attr.level);
   }
}

// Static Functions ----------------------------------------------------------|

//
// HUD_StringStack
//
static void HUD_StringStack(player_t *p)
{
   typedef struct hudstr_s
   {
      __str str;
      list_t link;
   } hudstr_t;

   if((ACS_Timer() % 3) == 0)
   {
      hudstr_t *hudstr = salloc(hudstr_t);
      hudstr->link.construct(hudstr);
      hudstr->str = StrParam("%.8X", ACS_Random(0, 0x7FFFFFFF));

      hudstr->link.link(&p->hudstrlist);

      if(p->hudstrlist.size == 20)
         free(p->hudstrlist.next->unlink());
   }

   ACS_SetHudSize(320, 200);
   ACS_SetFont("CONFONT");

   size_t i = 0;
   for(list_t *rover = p->hudstrlist.prev; rover != &p->hudstrlist; rover = rover->prev, i++)
   {
      hudstr_t *hudstr = rover->object;
      HudMessage("%S", hudstr->str);
      HudMessageParams(HUDMSG_ALPHA | HUDMSG_ADDBLEND, hid_scope_stringstackS - i, CR_RED, 300.2, 20.1 + (i * 9), 0.0, 0.5);
   }
}

//
// HUD_Waves
//
static void HUD_Waves(player_t *p)
{
   fixed health = (fixed)p->health / (fixed)p->maxhealth;
   int frame = minmax(health * 4, 1, 5);
   int timer = ACS_Timer();
   int pos;

   ACS_SetHudSize(320, 200);

   // Sine (health)
   pos = (10 + timer) % 160;
   DrawSpriteFade(StrParam("lgfx/HUD/H_D1%i.png", frame),
      hid_scope_sineS - pos,
      300.1 + roundk(ACS_Sin(pos / 32.0) * 7.0, 0),
      25.1 + pos,
      1.5, 0.3);

   // Square
   {
      fixed a = ACS_Cos(pos / 32.0);

      pos = (7 + timer) % 160;
      DrawSpriteFade(roundk(a, 2) != 0.0 ? "lgfx/HUD/H_D16.png" : "lgfx/HUD/H_D46.png",
         hid_scope_squareS - pos,
         300.1 + (a >= 0) * 7.0,
         25.1 + pos,
         1.9, 0.1);
   }

   // Triangle
   pos = (5 + timer) % 160;
   DrawSpriteFade("lgfx/HUD/H_D14.png", hid_scope_triS - pos, 300.1 + abs((pos % 16) - 8), 25.1 + pos, 1.2, 0.2);
}

// EOF

