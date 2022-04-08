// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Dialogue compiler utilities.                                             │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "d_compile.h"

void Dlg_PushB1(struct compiler *d, u32 b) {
   mem_size_t pc = d->def.codeP++;

   if(pc + 1 > PRG_END - PRG_BEG) {
      d->tb.err(&d->res, "PRG segment overflow");
      unwrap(&d->res);
   }

   if(pc + 1 > d->def.codeC * 4) {
      Vec_Grow(d->def.code, 1, _tag_dlgs);
      d->def.codeC++;
   }

   if(b > 0xFF) {
      d->tb.err(&d->res, "byte error (overflow) %u", b);
      unwrap(&d->res);
   }

   Cps_SetC(d->def.codeV, pc, b);
}

void Dlg_PushB2(struct compiler *d, u32 word) {
   Dlg_PushB1(d, word & 0xFF); unwrap(&d->res);
   Dlg_PushB1(d, word >> 8); unwrap(&d->res);
}

void Dlg_PushLdVA(struct compiler *d, u32 action) {
   Dlg_PushB1(d, DCD_LDV_VI); unwrap(&d->res);
   Dlg_PushB1(d, action); unwrap(&d->res);
}

struct ptr2 Dlg_PushLdAdr(struct compiler *d, u32 at, u32 set) {
   struct ptr2 adr;

   Dlg_PushB1(d, DCD_LDA_VI); unwrap(&d->res);
   Dlg_PushB1(d, set & 0xFF); unwrap(&d->res);
   adr.l = d->def.codeP - 1;

   Dlg_PushB1(d, DCD_STA_AI); unwrap(&d->res);
   Dlg_PushB2(d, at); unwrap(&d->res);

   Dlg_PushB1(d, DCD_LDA_VI); unwrap(&d->res);
   Dlg_PushB1(d, set >> 8); unwrap(&d->res);
   adr.h = d->def.codeP - 1;

   Dlg_PushB1(d, DCD_STA_AI); unwrap(&d->res);
   Dlg_PushB2(d, at + 1); unwrap(&d->res);

   return adr;
}

void Dlg_SetB1(struct compiler *d, u32 ptr, u32 b) {
   if(b > 0xFF) {
      d->tb.err(&d->res, "byte error (overflow) %u", b);
      unwrap(&d->res);
   }

   Cps_SetC(d->def.codeV, ptr, b);
}

void Dlg_SetB2(struct compiler *d, u32 ptr, u32 word) {
   Dlg_SetB1(d, ptr + 0, word & 0xFF); unwrap(&d->res);
   Dlg_SetB1(d, ptr + 1, word >> 8); unwrap(&d->res);
}

u32 Dlg_PushStr(struct compiler *d, cstr s, u32 l) {
   u32  p = d->def.stabP;
   u32 vl = Cps_Size(p + l) - d->def.stabC;

   if(p + l > STR_END - STR_BEG) {
      d->tb.err(&d->res, "STR segment overflow");
      unwrap(&d->res);
   }

   Dbg_Log(log_dlg,
           _l(__func__), _l(": ("), _p(l), _c(' '), _p(vl), _l(") '"), _p(s),
           _c('\''));

   Vec_Grow(d->def.stab, vl, _tag_dlgs);
   d->def.stabC += vl;
   d->def.stabP += l;

   for(u32 i = 0; i < l; i++) Cps_SetC(d->def.stabV, p + i, s[i]);

   return STR_BEG + p;
}

/* EOF */
