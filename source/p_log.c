/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Log UI and logging functions.
 *
 * ---------------------------------------------------------------------------|
 */

#include "common.h"
#include "p_log.h"
#include "p_player.h"
#include "m_list.h"
#include "p_hudid.h"
#include "w_world.h"

/* Static Functions -------------------------------------------------------- */

static void LogV(i32 levl) {
   ACS_BeginPrint();

   if(levl) {
      for(i32 i = 0; i < levl; i++) ACS_PrintChar('>');
      ACS_PrintChar(' ');
   }
}

static void LogPop(struct player *p) {
   p->log.hudC--;
   fastmemmove(&p->log.hudV[0], &p->log.hudV[1], sizeof p->log.hudV[0] * p->log.hudC);
}

static void LogH(struct player *p, struct logdat *ld) {
   ld->ftim = 5;
   ld->time = 140 - p->log.curtime;
   p->log.curtime = 140;

   if(p->log.hudC >= countof(p->log.hudV)) LogPop(p);
   p->log.hudV[p->log.hudC++] = *ld;
}

static void LogF(struct player *p, struct logfdt *lf) {
   Vec_GrowN(p->log.curmap->data, 1, 8, _tag_logs);
   Vec_Next(p->log.curmap->data) = *lf;
}

/* Extern Functions -------------------------------------------------------- */

void P_Log_Both(struct player *p, i32 levl, cstr fmt, ...) {
   struct logdat ld = {};

   LogV(levl);

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   ld.inf = ACS_EndStrParam();

   LogF(p, &ld.fdta);
   LogH(p, &ld);
}

void P_Log_HUDs(struct player *p, i32 levl, cstr fmt, ...) {
   struct logdat ld = {};

   LogV(levl);

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   ld.inf = ACS_EndStrParam();

   LogH(p, &ld);
}

void P_Log_Full(struct player *p, cstr fmt, ...) {
   struct logfdt lf = {};

   ACS_BeginPrint();

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   lf.inf = ACS_EndStrParam();

   LogF(p, &lf);
}

void P_Log_Entry(struct player *p) {
   struct logmap *lm = nil;
   i32 lnum = MapNum;

   for(i32 i = 0; i < p->log.mapsC; i++)
      if(p->log.mapsV[i].lnum == lnum) {lm = &p->log.mapsV[i]; break;}

   if(!lm)
   {
      Vec_GrowN(p->log.maps, 1, 32, _tag_logs);
      lm = &Vec_Next(p->log.maps);
      lm->name = (ACS_BeginPrint(), ACS_PrintName(PRINTNAME_LEVELNAME), ACS_EndStrParam());
      lm->lnum = lnum;
   }

   p->log.curmap = lm;

   p->logF(LC(LANG "ENTER_FMT"), lm->name, CanonTime(ct_full, ticks));
}

script void P_Log_PTick(struct player *p) {
   if(p->log.curtime == 0) {
      if(p->log.hudC) {
         LogPop(p);
         p->log.curtime = p->log.hudV[p->log.hudC - 1].time;
      }
   } else {
      p->log.curtime--;
   }

   for(i32 i = 0; i < p->log.hudC; i++) {
      if(p->log.hudV[i].ftim) {
         p->log.hudV[i].ftim--;
      }
   }
}

void P_CBI_TabLog(struct gui_state *g, struct player *p) {
   static i32 const ht = 10;

   if(G_Button(g, .x = 12, 25, Pre(btnprev)))
      if(--CBIState(g)->logsel < 0) CBIState(g)->logsel = p->log.mapsC - 1;

   if(G_Button(g, .x = 12 + gui_p.btnprev.w, 25, Pre(btnnext)))
      if(++CBIState(g)->logsel >= p->log.mapsC) CBIState(g)->logsel = 0;

   struct logmap *lm = &p->log.mapsV[CBIState(g)->logsel];

   PrintText_str(lm->name, sf_lmidfont, g->defcr, g->ox+15+gui_p.btnprev.w+gui_p.btnnext.w,1, g->oy+27,1);

   G_ScrBeg(g, &CBIState(g)->logscr, 2, 37, 280, 175, lm->dataC * ht);

   for(i32 i = 0; i < lm->dataC; i++) {
      i32 const y = ht * i;

      if(G_ScrOcc(g, &CBIState(g)->logscr, y, ht)) continue;

      PrintSprite(sp_UI_LogList, g->ox,1, y + g->oy,1);
      PrintText_str(lm->dataV[i].inf, sf_smallfnt, CR_GREEN, g->ox + 2,1, y + g->oy + 1,1);
   }

   G_ScrEnd(g, &CBIState(g)->logscr);
}

script void HUD_Log(struct player *p, i32 cr, i32 x, i32 yy) {
   if(p->getCVarI(sc_hud_showlog)) {
      k32 scale = p->getCVarK(sc_hud_logsize);
      i32 yo = 200 / scale;
      i32 xs = 320 / scale;
      i32 ys = 240 / scale;

      SetSize(xs, ys);
      SetClipW(0, 0, xs, ys, xs);

      i32 i = 0;
      for(i32 i = 0; i < p->log.hudC; i++) {
         struct logdat const *const ld = &p->log.hudV[i];

         i32 y = 10 * i;
         i32 ya;

         if(p->getCVarI(sc_hud_logfromtop)) {ya = 1; y = 20 + y;}
         else                               {ya = 2; y = (yo - y) + yy;}

         PrintText_str(ld->inf, sf_lmidfont, cr, x,1, y,ya);

         if(ld->ftim) SetFade(fid_logadS + i, 1, 8);

         if(CheckFade(fid_logadS + i)) {
            cstr s = RemoveTextColors_str(ld->inf, ACS_StrLen(ld->inf));
            PrintTextChS(s);
            PrintTextFX(sf_lmidfont, CR_WHITE, x,1, y,ya, fid_logadS + i, ptf_add);
         }
      }

      SetSize(320, 240);
   }
}

/* Scripts ----------------------------------------------------------------- */

script_str ext("ACS") addr(OBJ "LogS")
void Sc_Log(i32 levl, i32 type) {
   str name = ServCallS(sm_GetLogName);

   if(name[0] == '_') name = Language(LANG "LOG%S", name);

   with_player(LocalPlayer) switch(type) {
   case msg_ammo: if(p->getCVarI(sc_player_ammolog))
   case msg_huds: p->logH(levl, "%S", name); break;
   case msg_full: p->logF(      "%S", name); break;
   case msg_both: p->logB(levl, "%S", name); break;
   }
}

/* EOF */
