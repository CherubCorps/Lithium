/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Dialogue code compiler.
 *
 * ---------------------------------------------------------------------------|
 */

#include "d_compile.h"

/* Static Functions -------------------------------------------------------- */

#ifndef NDEBUG
script static
void Disassemble(struct dlg_def const *def) {
   ACS_BeginLog();
   __nprintf("Disassembling(%p,%u,%u)...\n", def->codeV, def->codeC,
             def->codeP);

   for(u32 i = 0; i < def->codeP;) {
      __nprintf("%04X ", PRG_BEG + i);

      u32 c = Cps_GetC(def->codeV, i++);
      i = Dlg_WriteCode(def, c, i);
      ACS_PrintChar('\n');
   }
   ACS_EndLog();
}

script static
void DumpCode(struct dlg_def *def) {
   ACS_BeginLog();
   PrintChrSt("Dumping code...\n");
   Dbg_PrintMemC(def->codeV, def->codeC);
   ACS_EndLog();
}

script static
void DumpStringTable(struct dlg_def *def) {
   ACS_BeginLog();
   __nprintf("Dumping string table(%p,%u,%u)...\n", def->stabV, def->stabC,
             def->stabP);
   Dbg_PrintMemC(def->stabV, def->stabC);
   ACS_EndLog();
}

static
void PrintDbg() {
   for(u32 i = 0; i < countof(dlgdefs); i++) {
      struct dlg_def *def = &dlgdefs[i];

      if(def->codeV) {
         ACS_BeginLog();
         __nprintf("--- Script %u ---\n", i);
         ACS_EndLog();
         Disassemble(def);
         DumpCode(def);
         DumpStringTable(def);
      }
   }

   ACS_BeginLog();
   PrintChrSt("Done.");
   ACS_EndLog();
}
#endif

static
void FinishDef(struct compiler *d)
{
   if(d->def.codeV) dlgdefs[d->num] = d->def;
   fastmemset(&d->def, 0, sizeof d->def);
}

static
void Dlg_GetItem_Page(struct compiler *d, u32 num, u32 act)
{
   d->def.pages[num] = d->def.codeP;

   Dbg_Log(log_dlg, "--- page %u (%u)", num, d->def.codeP);

   Dlg_GetStmt(d); unwrap(&d->res);

   if(act != ACT_NONE) {Dlg_PushLdVA(d, act); unwrap(&d->res);}
   Dlg_PushB1(d, DCD_BRK_NP); unwrap(&d->res);
}

static
bool Dlg_GetItem(struct compiler *d, u32 act)
{
   struct token *tok = d->tb.get();

   if(faststrcmp(tok->textV, "page") == 0) {
      tok = d->tb.expc(&d->res, d->tb.get(), tok_number);
      unwrap(&d->res);

      u32 num = faststrtou32(tok->textV);
      if(num > DPAGE_NORMAL_MAX) {
         d->tb.err(&d->res, "bad page index");
         unwrap(&d->res);
      }

      Dlg_GetItem_Page(d, num, act);
      unwrap(&d->res);
      return true;
   } else if(act == ACT_TRM_WAIT) {
      if(faststrcmp(tok->textV, "failure") == 0) {
         Dlg_GetItem_Page(d, DPAGE_FAILURE, act);
         unwrap(&d->res);
         return true;
      } else if(faststrcmp(tok->textV, "finished") == 0) {
         Dlg_GetItem_Page(d, DPAGE_FINISHED, act);
         unwrap(&d->res);
         return true;
      } else if(faststrcmp(tok->textV, "unfinished") == 0) {
         Dlg_GetItem_Page(d, DPAGE_UNFINISHED, act);
         unwrap(&d->res);
         return true;
      }
   }

   d->tb.unget();
   return false;
}

static
void Dlg_GetTop_Prog(struct compiler *d, u32 act, u32 beg, u32 end)
{
   struct token *tok = d->tb.expc(&d->res, d->tb.get(), tok_number);
   unwrap(&d->res);

   u32 num = beg + faststrtou32(tok->textV);
   if(num > end) {
      d->tb.err(&d->res, "invalid dialogue number %u", num);
      unwrap(&d->res);
   }

   d->tb.expdr(&d->res, tok_semico);
   unwrap(&d->res);

   FinishDef(d);
   d->num = num;

   Dbg_Log(log_dlg, "\n---\nheading %u\n---", num);

   while(Dlg_GetItem(d, act)) {
      unwrap(&d->res);
   }
   unwrap(&d->res);
}

