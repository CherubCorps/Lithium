// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Various functions used by powerups.                                      │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "m_engine.h"
#include "p_player.h"
#include "p_hudid.h"
#include "w_world.h"

alloc_aut(0) script_str ext("ACS") addr(OBJ "DOGS")
void Z_DOGS(void) {
   i32 tid = ACS_UniqueTID();

   ACS_SetMusic(sp_lmusic_DOGS);

   for(i32 i = 0; i < (35 * 30) / 10; i++) {
      k32 ang = ACS_RandomFixed(0, 1);
      k32 dst = ACS_RandomFixed(0, 64);
      ACS_SpawnForced(so_Steggles, pl.x + ACS_Cos(ang) * dst, pl.y + ACS_Sin(ang) * dst, pl.z + 8, tid);
      ServCallV(sm_AlertMonsters);
      ACS_Delay(10);
   }

   ACS_Delay(35);
   ACS_SetMusic(sp_DSEMPTY);
   ACS_SetActorState(tid, sm_PureSteggleEnergy);
}

script_str ext("ACS") addr(OBJ "BarrierBullets")
void Z_BarrierBullets(void) {
   if(PtrPlayerNumber(0, AAPTR_TARGET) >= 0) {
      k32 ang   = ACS_VectorAngle(GetX(0) - pl.x, GetY(0) - pl.y);
      k32 xang  = ACS_VectorAngle(pl.x - GetX(0), pl.y - GetY(0));
      k32 zdiff = pl.z - GetZ(0);
      k32 s     = ACS_Sin(ang) * 48.0;
      k32 c     = ACS_Cos(ang) * 48.0;
      k32 z     = (pl.z + pl.viewheight / 2) - (zdiff / 2);

      ACS_SpawnForced(so_BarrierFX, pl.x + c, pl.y + s, z);
      ACS_LineAttack(pl.tid, ang + ACS_RandomFixed(-0.1, 0.1), PITCH_BASE + ACS_RandomFixed(0.45, 0.55), 10);
   }
}

dynam_aut script_str ext("ACS") addr(OBJ "GetSigil")
void Z_GetSigil(void) {
   P_GUI_Close();

   P_LogF("*** WARNING - CAUSAL SHIFT DETECTED IN WETWARE ***");
   pl.sgacquired = true;

#ifndef NDEBUG
   if(dbglevel_any()) return;
#endif

   StartSound(ss_player_getsigil, lch_body3, 0, 1.0, ATTN_NONE);

   FreezeTime();

   ACS_FadeTo(0, 0, 0, 0.4, 0.087);

   ACS_Delay(3);

   SetFade(fid_divsigil, 44, 8);
   for(i32 i = 0;; i++) {
      if(!CheckFade(fid_divsigil)) break;

      SetSize(640, 480);
      PrintTextF_str(ns(lang(sl_div_get)), sf_bigupper, CR_ORANGE, 320,4, 240,2, fid_divsigil);
      PrintTextF_str(ns(lang(sl_div_warning)), sf_bigupper, CR_RED, 320,4, 240,1, fid_divsigil);

      ACS_Delay(1);
   }

   ACS_FadeTo(0, 0, 0, 0.0, 0.145);

   UnfreezeTime();
}

script_str ext("ACS") addr(OBJ "GiveHealthBonus")
void Z_GiveHealthBonus(i32 amount) {
   amount += pl.health;
   if(amount > pl.maxhealth + 100) amount = pl.maxhealth + 100;
   pl.setHealth(amount);
}

script_str ext("ACS") addr(OBJ "GiveHealth")
void Z_GiveHealth(i32 amount) {
   amount += pl.health;
   amount *= 1 + pl.attr.attrs[at_vit] / 80.0;
   if(amount > pl.maxhealth) amount = pl.maxhealth;
   pl.setHealth(amount);
}

script_str ext("ACS") addr(OBJ "CheckHealth")
bool Z_CheckHealth(void) {
   return pl.health < pl.maxhealth;
}

/* EOF */
