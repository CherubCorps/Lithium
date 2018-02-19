// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
#ifndef LITH_PLAYER_H
#define LITH_PLAYER_H

#include "lith_common.h"
#include "lith_cbi.h"
#include "lith_upgrades.h"
#include "lith_pdata.h"
#include "lith_bip.h"
#include "lith_log.h"
#include "lith_list.h"
#include "lith_weaponinfo.h"
#include "lith_shopdef.h"
#include "lith_attrib.h"
#include "lith_items.h"

#include <GDCC/HashMap.h>

#define MAX_PLAYERS 8

#define Lith_ForPlayer() \
   for(int _piter = 0; _piter < MAX_PLAYERS; _piter++) \
      __with(struct player *p = &players[_piter];) \
         if(p->active)

#define Lith_GiveAllScore(score, nomul) \
   Lith_ForPlayer() \
      p->giveScore(score, nomul)

#define Lith_GiveAllEXP(amt) \
   Lith_ForPlayer() \
      p->giveEXP(amt)

#define LocalPlayer \
   (ACS_PlayerNumber() < 0 ? null : &players[ACS_PlayerNumber()])
#define PlayerDiscount(n) (i96)((n) * p->discount)
#define NoPlayer(p) (!(p) || !(p)->active)
#define withplayer(ptr) \
   __with(struct player *p = (ptr);) \
      if(!NoPlayer(p))

// Types ---------------------------------------------------------------------|

GDCC_HashMap_Decl(upgrademap_t, int, upgrade_t)

enum
{
   // Shorthand
   pM = pcl_marine,
   pC = pcl_cybermage,
   pI = pcl_informant,
   pW = pcl_wanderer,
   pA = pcl_assassin,
   pD = pcl_darklord,
   pT = pcl_thoth,

   gO = pcl_outcasts,
   gM = pcl_missioners,
   gI = pcl_intruders,
   gA = pcl_any,
   gH = pcl_human,
   gN = pcl_nonhuman,
   gR = pcl_robot,
   gX = pcl_mods,
};

enum
{
   pro_none,
   pro_male,
   pro_female,
   pro_object,
   pro_max,
};

enum
{
   GUI_NONE,
   GUI_CBI,
   GUI_MAX
};

//
// player_delta
//
// Data that needs to be kept track of between frames.
//
struct player_delta
{
   // Status
   int   maxhealth;
   fixed alpha;
   i96   score;
   int   indialogue;

   // Position
   fixed x, y, z;
   fixed floorz;
   fixed pitch, yaw, roll;

   // Movement
   fixed velx, vely, velz;
   fixed pitchv, yawv;
   fixed forwardv, sidev, upv;
   fixed speedmul;
   fixed jumpboost;

   // Input
   int  buttons;
   bool scopetoken;
   int  frozen;
   int  semifrozen;

   // Attributes
   struct player_attributes attr;
};

// Extern Functions ----------------------------------------------------------|

// state
[[__call("ScriptS")]] void Lith_ResetPlayer(struct player *p);
[[__call("ScriptS")]] void Lith_PlayerLoadData(struct player *p);
[[__call("ScriptS")]] void Lith_PlayerSaveData(struct player *p);
[[__call("StkCall")]] int  Lith_PlayerCurWeaponType(struct player *p);
[[__call("StkCall")]] bool Lith_ButtonPressed(struct player *p, int bt);
[[__call("StkCall"), __optional_args(1)]] bool Lith_SetPlayerVelocity(struct player *p, fixed velx, fixed vely, fixed velz, bool add);
void Lith_ValidatePlayerTID(struct player *p);

// gui
[[__call("StkCall")]] void Lith_PlayerCloseGUI(struct player *p);
[[__call("StkCall")]] void Lith_PlayerUseGUI(struct player *p, int type);

// score
[[__optional_args(1)]] i96 Lith_GiveScore(struct player *p, i96 score, bool nomul);
[[__call("StkCall")]] void Lith_TakeScore(struct player *p, i96 score);

// attributes
[[__call("StkCall")]] void Lith_GiveEXP(struct player *p, u64 amt);

// misc
upgrade_t *Lith_PlayerGetNamedUpgrade(struct player *p, int name);
bool Lith_PlayerGetUpgradeActive(struct player *p, int name);
[[__call("StkCall")]] void Lith_ClearTextBuf(struct player *p);
[[__call("StkCall")]] __str Lith_PlayerDiscriminator(int pclass);

[[__call("ScriptS")]] void Lith_PlayerUpdateData(struct player *p);

[[__call("StkCall")]] void Lith_PlayerUpdateStats(struct player *p);

struct player *Lith_GetPlayer(int tid, int ptr);

// Types ---------------------------------------------------------------------|

//
// struct player
//
//       7/4/2016: That's a lot of data!
// edit  9/4/2016: Holy shit, that's really a lot of data!
// edit  7/5/2016: JESUS TAKE THE WHEEL
// edit  3/1/2017: help
// edit  6/1/2017: there's so much data that I had to split it
// edit 23/1/2017: D E S T R O Y
// edit 26/2/2017: There is yet again so much data that I had to split it.
// edit 11/3/2017: NOW WITH PROPERTY HELL
// edit 11/7/2017: and now it's over 5000 bytes.
// edit 14/7/2017: lol nevermind it's only 2kb now
// edit 31/8/2017: m e r g e
//
struct player
{
   // Properties -------------------------------------------------------------|

