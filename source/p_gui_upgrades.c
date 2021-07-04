/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Upgrades tab GUI.
 *
 * ---------------------------------------------------------------------------|
 */

#include "u_common.h"
#include "w_world.h"

/* FIXME */
static
str const upgrcateg[] = {
   [UC_Body] = sLANG "CAT_BODY",
   [UC_Weap] = sLANG "CAT_WEAP",
   [UC_Extr] = sLANG "CAT_EXTR",
   [UC_Down] = sLANG "CAT_DOWN",
};

static
void GUIUpgradesList(struct gui_state *g) {
   if(G_Button(g, .x = 77, 200, Pre(btnprev)))
      if(CBIState(g)->upgrfilter-- <= 0)
         CBIState(g)->upgrfilter = UC_MAX;

   if(G_Button(g, .x = 77 + gui_p.btnprev.w, 200, Pre(btnnext)))
      if(CBIState(g)->upgrfilter++ >= UC_MAX)
         CBIState(g)->upgrfilter = 0;

   i32 numbtns = 0;
   i32 filter  = CBIState(g)->upgrfilter - 1;

   str filter_name;

   if(filter != -1) {
      filter_name = ns(language(upgrcateg[filter]));

      for_upgrade(upgr) {
         if(upgr->info->category == filter) {
            numbtns++;
         }
      }
   } else {
      filter_name = ns(language(sl_cat_all));

      for_upgrade(upgr) {
         numbtns++;
      }

      numbtns += UC_MAX;
   }

   PrintTextFmt(tmpstr(language(sl_cat_filter)), filter_name);
   PrintText(sf_smallfnt, g->defcr, g->ox+2,1, g->oy+202,1);

   G_ScrBeg(g, &CBIState(g)->upgrscr, 2, 23, gui_p.btnlist.w, 178, gui_p.btnlist.h * numbtns);

   i32 curcategory = -1;

   i32 y = -gui_p.btnlist.h;
   for_upgrade(upgr) {
      if(filter != -1) {
         if(upgr->info->category != filter) continue;
      } else if(upgr->info->category != curcategory) {
         curcategory = upgr->info->category;
         y += gui_p.btnlist.h;
         PrintText_str(ns(language(upgrcateg[curcategory])), sf_lmidfont, g->defcr, g->ox + 40,4, y + g->oy + 1,1);
      }

      y += gui_p.btnlist.h;

      if(G_ScrOcc(g, &CBIState(g)->upgrscr, y, gui_p.btnlist.h))
         continue;

      cstr color;
      if(!get_bit(upgr->flags, _ug_owned) && !P_Shop_CanBuy(&upgr->info->shopdef, upgr)) {
         color = "u";
      } else {
         switch(upgr->info->key) {
            case UPGR_TorgueMode: color = "g"; break;
            case UPGR_Cannon_C:   color = "m"; break;
            default:              color = nil; break;
         }
      }

      struct gui_pre_btn const *pre;
      /**/ if(get_bit(upgr->flags, _ug_active)) pre = &gui_p.btnlistactivated;
      else if(get_bit(upgr->flags, _ug_owned )) pre = &gui_p.btnlistactive;
      else                                      pre = &gui_p.btnlistsel;

      i32 *upgrsel = &CBIState(g)->upgrsel;
      if(G_Button_HId(g, _i, tmpstr(language_fmt(LANG "UPGRADE_TITLE_%s",
                                                 upgr->info->name)),
                      0, y, _i == *upgrsel, .color = color, .preset = pre))
         *upgrsel = _i;

      for(i32 i = 0; i < 4; i++) {
         StrAry(gfxs,
                s":UI:Group1", s":UI:Group2", s":UI:Group3", s":UI:Group4");
         if(get_bit(upgr->agroups, i)) {
            PrintSprite(gfxs[i], g->ox + pre->w - 9,1, g->oy + y + 1,1);
         }
      }
   }

   G_ScrEnd(g, &CBIState(g)->upgrscr);
}

