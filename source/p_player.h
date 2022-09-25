// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Player data and functions.                                               │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#if defined(pclass_x)
pclass_x(pM, 1 << pcl_marine)
pclass_x(pC, 1 << pcl_cybermage)
pclass_x(pI, 1 << pcl_informant)
pclass_x(pW, 1 << pcl_wanderer)
pclass_x(pA, 1 << pcl_assassin)
pclass_x(pD, 1 << pcl_darklord)
pclass_x(pT, 1 << pcl_thoth)
/* Groups */
pclass_x(gO, pM | pC)
pclass_x(gM, pI | pW)
pclass_x(gI, pA | pD)
pclass_x(gF, pT)
/* Lifeform Type */
pclass_x(gH, pM | pC | pA)
pclass_x(gN, pW | pD | pT)
pclass_x(gR, pI)
/* Misc. Abilities */
pclass_x(gA, gH | gN | gR)
pclass_x(gU, pC | pW | pT)
#undef pclass_x
#elif !defined(p_player_h)
#define p_player_h

enum ZscName(Attrib) {
   at_acc,
   at_def,
   at_str,
   at_vit,
   at_stm,
   at_luk,
   at_spc,
   at_max
};

enum ZscName(SubweaponType) {
   _subw_gun,
   _subw_grenade,
   _subw_axe,
   _subw_dart,
   _subw_max,
};

enum ZscName(PClass) {
   pcl_marine,
   pcl_cybermage,
   pcl_informant,
   pcl_wanderer,
   pcl_assassin,
   pcl_darklord,
   pcl_thoth,
   pcl_max,
   #define pclass_x(shr, eq) shr = eq,
   #include "p_player.h"
   pcl_outcasts   = gO,
   pcl_missioners = gM,
   pcl_intruders  = gI,
   pcl_finalizer  = gF,
   pcl_human    = gH,
   pcl_nonhuman = gN,
   pcl_robot    = gR,
   pcl_any       = gA,
   pcl_magicuser = gU,
};

#if !ZscOn
#include "m_engine.h"
#include "p_cbi.h"
#include "p_upgrades.h"
#include "p_bip.h"
#include "m_list.h"
#include "p_weapons.h"
#include "p_shopdef.h"
#include "p_attrib.h"
#include "items.h"
#include "d_vm.h"

#define P_Wep_CurType() (pl.weapon.cur.type)
script void P_Player(void);
script void P_Init(void);
script void P_Data_Load(void);
script void P_Data_Save(void);
bool P_ButtonPressed(i32 bt);
optargs(1) bool P_SetVel(k32 velx, k32 vely, k32 velz, bool add);
void P_ValidateTID(void);
void P_GUI_Close(void);
void P_GUI_Use(void);
score_t P_Scr_Give(k32 x, k32 y, k32 z, score_t score, bool nomul);
score_t P_Scr_GivePos(i32 x, i32 y, score_t score, bool nomul);
void P_Scr_Take(score_t score);
void P_Lv_GiveEXP(i32 amt);
stkcall i32 P_Color(i32 pclass);
void P_Dat_PTick(void);
script void P_Scr_Payout(void);
void P_Log_SellWeapon(i32 which, score_t score);
void P_Log_Weapon(i32 which);
void P_LogB(i32 levl, cstr fmt, ...); /* log to HUD and full log */
void P_LogH(i32 levl, cstr fmt, ...); /* log to HUD only */
void P_LogF(          cstr fmt, ...); /* log to full log only */
void P_Log_Entry(void);
script void P_Log(i32 cr, i32 x, i32 yy);

script void P_Wep_PTickPre(void);
       void P_Dat_PTickPre(void);
script void P_CBI_PTick   (void);
script void P_Inv_PTick   (void);
script void P_Log_PTick   (void);
script void P_Upg_PTick   (void);
script void P_Wep_PTick   (void);
       void P_Ren_PTick   (void);

script void P_TeleportIn (void);
sync   void P_TeleportOut(i32 tag);

#ifndef NDEBUG
void P_Ren_Debug(void);
#endif
script void P_Ren_Step(void);
void P_Ren_View(void);
script void P_Ren_Scope(void);

enum {
   _gui_none,
   _gui_cbi,
   _gui_waypoint,

   _gui_disables_hud,
   _gui_dlg = _gui_disables_hud,
   _gui_intro,
};

enum {
   _itm_disp_none,
   _itm_disp_log,
   _itm_disp_pop,
   _itm_disp_both,
   _itm_disp_max
};

enum {
   _hud_marine,
   _hud_cybermage,
   _hud_informant,
   _hud_wanderer,
   _hud_assassin,
   _hud_darklord,
   _hud_thoth,
   _hud_old,
   _hud_max
};

/* Data that needs to be kept track of between frames. */
struct player_delta {
   /* Status */
   k32     alpha;
   score_t score;
   i32     maxhealth;