/* Extern Functions -------------------------------------------------------- */

u32 Dlg_WriteCode(struct dlg_def const *def, u32 c, u32 i)
{
   u32 c2, c3;

   struct dcd_info const *inf = &dcdinfo[c];

   __nprintf("%02X ", c);

   if(!inf->name[0]) {
      PrintChrSt("       invalid opcode");
      return i;
   }

   switch(inf->adrm) {
      case ADRM_AI:
         c2 = Cps_GetC(def->codeV, i++);
         c3 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X %02X %s $%02X%02X  ", c2, c3, inf->name, c3, c2);
         break;
      case ADRM_AX:
         c2 = Cps_GetC(def->codeV, i++);
         c3 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X %02X %s $%02X%02X,X", c2, c3, inf->name, c3, c2);
         break;
      case ADRM_AY:
         c2 = Cps_GetC(def->codeV, i++);
         c3 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X %02X %s $%02X%02X,Y", c2, c3, inf->name, c3, c2);
         break;
      case ADRM_II:
         c2 = Cps_GetC(def->codeV, i++);
         c3 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X %02X %s ($%02X%02X)", c2, c3, inf->name, c3, c2);
         break;
      case ADRM_IX:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s ($%02X,X) ", c2, inf->name, c2);
         break;
      case ADRM_IY:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s ($%02X),Y", c2, inf->name, c2);
         break;
      case ADRM_NP:
         __nprintf("      %s        ", inf->name);
         break;
      case ADRM_ZI:
      case ADRM_RI:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s $%02X    ", c2, inf->name, c2);
         break;
      case ADRM_VI:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s #$%02X   ", c2, inf->name, c2);
         break;
      case ADRM_ZX:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s $%02X,X  ", c2, inf->name, c2);
         break;
      case ADRM_ZY:
         c2 = Cps_GetC(def->codeV, i++);
         __nprintf("%02X    %s $%02X,Y  ", c2, inf->name, c2);
         break;
   }

   return i;
}

void Dlg_MInit(void)
{
   /* Free any previous dialogue definitions. */
   for(u32 i = 0; i < countof(dlgdefs); i++) {
      struct dlg_def *def = &dlgdefs[i];

      if(def->codeV) {
         Vec_Clear(def->code);
         Vec_Clear(def->stab);
      }

      fastmemset(def, 0, sizeof *def);
   }

   struct compiler d = {};

   FILE *fp =
      W_Open(StrParam("lfiles/Dialogue_%tS.txt", PRINTNAME_LEVEL), 't');

   if(fp) {
      TBufCtor(&d.tb, fp);

      for(;;) {
         struct token *tok = d.tb.expc2(&d.res, d.tb.get(), tok_identi, tok_eof);
         unwrap_do(&d.res, { goto done; });
         if(tok->type == tok_eof) break;

         if(faststrcmp(tok->textV, "program") == 0) {
            Dlg_GetTop_Prog(&d, ACT_NONE, DNUM_PRG_BEG, DNUM_PRG_END);
         } else if(faststrcmp(tok->textV, "dialogue") == 0) {
            Dlg_GetTop_Prog(&d, ACT_DLG_WAIT, DNUM_DLG_BEG, DNUM_DLG_END);
         } else if(faststrcmp(tok->textV, "terminal") == 0) {
            Dlg_GetTop_Prog(&d, ACT_TRM_WAIT, DNUM_TRM_BEG, DNUM_TRM_END);
         } else {
            d.tb.err(&d.res, "invalid toplevel item '%s'", tok->textV);
         }
         unwrap_do(&d.res, { goto done; });
      }

      FinishDef(&d);

   done:
      unwrap_print(&d.res);

      TBufDtor(&d.tb);
      fclose(d.tb.fp);
   }

   #ifndef NDEBUG
   if(get_bit(dbglevel, log_dlg)) PrintDbg();
   #endif
}

/* EOF */
