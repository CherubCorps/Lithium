/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Debugging functions.
 *
 * ---------------------------------------------------------------------------|
 */

#ifndef NDEBUG
#include "common.h"
#include "p_player.h"
#include "w_monster.h"
#include "m_char.h"

#include <stdio.h>
#include <GDCC.h>

/* Static Objects ---------------------------------------------------------- */

StrAry(fonts,
       s"areaname", s"bigupper", s"jiskan16", s"k6x8", s"ljtrmfont",
       s"lmidfont", s"ltrmfont", s"misakig", s"misakim", s"smallfnt");

/* Extern Objects ---------------------------------------------------------- */

str dbgstat[64], dbgnote[64];
i32 dbgstatnum,  dbgnotenum;

/* Extern Functions -------------------------------------------------------- */

void Dbg_Stat_Impl(cstr fmt, ...)
{
   if(!get_bit(dbglevel, log_devh)) return;

   va_list vl;

   ACS_BeginPrint();

   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   dbgstat[dbgstatnum++] = ACS_EndStrParam();
}

void Dbg_Note_Impl(cstr fmt, ...)
{
   if(!get_bit(dbglevel, log_devh)) return;

   va_list vl;

   ACS_BeginPrint();

   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   dbgnote[dbgnotenum++] = ACS_EndStrParam();
}

void Dbg_PrintMemC(void const *data, size_t size)
{
   u32 const *d = data;
   i32 pos = 0;

   for(size_t i = 0; i < size * 4; i++) {
      if(pos + 3 > 79) {
         ACS_PrintChar('\n');
         pos = 0;
      }

      byte c = Cps_GetC(d, i);

      __nprintf(IsPrint(c) ? "%c  " : c"%.2X ", c);

      pos += 3;
   }

   PrintChrSt("\nEOF\n");
}

void Dbg_PrintMem(void const *data, size_t size)
{
   byte const *d = data;
   i32 pos = 0;

   for(size_t i = 0; i < size; i++)
   {
      if(pos + 3 > 79) {
         ACS_PrintChar('\n');
         pos = 0;
      }

      __nprintf(IsPrint(d[i]) ? "%c  " : c"%.2X ", d[i]);

      pos += 3;
   }

   PrintChrSt("\nEOF\n");
}

void Log(cstr fmt, ...)
{
   va_list vl;

   ACS_BeginPrint();

   va_start(vl, fmt);
   __vnprintf(fmt, vl);
   va_end(vl);

   ACS_EndLog();
}

/* Scripts ----------------------------------------------------------------- */

alloc_aut(0) script_str ext("ACS") addr(OBJ "FontTest")
void Sc_FontTest(i32 fontnum) {
   static struct {cstr lhs, rhs;} const strings[] = {
      /*
      "WidTest", u8"0123456789ABCDEFGHIJKLMNOPQ",
      */
      "L1SPnc1", u8"¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼",
      "L1SPnc2", u8"½¾¿×÷µ",
      "L1SLet1", u8"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛ",
      "L1SLet2", u8"ÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö",
      "L1SLet3", u8"øùúûüýþÿ",
      "LExtA",   u8"ĀāĒēĪīŌōŒœŪūſ",
      "GenPnc1", u8"‒–—‘’‚‛“”„‟†‡•‣․‥…‧‰‱′″‴‵‶‷",
      "GenPnc2", u8"‹›※‼‽⁂⁃⁄⁅⁆⁇⁈⁉⁊⁋⁌⁍⁎⁏⁑⁒⁓⁕⁖⁗⁘⁙",
      "GenPnc3", u8"⁚⁛⁜⁝⁞",
      "CyrMj1",  u8"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩ",
      "CyrMj2",  u8"ЪЫЬЭЮЯ",
      "CyrMi1",  u8"абвгдеёжзийклмнопрстуфхцчшщ",
      "CyrMi2",  u8"ъыьэюя",
      "Serv",    u8"",
      "SceNum",  u8"",
   };

   static cstr const pangrams[] = {
      u8"Falsches Üben von Xylophonmusik quält jeden größeren Zwerg",
      u8"The quick brown fox jumps over the lazy dog",
      u8"Jovencillo emponzoñado de whisky: ¡qué figurota exhibe!",
      u8"Voix ambiguë d'un cœur qui au zéphyr préfère les jattes de kiwis",
      u8"Широкая электрификация южных подъёму сельского хозяйства",
   };

   for(;;) {
      enum {_h = 12};
      str font = fonts[fontnum];
      SetSize(320, 240);
      i32 y = 0;
      for(i32 i = 0; i < countof(strings); i++) {
         PrintLine(0, y, 70, y, 0xFF00FF);
         PrintLine(0, y, 0, y + _h, 0xFF00FF);
         PrintLine(70, y, 320, y, 0x00FFFF);
         PrintLine(70, y, 70, y + _h, 0x00FFFF);
         PrintTextChS(strings[i].lhs);
         PrintTextX(font, CR_WHITE, 0,1, y,1, ptf_no_utf);
         PrintTextChS(strings[i].rhs);
         PrintTextX(font, CR_WHITE, 70,1, y,1, ptf_no_utf);
         y += _h;
      }
      SetClipW(0, 0, 320, 240, 320);
      PrintTextChS(pangrams[ticks / 35 % countof(pangrams)]);
      PrintTextX(font, CR_WHITE, 0,1, y,1, ptf_no_utf);
      ClearClip();
      ACS_Delay(1);
   }
}

script_str ext("ACS") addr(OBJ "GiveEXPToMonster")
void Sc_GiveEXPToMonster(i32 amt) {
   ifauto(dmon_t *, m, DmonPtr(0, AAPTR_PLAYER_GETTARGET)) m->exp += amt;
}

script_str ext("ACS") addr(OBJ "GiveMeAllOfTheScore")
void Sc_DbgGiveScore(void) {
   with_player(LocalPlayer) P_Scr_Give(p, INT96_MAX, true);
}

script_str ext("ACS") addr(OBJ "DumpAlloc")
void Sc_DbgDumpAlloc(void) {
   __GDCC__alloc_dump();
}

script_str ext("ACS") addr(OBJ "PrintMonsterInfo")
void Sc_PrintMonsterInfo(void) {
   PrintMonsterInfo();
}

script_str ext("ACS") addr(OBJ "TriggerEnding")
void Sc_TriggerEnding(i32 num) {
   cstr which;
   switch(num) {
   default:
   case 0: which = "Division";    break;
   case 1: which = "Other";       break;
   case 2: which = "Normal";      break;
   case 3: which = "Barons";      break;
   case 4: which = "CyberDemon";  break;
   case 5: which = "SpiderDemon"; break;
   case 6: which = "IconOfSin";   break;
   }
   F_Start(which);
}
#endif

/* EOF */
