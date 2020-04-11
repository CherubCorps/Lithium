/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Generalized HUD functions.
 *
 * ---------------------------------------------------------------------------|
 */

#include "p_player.h"
#include "p_hud.h"
#include "p_hudid.h"

/* Extern Functions -------------------------------------------------------- */

void HUD_WeaponSlots(struct player *p, i32 cr_one, i32 cr_two, i32 cr_many, i32 cr_cur, i32 _x, i32 _y) {
   if(p->getCVarI(sc_hud_showweapons))
      for(i32 i = 1; i < SLOT_MAX; i++)
         ifauto(i32, slot, p->weapon.slot[i])
   {
      k32 x = _x+.2 - 8 * (SLOT_MAX - i);
      k32 y = _y+.2;
      i32 cr;
      switch(slot) {
      case 1:  cr = cr_one;  break;
      case 2:  cr = cr_two;  break;
      default: cr = cr_many; break;
      }

      ACS_BeginPrint();
      ACS_PrintInt(i);
      PrintTextX(s_lhudfontsmall, cr, x,2, y,2, ptf_no_utf);

      if(p->weapon.cur->info->slot == i)
         SetFade(fid_slotnS + i, 1, 6);

      if(CheckFade(fid_slotnS + i)) {
         ACS_BeginPrint();
         ACS_PrintInt(i);
         PrintTextFX(s_lhudfontsmall, cr_cur, x,2, y,2, fid_slotnS + i, ptf_no_utf);
      }
   }
}

void HUD_Score(struct player *p, cstr fmt, i96 scrn, str font, i32 cr, i32 x, i32 xa) {
   if(p->getCVarI(sc_hud_showscore)) {
      char scr[64];
      sprintf(scr, fmt, scoresep(scrn));

      PrintTextChS(scr);
      PrintTextX(font, cr, x,xa, 3,1, ptf_no_utf);

      if(p->score > p->old.score) {
         SetFade(fid_schit1, 4, 12);
      } else if(p->score < p->old.score) {
         i96 scrdif = p->old.score - p->score;
         u32 tics = scrdif * 8 / 0xFFFF;
         u32 mmx = 30000 - minmax(scrdif / 4, 3276, 29490);
         SetFade(fid_schit2, minmax(tics, 1, 35), mmx >> 8);
      }

      if(p->scoreaccumtime > 0) {
         SetFade(fid_scacum, 5, 12);
         p->scoreaccumstr = StrParam("%c%s", p->scoreaccum >= 0 ? '+' : ' ', scoresep(p->scoreaccum));
      }

      if(CheckFade(fid_schit1)) {
         PrintTextChS(scr);
         PrintTextFX(font, CR_ORANGE, x,xa, 3,1, fid_schit1, ptf_no_utf);
      } else if(CheckFade(fid_schit2)) {
         PrintTextChS(scr);
         PrintTextFX(font, CR_PURPLE, x,xa, 3,1, fid_schit2, ptf_no_utf);
      }

      if(CheckFade(fid_scacum))
         PrintTextFX_str(p->scoreaccumstr, font, CR_WHITE, x,xa, 10,1, fid_scacum, ptf_no_utf);
   }

   if(p->getCVarI(sc_hud_showlvl)) {
      PrintTextFmt("Lv.%u", p->attr.level);
      if(p->attr.points) __nprintf(" (\Cn%u\C- pts)", p->attr.points);
      PrintTextX(font, cr, x,xa, 17,1, ptf_no_utf);
   }

   i32 expbar = p->getCVarI(sc_hud_expbar);
   if(expbar > 0) {
      Str(exp_bar_0, s":UI:ExpBar0");
      PrintSprite(exp_bar_0, x,xa, 24,1);
      i32 xx = x;
      k32 fr =
         (p->attr.exp - p->attr.expprev) /
         (k32)(p->attr.expnext - p->attr.expprev);
      switch(xa) {
         case 0: xx -= 12; break;
         case 2: xx -= 24; break;
      }
      SetClip(xx, 24, 24 * fr, 2);
      PrintSprite(StrParam(":UI:ExpBar%i", expbar), xx,1, 24,1);
      ClearClip();
   }
}

void HUD_KeyInd(struct player *p, i32 x, i32 y, bool horz, k32 a) {
   #define Inc (horz ? (x -= 10) : (y += 10))
   if(p->krs) PrintSpriteA(sp_HUD_H_KS1, x,2, y,1, a), Inc;
   if(p->kys) PrintSpriteA(sp_HUD_H_KS2, x,2, y,1, a), Inc;
   if(p->kbs) PrintSpriteA(sp_HUD_H_KS3, x,2, y,1, a), Inc;
   if(p->krc) PrintSpriteA(sp_HUD_H_KC1, x,2, y,1, a), Inc;
   if(p->kyc) PrintSpriteA(sp_HUD_H_KC2, x,2, y,1, a), Inc;
   if(p->kbc) PrintSpriteA(sp_HUD_H_KC3, x,2, y,1, a), Inc;
   #undef Inc
}

void HUD_DrawHealth(struct player *p, i32 health, i32 x, i32 y, i32 cr, i32 cr_fade) {
   i32 protect = p->megaProtect;

   ACS_BeginPrint();
   ACS_PrintInt(health);
   PrintTextX(s_lhudfont, cr, x,1, y,0, ptf_no_utf);

   if(protect) {
      k32 amt = protect / 200.0;
      ACS_BeginPrint();
      ACS_PrintInt(health);
      PrintTextAX(s_lhudfont, CR_TAN, x,1, y,0, amt, ptf_no_utf);
   }

   if(CheckFade(fid_health)) {
      ACS_BeginPrint();
      ACS_PrintInt(health);
      PrintTextFX(s_lhudfont, cr_fade, x,1, y,0, fid_health, ptf_no_utf);
   }
}

/* EOF */
