// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Log UI and logging functions.                                            │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "m_engine.h"
#include "p_log.h"
#include "p_player.h"
#include "m_list.h"
#include "p_hudid.h"
#include "w_world.h"

struct logmap {
   str name;
   i32 lnum;
   str       *dataV;
   mem_size_t dataC;
};

struct logdat {
   str fdta;
   i32 time;
   i32 ftim;
};

struct loginfo
{
   struct logdat hudV[7];
   mem_size_t    hudC;

   struct logmap *mapsV;
   mem_size_t     mapsC;

   struct logmap *curmap;
   i32            curtime;
};

static
struct loginfo log;

static
void LogV(i32 levl) {
   ACS_BeginPrint();

   if(levl) {
      for(i32 i = 0; i < levl; i++) ACS_PrintChar('>');
      ACS_PrintChar(' ');
   }
}

static
void LogPop(void) {
   log.hudC--;
   fastmemmove(&log.hudV[0], &log.hudV[1], sizeof log.hudV[0] * log.hudC);
}

static
void LogH(struct logdat *ld) {
   ld->ftim = 5;
   ld->time = 140 - log.curtime;
   log.curtime = 140;

   if(log.hudC >= countof(log.hudV)) LogPop();
   log.hudV[log.hudC++] = *ld;
}

static
void LogF(str lf) {
   log.curmap->dataV = Talloc(log.curmap->dataV, log.curmap->dataC + 1, _tag_logs);
   log.curmap->dataV[log.curmap->dataC++] = lf;
}

void P_Log_Both(i32 levl, cstr fmt, ...) {
   struct logdat ld = {};

   LogV(levl);

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   ld.fdta = ACS_EndStrParam();

   LogF(ld.fdta);
   LogH(&ld);
}

void P_Log_HUDs(i32 levl, cstr fmt, ...) {
   struct logdat ld = {};

   LogV(levl);

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   ld.fdta = ACS_EndStrParam();

   LogH(&ld);
}

void P_Log_Full(cstr fmt, ...) {
   ACS_BeginPrint();

   va_list vl;
   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   LogF(ACS_EndStrParam());
}

void P_Log_Entry(void) {
   struct logmap *lm = nil;
   i32 lnum = MapNum;

   for(i32 i = 0; i < log.mapsC; i++) {
      if(log.mapsV[i].lnum == lnum) {
         lm = &log.mapsV[i];
         break;
      }
   }

   if(!lm) {
      log.mapsV = Talloc(log.mapsV, log.mapsC + 1, _tag_logs);
      lm = &log.mapsV[log.mapsC++];
      lm->name = ml.name;
      lm->lnum = lnum;
   }

   log.curmap = lm;

   pl.logF(tmpstr(lang(sl_enter_fmt)), lm->name, CanonTime(ct_full, ACS_Timer()));
}

script void P_Log_PTick(void) {
   if(log.curtime == 0) {
      if(log.hudC) {
         LogPop();
         log.curtime = log.hudV[log.hudC - 1].time;
      }
   } else {
      log.curtime--;
   }

   for(i32 i = 0; i < log.hudC; i++) {
      log.hudV[i].ftim--;
   }
}

void P_CBI_TabLog(struct gui_state *g) {
   static
   i32 const ht = 10;

   i32 logsel_last = pl.cbi.st.logsel;

   if(G_Button(g, .x = 12, 25, Pre(btnprev))) {
      if(--pl.cbi.st.logsel < 0) {
         pl.cbi.st.logsel = log.mapsC - 1;
      }
   }

   if(G_Button(g, .x = 12 + gui_p.btnprev.w, 25, Pre(btnnext))) {
      if(++pl.cbi.st.logsel >= log.mapsC) {
         pl.cbi.st.logsel = 0;
      }
   }

   if(pl.cbi.st.logsel != logsel_last) {
      G_ScrollReset(g, &pl.cbi.st.logscr);
   }

   struct logmap *lm = &log.mapsV[pl.cbi.st.logsel];

   PrintText_str(lm->name, sf_lmidfont, g->defcr, g->ox+15+gui_p.btnprev.w+gui_p.btnnext.w,1, g->oy+27,1);

   G_ScrBeg(g, &pl.cbi.st.logscr, 2, 37, 280, 175, lm->dataC * ht);

   for(i32 i = 0; i < lm->dataC; i++) {
      i32 const y = ht * i;

      if(G_ScrOcc(g, &pl.cbi.st.logscr, y, ht)) continue;

      PrintSprite(sp_UI_LogList, g->ox,1, y + g->oy,1);
      PrintText_str(lm->dataV[i], sf_smallfnt, CR_GREEN, g->ox + 2,1, y + g->oy + 1,1);
   }

   G_ScrEnd(g, &pl.cbi.st.logscr);
}

script void P_Log(i32 cr, i32 x, i32 yy) {
   if(CVarGetI(sc_hud_showlog)) {
      bool log_from_top = CVarGetI(sc_hud_logfromtop);
      k32 scale = CVarGetK(sc_hud_logsize);
      i32 yo = 200 / scale;
      i32 xs = 320 / scale;
      i32 ys = 240 / scale;

      SetSize(xs, ys);
      SetClipW(0, 0, xs, ys, xs);

      i32 i = 0;
      for(i32 i = 0; i < log.hudC; i++) {
         struct logdat const *const ld = &log.hudV[i];

         i32 y = 10 * i;
         i32 ya;

         if(log_from_top) {ya = 1; y = 64 + y;}
         else             {ya = 2; y = (yo - y) + yy;}

         k32 a = 1.0;
         if(ld->ftim > 0) {
            SetFade(fid_logadS + i, 1, 8);
         } else if(ld->ftim < -129) {
            a = (ld->ftim + 129) / 10.0 + 1.0;
         }

         PrintTextA_str(ld->fdta, sf_lmidfont, cr, x,1, y,ya, a);

         if(CheckFade(fid_logadS + i)) {
            cstr s = RemoveTextColors_str(ld->fdta, ACS_StrLen(ld->fdta));
            PrintTextChS(s);
            PrintTextFX(sf_lmidfont, CR_WHITE, x,1, y,ya, fid_logadS + i, _u_add);
         }
      }

      SetSize(320, 240);
   }
}

script_str ext("ACS") addr(OBJ "LogS")
void Z_Log(i32 levl, i32 type) {
   if(P_None()) return;

   str name = EDataS(_edt_logname);

   if(name[0] == '_') {
      ACS_BeginPrint();
      PrintChrLi(LANG "LOG");
      ACS_PrintString(name);
      name = lang(ACS_EndStrParam());
   }

   switch(type) {
   case msg_scri:
      if(CVarGetI(sc_player_itemdisp) & _itm_disp_log) {
      case msg_huds:
         pl.logH(levl, tmpstr(name));
      }
      break;
   case msg_full:
      pl.logF(tmpstr(name));
      break;
   case msg_item:
      if(CVarGetI(sc_player_itemdisp) & _itm_disp_log) {
         pl.logB(levl, tmpstr(name));
      }
      break;
   }
}

/* EOF */
