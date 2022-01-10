/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Upgrade entry points and utilities.
 *
 * ---------------------------------------------------------------------------|
 */

#ifndef p_upgrades_h
#define p_upgrades_h

#include "common.h"
#include "u_names.h"
#include "u_data.h"
#include "p_shopdef.h"
#include "m_list.h"

#define P_Upg_Buy(upgr, ...) \
   P_Shop_Buy(&(upgr)->info->shopdef, (upgr), LANG "UPGRADE_TITLE_%s%s", __VA_ARGS__)

#define for_upgrade(name) \
   for(i32 _i = 0; _i < UPGR_MAX; _i++) \
      if(get_bit(pl.upgrades[_i].flags, _ug_available)) \
         __with(struct upgrade *name = &pl.upgrades[_i];)

#define RequiresButDontHave(a1, a2) (get_bit(upgr->info->requires, a1) && !(a2))
#define RequiresButDontHave_AI  RequiresButDontHave(UR_AI,  cbiupgr[cupg_armorinter])
#define RequiresButDontHave_WMD RequiresButDontHave(UR_WMD, cbiupgr[cupg_weapninter])
#define RequiresButDontHave_WRD RequiresButDontHave(UR_WRD, cbiupgr[cupg_weapninte2])
#define RequiresButDontHave_RDI RequiresButDontHave(UR_RDI, cbiupgr[cupg_rdistinter])
#define RequiresButDontHave_RA  RequiresButDontHave(UR_RA,  get_bit(pl.upgrades[UPGR_ReactArmor].flags, _ug_owned))

extern struct upgradeinfo upgrinfo[UPGR_MAX];

/* These are included here so the compiler may check the function signatures. */
#define Fn_F(n, cb) void Upgr_##n##_##cb(struct upgrade *upgr);
#define Fn_S(n, cb) script void Upgr_##n##_##cb(struct upgrade *upgr);
#include "u_func.h"

void Upgr_MInit(void);

script void P_Upg_PInit(void);
void P_Upg_PQuit(void);
void P_Upg_PDeinit(void);
void P_Upg_PMInit(void);

void P_Upg_Enter(void);

#endif