   // state
   __prop reset         {call: Lith_ResetPlayer(this)}
   __prop loadData      {call: Lith_PlayerLoadData(this)}
   __prop saveData      {call: Lith_PlayerSaveData(this)}
   __prop weapontype    {get:  Lith_PlayerCurWeaponType(this)}
   __prop buttonPressed {call: Lith_ButtonPressed(this)}
   __prop setVel        {call: Lith_SetPlayerVelocity(this)}
   __prop mana          {get:  Lith_CheckActorInventory(->tid, "Lith_MagicAmmo")}
   __prop manamax       {get:  ACS_GetMaxInventory(->tid, "Lith_MagicAmmo")}
   __prop validateTID   {call: Lith_ValidatePlayerTID(this)}
   __prop health {get: ACS_GetActorProperty(->tid, APROP_Health),
                  set: ACS_SetActorProperty(->tid, APROP_Health)}
   __prop setActivator {call: ACS_SetActivator(->tid)}
   __prop getVel {call: mag2k(->velx, ->vely)}

   // inventory
   __prop addItem {call: Lith_PlayerAddItem(this)}

   // score
   __prop giveScore {call: Lith_GiveScore(this)}
   __prop takeScore {call: Lith_TakeScore(this)}

   // attributes
   __prop giveEXP {call: Lith_GiveEXP(this)}

   // log
   __prop log  {call: Lith_Log (this)}
   __prop logF {call: Lith_LogF(this)}
   __prop logH {call: Lith_LogH(this)}

   // gui
   __prop useGUI   {call: Lith_PlayerUseGUI(this)}
   __prop closeGUI {call: Lith_PlayerCloseGUI(this)}

   // misc
   __prop getUpgr       {call: Lith_PlayerGetNamedUpgrade(this)}
   __prop getUpgrActive {call: Lith_PlayerGetUpgradeActive(this)}
   __prop deliverMail   {call: Lith_DeliverMail(this)}
   __prop clearTextBuf  {call: Lith_ClearTextBuf(this)}
   __prop bipUnlock     {call: Lith_UnlockBIPPage(->bipptr, __arg, ->pclass)}
   __prop discrim       {get:  Lith_PlayerDiscriminator(->pclass)}
   __prop classname     {get:  ACS_GetActorPropertyString(->tid, APROP_NameTag)}

   // cvars
   __prop getCVarI {call: ACS_GetUserCVar      (->num)}
   __prop getCVarK {call: ACS_GetUserCVarFixed (->num)}
   __prop getCVarS {call: ACS_GetUserCVarString(->num)}
   __prop setCVarI {call: ACS_SetUserCVar      (->num)}
   __prop setCVarK {call: ACS_SetUserCVarFixed (->num)}
   __prop setCVarS {call: ACS_SetUserCVarString(->num)}

   // shop
   __prop getCost {call: Lith_ShopGetCost(this)}
   __prop canBuy  {call: Lith_ShopCanBuy(this)}
   __prop buy     {call: Lith_ShopBuy(this)}

   // Members ----------------------------------------------------------------|

   // Initialization
   bool wasinit;
   bool active;
   bool dead;
   bool reinit;

   // Info
   int   tid;
   int   num;
   u64   ticks;
   __str name;
   int   pclass;
   __str pcstr;
   int   pronoun;
   int   dlgnum;
   int   fun;

   // Deltas
   [[__anonymous]]
   struct player_delta cur;
   struct player_delta old;
   int oldhealth;

   // BIP
   bip_t bip, *bipptr;

   // Upgrades
   struct upgr_data upgrdata;
   upgrade_t        upgrades[UPGR_STATIC_MAX];
   upgrademap_t     upgrademap;
   int              upgrmax;
   bool             upgrinit;

   // Inventory
   container_t inv[8];
   container_t misc;
   item_t     *useitem;
   item_t     *selitem;
   bool        movitem;

   // HUD
   loginfo_t  loginfo;
   bool       hudenabled;

   // DECORATE/ZScript
   int  decvars[8];
   char txtbuf[8];
   int  tbptr;

   // Score
   i96    scoreaccum;
   __str  scoreaccumstr;
   int    scoreaccumtime;
   double scoremul;
   float  discount;

   // Misc
   int   spuriousexplosions;
   int   brouzouf;
   bool  hadinfrared;
   fixed rage;
   __str notes[16];
   bool  autoreload;

   // Static data
   bool  staticinit;
   int   spawnhealth;
   fixed spawndfactor;
   fixed jumpheight;
   fixed viewheight;
   __str stepnoise;

   // pitch/yaw in precalculated sane radian format
   float pitchf;
   float yawf;

   // Additive angles
   float addpitch;
   float addyaw;
   float addroll;

   // Damage bob angles
   float bobpitch;
   float bobyaw;

   // Extra angles
   float extrpitch;
   float extryaw;

   // View TIDs
   int cameratid;
   int weathertid;

   // GUI
   int   activegui;
   cbi_t cbi;

   // Statistics
   int weaponsheld;
   int itemsbought;
   int upgradesowned;

   i64 healthsum;
   i64 healthused;

   i96 scoresum;
   i96 scoreused;

   int unitstravelled;

   // Weapons
   weapondata_t weapon;

   int    riflefiremode;
   list_t hudstrlist;

   __str weaponclass;

   // Keys
   struct keycards_s
   {
      bool rc : 1;
      bool yc : 1;
      bool bc : 1;
      bool rs : 1;
      bool ys : 1;
      bool bs : 1;
   } keys;

   // 🌌 「÷」 0
   struct divsigil
   {
      bool acquired;
   } sigil;
};

typedef void (*player_cb_t)(struct player *p);

// Extern Objects ------------------------------------------------------------|

#ifndef EXTERNAL_CODE
extern struct player players[MAX_PLAYERS];
#else
struct player (*Lith_GetPlayersExtern(void))[MAX_PLAYERS];
#define players (*Lith_GetPlayersExtern())
#endif

#endif