static
void GUIUpgradeRequirements(struct gui_state *g, struct upgrade *upgr) {
   i32 y = 0;

   #define Req(name) statement({ \
      ACS_BeginPrint(); \
      ACS_PrintString(ns(language(name))); \
      ACS_PrintChar(' '); \
      ACS_PrintString(ns(language(sl_required))); \
      PrintText(sf_smallfnt, CR_RED, g->ox+98,1, g->oy+187 + y,2); \
      y -= 10; \
   })

   if(RequiresButDontHave_AI)  Req(sl_cbi_armorinter);
   if(RequiresButDontHave_WMD) Req(sl_cbi_weapninter);
   if(RequiresButDontHave_WRD) Req(sl_cbi_weapninte2);
   if(RequiresButDontHave_RDI) Req(sl_cbi_rdistinter);
   if(RequiresButDontHave_RA)  Req(sl_cbi_reactarmor);

   #undef Req

   /* Performance rating */
   if(upgr->info->perf && pl.pclass != pcl_cybermage) {
      bool over = upgr->info->perf + pl.cbi.pruse > cbiperf;
      cstr fmt;

      if(get_bit(upgr->flags, _ug_active))
         fmt = tmpstr(language(sl_shop_disable_saves));
      else if(over)
         fmt = tmpstr(language(sl_shop_cant_activate));
      else
         fmt = tmpstr(language(sl_shop_activate_uses));

      PrintTextFmt(fmt, upgr->info->perf);
      PrintText(sf_smallfnt, g->defcr, g->ox+98,1, g->oy+187 + y,2);
      y -= 10;
   }

   /* Score multiplier */
   if(upgr->info->scoreadd != 0) {
      char cr;
      cstr op;
      bool chk;

      if(get_bit(upgr->flags, _ug_active)) {
         chk = upgr->info->scoreadd > 0;
         op  = tmpstr(language(sl_shop_mul_disable));
      } else {
         chk = upgr->info->scoreadd < 0;
         op  = tmpstr(language(sl_shop_mul_enable));
      }

      i32 perc = fastabs(ceilk(100.0k * upgr->info->scoreadd));
      if(chk) {cr = 'a'; perc = 100 - perc;}
      else    {cr = 'n'; perc = 100 + perc;}

      PrintTextFmt(op, cr, perc);
      PrintText(sf_smallfnt, g->defcr, g->ox+98,1, g->oy+187 + y,2);
      y -= 10;
   }
}

static
void GUIUpgradeDescription(struct gui_state *g, struct upgrade *upgr) {
   G_Clip(g, g->ox+98, g->oy+17, 190, 170, 184);

   /* Cost */
   cstr mark;
   str cost;

   switch(upgr->info->key) {
   case UPGR_lolsords:   mark = "\Cjfolds"; break;
   case UPGR_TorgueMode: mark = "\Cd$";     break;
   default:              mark = "\Cnscr";   break;
   }

   if(upgr->info->cost) cost = StrParam("%s%s", scoresep(P_Shop_Cost(&upgr->info->shopdef)), mark);
   else                 cost = ns(language(sl_free));

   PrintText_str(cost, sf_smallfnt, g->defcr, g->ox+98,1, g->oy+17,1);

   /* Category */
   PrintText_str(ns(language(upgrcateg[upgr->info->category])), sf_smallfnt, g->defcr, g->ox+98,1, g->oy+27,1);

   /* Effect */
   ifauto(str, effect, language_fmt(LANG "UPGRADE_EFFEC_%s", upgr->info->name))
      PrintTextFmt("%S %S", ns(language(sl_effect)), effect);

   static
   i32 const crs[] = {
      CR_RED, CR_ORANGE, CR_YELLOW, CR_GREEN, CR_BLUE, CR_PURPLE, CR_DARKRED
   };

   i32 cr = g->defcr;

   if(upgr->info->key == UPGR_UNCEUNCE)
      cr = crs[ACS_Timer() / 4 % countof(crs)];

   PrintText(sf_smallfnt, cr, g->ox+98,1, g->oy+37,1);

   G_ClipRelease(g);
}

static
void GUIUpgradeButtons(struct gui_state *g, struct upgrade *upgr) {
   /* Buy */
   if(G_Button(g, tmpstr(language(sl_buy)), 98, 192, !P_Shop_CanBuy(&upgr->info->shopdef, upgr), .fill = {&CBIState(g)->buyfill, CVarGetI(sc_gui_buyfiller)}))
      P_Upg_Buy(upgr, false);

   /* Activate */
   if(G_Button(g, tmpstr(get_bit(upgr->flags, _ug_active) ?
                         language(sl_deactivate) :
                         language(sl_activate)),
               98 + gui_p.btndef.w + 2, 192, !P_Upg_CanActivate(upgr)))
      P_Upg_Toggle(upgr);

   /* Groups */
   PrintText_str(ns(language(sl_autogroups)), sf_smallfnt, g->defcr, g->ox+242,0, g->oy+192,0);

   for(i32 i = 0; i < 4; i++) {
      static
      i32 const crs[] = {CR_BRICK, CR_GREEN, CR_LIGHTBLUE, CR_GOLD};

      ACS_BeginPrint();
      ACS_PrintChar('1' + i);
      PrintText(sf_lmidfont, crs[i], g->ox+207 + i * 20,2, g->oy+202,1);

      if(G_ChkBox_HId(g, i, get_bit(upgr->agroups, i), 208 + i * 20, 198)) {
         tog_bit(upgr->agroups, i);
         P_Data_Save();
      }
   }
}

void P_CBI_TabUpgrades(struct gui_state *g) {
   GUIUpgradesList(g);

   if(CBIState(g)->upgrsel == UPGR_MAX) {
      for_upgrade(upgr) {
         if(get_bit(upgr->flags, _ug_available)) {
            CBIState(g)->upgrsel = _i;
            break;
         }
      }
   }

   struct upgrade *upgr = &pl.upgrades[CBIState(g)->upgrsel];

   GUIUpgradeDescription (g, upgr);
   GUIUpgradeButtons     (g, upgr);
   GUIUpgradeRequirements(g, upgr);
}

/* EOF */
