/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Attribute handling.
 *
 * ---------------------------------------------------------------------------|
 */

#include "common.h"
#include "p_player.h"
#include "w_world.h"

/* Static Functions -------------------------------------------------------- */

static void AttrBar(struct gui_state *g, i32 x, i32 y, i32 w, str gfx)
{
   G_Clip(g, x, y, w * 4, 8);
   PrintSprite(gfx, x,1, y,1);
   G_ClipRelease(g);
}

static void DrawAttr(struct gui_state *g, i32 x, i32 y, struct player *p, i32 at)
{
   u32  attr = p->attr.attrs[at];
   cstr name = p->attr.names[at];
   k32  helptrns = 0.5;

   if(p->attr.points)
      if(G_Button_Id(g, at, .x = x-42 + gui_p.btnnext.w, y-2, Pre(btnnext), .slide = true))
   {
      p->attr.points--;
      p->attr.attrs[at]++;
   }

   PrintTextChr(name, 3);
   PrintText(s_lmidfont, CR_WHITE, x-24,1, y,1);

   PrintSprite(sp_UI_AttrBar1, x,1, y,1);

   AttrBar(g, x, y, attr, sp_UI_AttrBar2);

   if(attr > ATTR_VIS_MAX)
   {
      i32 vatr = attr - ATTR_VIS_MAX;
      AttrBar(g, x, y, (vatr / (k64)ATTR_VIS_DIFF) * ATTR_VIS_MAX, sp_UI_AttrBar4);
      helptrns += 0.3;
   }

   PrintTextA_str(Language(LANG "ATTR_HELP_%.3s", name), s_smallfnt, CR_WHITE, x+1,1, y,1, helptrns);

   PrintTextFmt("%u/%i", attr, ATTR_VIS_MAX);
   PrintText(s_lmidfont, CR_WHITE, x+202,1, y,1);
}

static void StatusInfo(struct gui_state *g, i32 y, str left, str right)
{
   PrintText_str(left,  s_lmidfont, CR_WHITE,  30,1, y,1);
   PrintText_str(right, s_smallfnt, CR_WHITE, 280,2, y,1);
}

/* Extern Functions -------------------------------------------------------- */

void P_CBI_TabStatus(struct gui_state *g, struct player *p)
{
   Str(class, s"Class"); /* TODO */

   i32 y = 40;

   PrintText_str(p->name, s_lmidfont, CR_WHITE, 30,1, y,1);
   y += 10;

   StatusInfo(g, y += 10, class, p->classname);

   StatusInfo(g, y += 10, st_lv, StrParam("%u", p->attr.level));
   StatusInfo(g, y += 10, st_hp, StrParam("%i/%i", p->health, p->maxhealth));

   if(p->pclass & pcl_magicuser)
      StatusInfo(g, y += 10, st_mp, StrParam("%i/%i", p->mana, p->manamax));

   StatusInfo(g, y += 10, st_exp,  StrParam("%u", p->attr.exp));
   StatusInfo(g, y += 10, st_next, StrParam("%u", p->attr.expnext));

   y += p->pclass & pcl_magicuser ? 20 : 30;

   if(p->attr.points)
   {
      /* TODO */
      PrintTextFmt("Divide %u points among your attributes.", p->attr.points);
      PrintText(s_smallfnt, CR_WHITE, 20,1, y,1);
   }

   y += 10;

   for(i32 i = 0; i < at_max; i++, y += 10)
      DrawAttr(g, 53, y, p, i);
}

/* EOF */