   struct dlg_start_info dlg;

   /* Position */
   k32 x, y, z;
   k32 pitch, yaw, roll;

   /* Movement */
   k32 velx, vely, velz;
   k32 pitchv, yawv;
   k32 forwardv, sidev, upv;

   /* Input */
   i32  buttons;
   bool scopetoken;
   i32  frozen;
   i32  semifrozen;

   /* GUI */
   i32 modal;

   /* Attributes */
   struct player_attributes attr;
};

struct old_player_delta {
   anonymous struct player_delta del;

   i32 health;
   i32 mana;
   i32 shield;
};

/*      07-04-2016: That's a lot of data!
 * edit 09-04-2016: Holy shit, that's really a lot of data!
 * edit 07-05-2016: JESUS TAKE THE WHEEL
 * edit 03-01-2017: help
 * edit 06-01-2017: there's so much data that I had to split it
 * edit 23-01-2017: D E S T R O Y
 * edit 26-02-2017: There is yet again so much data that I had to split it.
 * edit 11-03-2017: NOW WITH PROPERTY HELL
 * edit 11-07-2017: and now it's over 5000 bytes.
 * edit 14-07-2017: lol nevermind it's only 2kb now
 * edit 31-08-2017: m e r g e
 * edit 04-04-2020: WHY DID YOU MAKE IT THAT COMPLEX YOU BUNGUS
 */
struct player {
   /* data */
   __prop megaProtect  {default:    PtrInvNum(->tid, so_MegaProtection)}
   __prop mana         {default:    PtrInvNum(->tid, so_ManaAmmo)}
   __prop manamax      {default:    ACS_GetMaxInventory(->tid, so_ManaAmmo)}
   __prop health       {default:    GetPropI(->tid, APROP_Health)}
   __prop setHealth    {operator(): SetPropI(->tid, APROP_Health)}
   __prop shield       {default:    GetMembI(->tid, sm_Shield)}
   __prop setShield    {operator(): SetMembI(->tid, sm_Shield)}
   __prop setActivator {operator(): ACS_SetActivator(0, AAPTR_PLAYER1)}
   __prop getVel       {operator(): mag2k(->velx, ->vely)}
   __prop onground     {default:    GetMembI(->tid, sm_OnGround)}
   __prop waterlevel   {default:    GetMembI(->tid, sm_WaterLevel)}
   __prop classname    {default:    GetNameTag(->tid)}
   __prop overdrive    {default:    GetMembI(->tid, sm_Overdrive)}

   /* Initialization */
   bool wasinit;
   bool dead;
   bool reinit;

   /* Info */
   i32  tid;
   i32  num;
   i32  ticks;
   str  name;
   i32  pclass;
   char discrim[5];
   i32  color;
   cstr obit;

   /* Deltas */
   anonymous
   struct     player_delta cur;
   struct old_player_delta old;

   /* Upgrades */
   struct upgr_data upgrdata;
   struct upgrade   upgrades[UPGR_MAX];

   i32 autobuy;

   /* Inventory */
   struct container inv[_inv_num];

   struct item *useitem;
   struct item *selitem;
   struct item *opnitem;
   bool         movitem;

   /* HUD */
   bool hudenabled;

   /* Score */
   score_t scoreaccum;
   i32     scoreaccumtime;
   i32     scoremul;

   /* Misc */
   k32 rage;
   i32 speedmul;
   i32 jumpboost;

   char *notes[16];

   i32 nextstep;

   bool teleportedout;
   i32  done_intro;

   i32 shieldmax;
   i32 regenwaitmax;
   i32 regenwait;

   i32 missionstatshow;

   /* Input */
   struct gui_txt tb;

   /* Static data */
   i32 spawnhealth;
   k32 jumpheight;
   k32 viewheight;
   k32 attackheight;

   /* pitch/yaw in precalculated sane radian format */
   k64 pitchf;
   k64 yawf;

   /* Additive angles */
   k64 addpitch;
   k64 addyaw;
   k64 addroll;

   /* Damage bob angles */
   k64 bobpitch;
   k64 bobyaw;
   k64 bobroll;

   /* Extra angles */
   k64 extrpitch;
   k64 extryaw;
   k64 extrroll; /* throw em to the dogs -suwy */

   /* GUI */
   struct cbi cbi;

   /* Statistics */
   i32 healthsum;
   i32 healthused;

   score_t scoresum;
   score_t scoreused;

   i32 unitstravelled;

   i32 spuriousexplosions;
   i32 brouzouf;

   i32 itemsbought;
   i32 upgradesowned;

   /* Weapons */
   struct weapondata weapon;
   i32 weaponsheld;

   i32  riflefiremode;
   bool bladehit, rendhit;

   /* 🌌 「÷」 0 */
   bool sgacquired;
};
#endif

#if !ZscOn
extern struct player pl;
#endif

#endif
