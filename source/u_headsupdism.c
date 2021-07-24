/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * HeadsUpDisM upgrade.
 *
 * ---------------------------------------------------------------------------|
 */

#include "u_common.h"
#include "p_hud.h"

#define UData pl.upgrdata.headsupdism

/* Static Functions -------------------------------------------------------- */

static
void HUD_Ammo() {
   struct invweapon const *wep = pl.weapon.cur;

   str typebg;
   i32 y;

   if(CVarGetI(sc_hud_showweapons)) {y = 13; typebg = sp_HUD_M_SplitRight;}
   else                             {y = 0;  typebg = sp_HUD_M_SplitFront;}

   if(wep->ammotype & AT_NMag || wep->ammotype & AT_Ammo) {
      PrintSprite(sp_HUD_M_BarBig, 278,2, 239-y,2);
      PrintSprite(typebg, 319,2, 239,2);
   }

   cstr type = nil;

   if(wep->ammotype & AT_NMag) {
      type = "MAG";

      char txt[16];
      if(wep->ammotype & AT_Ammo && !wep->ammocur)
         faststrcpy(txt, "OUT");
      else
         sprintf(txt, "%i/%i", wep->magmax - wep->magcur, wep->magmax);
      PrintTextChS(txt);
      PrintTextX(sf_bigupper, Cr(green), 217,1, 238-y,2, _u_no_unicode);
   }

   if(wep->ammotype & AT_Ammo) {
      type = "AMMO";

      i32 x = 0;

      if(wep->ammotype & AT_NMag) {
         PrintSprite(sp_HUD_M_BarBig, 214,2, 239-y,2);
         x = 64;
      }

      ACS_BeginPrint();
      ACS_PrintInt(wep->ammocur);
      PrintTextX(sf_bigupper, Cr(green), 217-x,1, 238-y,2, _u_no_unicode);
   }

   if(type) {
      PrintTextChS(type);
      PrintTextX(sf_bigupper, Cr(green), 281,1, 238,2, _u_no_unicode);
   }

   if(P_Wep_CurType() == weapon_m_rifle && ServCallI(sm_GetRifleGrenade))
      PrintSprite(sp_HUD_H_D44, 280,1, 237,1);
}

static
void HUD_Health(struct upgrade *upgr) {
   PrintSprite(InvNum(so_PowerStrength) ? sp_HUD_M_SplitBackRed : sp_HUD_M_SplitBack, 0,1, 240,2);

   PrintTextChr("VIT", 3);
   PrintTextX(sf_bigupper, Cr(green), 2,1, 238,2, _u_no_unicode);

   k32 ft = 0;

   if(pl.health < pl.oldhealth) {
      UData.cr = CR_PURPLE;
      ft = (pl.oldhealth - pl.health) / 300.0;
      ft = minmax(ft, 0.1, 1.0);
   } else if(pl.health > pl.oldhealth) {
      UData.cr = CR_YELLOW;
      ft = 0.1;
   }

   if(ft) SetFade(fid_health, 4, ft * 255);

   HUD_DrawHealth(pl.health, 34, 232, Cr(green), UData.cr);

   str gfx = sa_wgfx[pl.weapon.cur->info->slot];

   i32 x = (8 + pl.ticks) % 57;

   for(i32 i = 0; i < 20; i++) {
      i32 xx = x - i;
      if(xx < 0) xx += 57;

      i32 y = 9;
      if(xx < 11) y += 11 - xx % 12;

      PrintSpriteA(gfx, 88-xx,1, 215+y,1, (20 - i) / 20.0);
   }
}

/* Extern Functions -------------------------------------------------------- */

void Upgr_HeadsUpDisM_Render(struct upgrade *upgr) {
   if(!pl.hudenabled) return;

   HUD_Log(Cr(green), 0, 0);

   HUD_Score("%s\Cnscr", pl.score, sf_smallfnt, CR_WHITE);

   if(CVarGetI(sc_hud_showweapons))
      PrintSprite(sp_HUD_M_Bar, 278,2, 239,2);

   HUD_WeaponSlots(Cr(wselm1), Cr(wselm2), Cr(wselm3), Cr(wselms), 281, 238);

   /* Status */
   HUD_Ammo();
   HUD_Health(upgr);
}

/* EOF */
