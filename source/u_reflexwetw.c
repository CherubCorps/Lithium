// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ ReflexWetw upgrade.                                                      │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "u_all.h"

#define udata pl.upgrdata.reflexwetw
#define CHARGE_MAX (35 * 0.8)

alloc_aut(0) script_str ext("ACS") addr(OBJ "DodgeView")
void Z_DodgeView(void) {
   k32 vh = pl.viewheight;
   for(i32 i = 0; i < 20; i++) {
      k32 mul = 1.0 - (ACS_Sin(i / 40.0) * 0.6);
      SetViewHeight(0, vh * mul);
      ACS_Delay(1);
   }
   SetViewHeight(0, vh);
}

void Upgr_ReflexWetw_Activate(void) {
   udata.charge = CHARGE_MAX;
   udata.leaped = 0;
}

void Upgr_ReflexWetw_Update(void) {
   pl.speedmul += 30;

   if(udata.leaped == 1) {
      udata.leaped = 2;
   }

   if(udata.charge < CHARGE_MAX) {
      udata.charge++;
   }

   if(pl.frozen) return;

   if(udata.charge >= CHARGE_MAX) {
      if(pl.onground) udata.leaped = 0;

      if(pl.buttons & BT_SPEED) {
         k32 angle = pl.yaw - ACS_VectorAngle(pl.forwardv, pl.sidev);

         AmbientSound(ss_player_slide, 1.0);
         P_SetVel(pl.velx + (ACS_Cos(angle) * 32.0), pl.vely + (ACS_Sin(angle) * 32.0), 0);

         Z_DodgeView();

         udata.charge = 0;
      }
   }

   if(pl.waterlevel == 0 && P_ButtonPressed(BT_JUMP) && !InvNum(so_PowerFlight) &&
      !has_status(so_StatJetBooster) && !udata.leaped &&
      (!pl.onground || udata.charge < CHARGE_MAX))
   {
      k32 angle = pl.yaw - ACS_VectorAngle(pl.forwardv, pl.sidev);

      AmbientSound(ss_player_doublejump, 1.0);
      P_SetVel(pl.velx + (ACS_Cos(angle) * 4.0), pl.vely + (ACS_Sin(angle) * 4.0), 12.0);

      udata.leaped = 1;
   }
}

script_str ext("ACS") addr(OBJ "DodgeViewShadowWalk")
void Z_DodgeViewShadowWalk(void) {
   k32 vh = pl.viewheight;

   for(i32 i = 0; i < 105; i++) {
      k32 mul = 1.0k - (ACS_Sin(i / 105.0k / 2.0k) * 0.3k) - 0.1k;
      SetViewHeight(0, vh * mul);
      ACS_Delay(1);
   }

   SetViewHeight(0, vh);
}

/* EOF */
