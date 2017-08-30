// vim: columns=110
#include "lith_common.h"
#include "lith_player.h"
#include "lith_monster.h"
#include "lith_hudid.h"
#include "lith_world.h"

#include <math.h>

#define ValidateWeapon(parm) (parm < weapon_max && parm >= weapon_min)

// Extern Objects ------------------------------------------------------------|

#define A(a) "Lith_" a "Ammo"
#define M(a) "Lith_" a "ShotsFired"
#define P(a) "weapons/" a "/pickup"
#define N(a) .classname = "Lith_" a, .name = a
#define F(...) .flags = __VA_ARGS__
weaponinfo_t const weaponinfo[weapon_max] = {
   // !!ACHTUNG!! DON'T FUCKING CHANGE THIS WITHOUT CHANGING THE HEADER FIRST !!ACHTUNG!!
   {0, pcl_any, null, "MMMMHMHMMMHMMM"},

   // Outcast Weapons
   {1, pcl_outcasts, N("ChargeFist"),      P("cfist"),    AT_None,                          },
   {5, pcl_outcasts, N("MissileLauncher"), P("missile"),  AT_AMag, A("Rocket"), M("Missile")},
   {6, pcl_outcasts, N("PlasmaDiffuser"),  P("plasdiff"), AT_Ammo, A("Plasma")              },

   // Marine Weapons
   {1, pcl_marine, N("Fist"),            "MMMMHMHMMMHMMM", AT_None,              },
   {2, pcl_marine, N("Pistol"),          P("pistol"),      AT_NMag, M("Pistol")  },
   {2, pcl_marine, N("Revolver"),        P("revolver"),    AT_NMag, M("Revolver")},
   {3, pcl_marine, N("Shotgun"),         P("shotgun"),     AT_None,              },
   {3, pcl_marine, N("LazShotgun"),      P("lshotgun"),    AT_None,              },
   {3, pcl_marine, N("SuperShotgun"),    P("ssg"),         AT_Ammo, A("Shell")   },
   {4, pcl_marine, N("CombatRifle"),     P("rifle"),       AT_NMag, M("Rifle")   },
   {4, pcl_marine, N("SniperRifle"),     P("sniper"),      AT_NMag, M("Sniper")  },
   {5, pcl_marine, N("GrenadeLauncher"), P("rocket"),      AT_Ammo, A("Rocket")  },
   {6, pcl_marine, N("PlasmaRifle"),     P("plasma"),      AT_Ammo, A("Plasma")  },
   {7, pcl_marine, N("BFG9000"),         P("cannon"),      AT_Ammo, A("Cannon")  },

   // Cyber-Mage Weapons
   {1, pcl_cybermage, N("CFist"),         "YOUSONOFABITCH", AT_None,                           },
   {2, pcl_cybermage, N("Mateba"),        P("mateba"),      AT_NMag, M("Mateba")               },
   {3, pcl_cybermage, N("ShockRifle"),    P("erifle"),      AT_NMag, M("ShockRifle")           },
   {3, pcl_cybermage, N("SPAS"),          P("cshotgun"),    AT_AMag, A("Shell"), M("SPAS")     },
   {4, pcl_cybermage, N("SMG"),           P("smg"),         AT_NMag, M("SMG")                  },
   {5, pcl_cybermage, N("IonRifle"),      P("ionrifle"),    AT_AMag, A("Rocket"), M("IonRifle")},
   {6, pcl_cybermage, N("CPlasmaRifle"),  P("plasma"),      AT_Ammo, A("Plasma")               },
   {7, pcl_cybermage, N("StarDestroyer"), P("shipgun"),     AT_Ammo, A("Cannon")               },

   {0, pcl_cybermage, N("Blade"),    "MMMMHMHMMMHMMM", AT_Ammo, "Lith_Mana",                F(wf_magic)},
   {0, pcl_cybermage, N("Delear"),   "MMMMHMHMMMHMMM", AT_AMag, "Lith_Mana", M("Delear"),   F(wf_magic)},
   {0, pcl_cybermage, N("Feuer"),    "MMMMHMHMMMHMMM", AT_Ammo, "Lith_Mana",                F(wf_magic)},
   {0, pcl_cybermage, N("Rend"),     "MMMMHMHMMMHMMM", AT_Ammo, "Lith_Mana",                F(wf_magic)},
   {0, pcl_cybermage, N("Hulgyon"),  "MMMMHMHMMMHMMM", AT_Ammo, "Lith_Mana",                F(wf_magic)},
   {0, pcl_cybermage, N("StarShot"), "MMMMHMHMMMHMMM", AT_AMag, "Lith_Mana", M("StarShot"), F(wf_magic)},
   {0, pcl_cybermage, N("Cercle"),   "MMMMHMHMMMHMMM", AT_Ammo, "Lith_Mana",                F(wf_magic)},
};
#undef A
#undef M
#undef P
#undef N
#undef F

// Static Functions ----------------------------------------------------------|

//
// GiveWeaponItem
//
static void GiveWeaponItem(int parm)
{
   switch(parm)
   {
   case weapon_c_fist:
   case weapon_fist:      ACS_GiveInventory("Lith_Death",      1);    break;
   case weapon_c_spas:    ACS_GiveInventory("Lith_ShellAmmo",  8);    break;
   case weapon_ssg:       ACS_GiveInventory("Lith_ShellAmmo",  4);    break;
   case weapon_c_sniper:  ACS_GiveInventory("Lith_RocketAmmo", 6);    break;
   case weapon_launcher:  ACS_GiveInventory("Lith_RocketAmmo", 2);    break;
   case weapon_c_plasma:
   case weapon_plasma:    ACS_GiveInventory("Lith_PlasmaAmmo", 1500); break;
   case weapon_c_shipgun: ACS_GiveInventory("Lith_CannonAmmo", 5);    break;
   case weapon_bfg:       ACS_GiveInventory("Lith_CannonAmmo", 4);    break;
   }
}

//
// PlayWeaponPickupSound
//
static void PlayWeaponPickupSound(player_t *p, weaponinfo_t const *info)
{
   if(!p->getUpgr(UPGR_7777777)->active) ACS_LocalAmbientSound(info->pickupsound, 127);
   else                                  ACS_LocalAmbientSound("marathon/pickup", 127);
}

//
// Lith_PickupScore
//
static void Lith_PickupScore(player_t *p, int parm)
{
   score_t score = 4000 * weaponinfo[parm].slot;

   GiveWeaponItem(parm);
   p->giveScore(score);
   p->log("> Sold the %S for %lli\Cnscr\C-.",
      Language("LITH_TXT_INFO_SHORT_%S", weaponinfo[parm].name),
      p->getModScore(score));
}

// Extern Functions ----------------------------------------------------------|

//
// Lith_WeaponPickup
//
[[__call("ScriptS"), __extern("ACS")]]
bool Lith_WeaponPickup(int name)
{
   extern void Lith_PickupMessage(player_t *p, weaponinfo_t const *info);

   player_t *p = LocalPlayer;
   if(NoPlayer(p)) return false;

   bool weaponstay = ACS_GetCVar("sv_weaponstay");
   int parm = weapon_unknown;

   switch(p->pclass)
   {
   #define Case(name, set) case name: parm = set; break
   case pcl_marine:
      switch(name)
      {
      Case(wepnam_fist,           weapon_fist);
      Case(wepnam_chainsaw,       weapon_cfist);
      Case(wepnam_pistol,         weapon_pistol);
      Case(wepnam_shotgun,        weapon_shotgun);
      Case(wepnam_supershotgun,   weapon_ssg);
      Case(wepnam_chaingun,       weapon_rifle);
      Case(wepnam_rocketlauncher, weapon_launcher);
      Case(wepnam_plasmarifle,    weapon_plasma);
      Case(wepnam_bfg9000,        weapon_bfg);
      }
      break;

   case pcl_cybermage:
      switch(name)
      {
      Case(wepnam_fist,           weapon_c_fist);
      Case(wepnam_chainsaw,       weapon_cfist);
      Case(wepnam_pistol,         weapon_c_mateba);
      Case(wepnam_shotgun,        weapon_c_rifle);
      Case(wepnam_supershotgun,   weapon_c_spas);
      Case(wepnam_chaingun,       weapon_c_smg);
      Case(wepnam_rocketlauncher, weapon_c_sniper);
      Case(wepnam_plasmarifle,    weapon_c_plasma);
      Case(wepnam_bfg9000,        weapon_c_shipgun);
      }
      break;
   #undef Case
   }

   if(!ValidateWeapon(parm))
      return true;

   weaponinfo_t const *info = &weaponinfo[parm];

   if(HasWeapon(p, parm))
   {
      if(!weaponstay) {
         PlayWeaponPickupSound(p, info);
         Lith_PickupScore(p, parm);
      }

      return !weaponstay;
   }
   else
   {
      PlayWeaponPickupSound(p, info);

      p->weaponsheld++;
      p->bipUnlock(info->name);

      GiveWeaponItem(parm);
      Lith_PickupMessage(p, info);

      ACS_GiveInventory(info->classname, 1);

      return !weaponstay;
   }
}

//
// Lith_CircleSpread
//
[[__call("ScriptS"), __extern("ACS")]]
fixed Lith_CircleSpread(fixed mdx, fixed mdy, bool getpitch)
{
   static fixed A;
   static fixed P;

   if(!getpitch)
   {
      fixed dx = ACS_RandomFixed(mdx,  0.0);
      fixed dy = ACS_RandomFixed(mdy,  0.0);
      fixed a  = ACS_RandomFixed(1.0, -1.0);

      A = ACS_Sin(a) * dx;
      P = ACS_Cos(a) * dy;

      return A;
   }
   else
      return P;
}

//
// Lith_ChargeFistDamage
//
[[__call("ScriptS"), __extern("ACS")]]
int Lith_ChargeFistDamage()
{
   int amount = ACS_CheckInventory("Lith_FistCharge");
   ACS_TakeInventory("Lith_FistCharge", 0x7FFFFFFF);
   return amount * ACS_Random(1, 3);
}

//
// Lith_GSInit_Weapon
//
void Lith_GSInit_Weapon(void)
{
   for(int i = 0; i < weapon_max; i++)
   {
      weaponinfo_t *info = (weaponinfo_t *)&weaponinfo[i];
      info->type = i;
   }
}

//
// Lith_PlayerUpdateWeapon
//
// Update information on what weapons we have.
//
void Lith_PlayerUpdateWeapon(player_t *p)
{
   weapondata_t *w = &p->weapon;

   w->prev = w->cur;

   // Reset data temporarily.
   w->cur = null;
   for(int i = 0; i < SLOT_MAX; i++)
      w->slot[i] = 0;

   // Iterate over each weapon setting information on it.
   for(int i = weapon_min; i < weapon_max; i++)
   {
      weaponinfo_t const *info = &weaponinfo[i];
      invweapon_t *wep = &w->inv[i];

      w->slot[info->slot] += (wep->owned = ACS_CheckInventory(info->classname));

      wep->info      = info;
      wep->owned     = ACS_CheckInventory(info->classname);
      wep->ammotype  = info->defammotype;
      wep->ammoclass = info->defammoclass;
      wep->magclass  = info->defmagclass;

      switch(i)
      {
      case weapon_shotgun:
         if(p->getUpgr(UPGR_GaussShotty)->active) {
            wep->ammotype  = AT_NMag;
            wep->ammoclass = "Lith_GaussShotsFired";
         }
         break;
      case weapon_c_spas:
         if(p->getUpgr(UPGR_SPAS_B)->active)
            wep->ammotype = AT_Ammo;
         break;
      case weapon_c_smg:
         if(p->getUpgr(UPGR_SMG_A)->active)
            wep->ammoclass = "Lith_SMGShotsFired2";
         break;
      }

      // Set magazine class if this weapon doesn't take ammo.
      if(wep->ammotype & AT_NMag && !(wep->ammotype & AT_Ammo))
         wep->magclass = wep->ammoclass;

      // Check for currently held weapon.
      if(!w->cur && ACS_StrICmp(p->weaponclass, info->classname) == 0)
         w->cur = wep;

      // Remove inactive magic weapons.
      else if(info->flags & wf_magic && wep->owned && ++wep->magictake > 20) {
         ACS_TakeInventory(info->classname, 1);
         wep->magictake = 0;
      }

      // Auto-reload anything else.
      if(p->getUpgr(UPGR_AutoReload)->active && wep->owned && wep->ammotype & AT_NMag && !(info->flags & wf_magic))
      {
         if(wep->autoreload >= 35 * 5)
            ACS_TakeInventory(wep->magclass, 999);

         if(w->cur != wep) wep->autoreload++;
         else              wep->autoreload = 0;
      }
   }

   if(!w->cur) w->cur = &w->inv[weapon_unknown];
}

//
// Lith_PlayerUpdateWeapons
//
void Lith_PlayerUpdateWeapons(player_t *p)
{
   __with(int heat = ACS_CheckInventory("Lith_SMGHeat");) {
           if(heat < 100) ACS_TakeInventory("Lith_SMGHeat", 5);
      else if(heat < 200) ACS_TakeInventory("Lith_SMGHeat", 4);
      else if(heat < 300) ACS_TakeInventory("Lith_SMGHeat", 3);
      else if(heat < 400) ACS_TakeInventory("Lith_SMGHeat", 2);
      else                ACS_TakeInventory("Lith_SMGHeat", 1);
   }

   if(p->weapontype == weapon_c_delear)
      ACS_GiveInventory("Lith_DelearSpriteDisplay", 1);
}

//
// Lith_AmmoRunOut
//
[[__call("ScriptS"), __extern("ACS")]]
fixed Lith_AmmoRunOut(bool ro, fixed mul)
{
   player_t *p = LocalPlayer;
   if(NoPlayer(p)) return 0;
   __str cl  = p->weapon.cur->magclass;
   fixed inv = ACS_CheckInventory(cl) / (fixed)ACS_GetMaxInventory(0, cl);
   mul = mul ? mul : 1.2;
   if(ro) inv = inv * mul;
   else   inv = mul - inv * 0.35;
   return minmax(inv, 0.0, 1.0);
}

//
// Lith_GetFinalizerMaxHealth
//
[[__call("ScriptS"), __extern("ACS")]]
int Lith_GetFinalizerMaxHealth(void)
{
   int sh = ACS_GetActorProperty(0, APROP_SpawnHealth);
   ifauto(dmon_t *, m, DmonPtr())
      return sh + (m->maxhealth - sh) * 0.5;
   else
      return sh;
}

//
// Lith_SwitchRifleMode
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_SwitchRifleFiremode(void)
{
   int max = rifle_firemode_max;

   withplayer(LocalPlayer)
   {
      if(!p->getUpgr(UPGR_RifleModes)->active)
         max--;

      p->riflefiremode = (++p->riflefiremode) % max;
      ACS_LocalAmbientSound("weapons/rifle/firemode", 127);
   }
}

//
// Lith_ResetRifleMode
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_ResetRifleMode()
{
   withplayer(LocalPlayer)
      if(p->getCVarI("lith_weapons_riflemodeclear"))
         p->riflefiremode = rifle_firemode_auto;
}

//
// Lith_Blade
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_Blade(bool hit)
{
   ACS_SetHudSize(800, 600);
   DrawSpriteX(hit ? "lgfx/Weapon/BladeHit.png" : "lgfx/Weapon/Blade.png", HUDMSG_FADEOUT|HUDMSG_ADDBLEND, hid_blade, 0.1, 0.1, TICSECOND * 3, 0.15);
}

//
// Lith_Rend
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_Rend(bool hit, int set)
{
   static int num;
   if(!hit) num = set;
   __str graphic = hit ? StrParam("lgfx/Weapon/RendHit%i.png", num)
                       : StrParam("lgfx/Weapon/Rend%i.png", num);
   ACS_SetHudSize(800, 600);
   DrawSpriteX(graphic, HUDMSG_FADEOUT|HUDMSG_ADDBLEND, hid_blade, 0.1, 0.1, TICSECOND * 2, 0.1);
}

//
// Lith_Feuer
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_Feuer(bool left, bool fire)
{
   withplayer(LocalPlayer)
   {
      if(fire) {
         ACS_FadeTo(255, 255, 225, 0.3, 0);
         ACS_FadeTo(255, 255, 225, 0, 0.5);
      }

      __str actor = fire ? "Lith_FeuerExplosion" : "Lith_FeuerTest";
      int pufftid;
      ACS_LineAttack(0, p->yaw, p->pitch, 0, "Lith_Dummy", "Lith_NoDamage", 1024, FHF_NORANDOMPUFFZ|FHF_NOIMPACTDECAL, pufftid = ACS_UniqueTID());

      int sx = p->x;
      int sy = p->y;
      int sz = p->z + 32;
      int ex = ACS_GetActorX(pufftid);
      int ey = ACS_GetActorY(pufftid);
      int ez = ACS_GetActorZ(pufftid);

      struct polar cpp = ctopol(ex - sx, ey - sy);
      cpp.dst /= 4;
      if(left) cpp.ang += 0.07;
      else     cpp.ang -= 0.07;

      int cx = sx + ACS_Cos(cpp.ang) * cpp.dst;
      int cy = sy + ACS_Sin(cpp.ang) * cpp.dst;
      fixed max = fire ? 20 : 70;

      for(int i = 0; i < max; i++) {
         struct vec2i v = qbezieri(sx, sy, cx, cy, ex, ey, i / max);
         int tid;
         ACS_SpawnForced(actor, v.x, v.y, lerpk(sz, ez, i / max), tid = ACS_UniqueTID());
         if(fire) {
            Lith_SetPointer(tid, AAPTR_DEFAULT, AAPTR_TARGET, p->tid);
            ACS_Delay(1);
         }
      }

      if(fire) {
         int tid;
         ACS_SpawnForced("Lith_FeuerFinal", ex, ey, ez, tid = ACS_UniqueTID());
         Lith_SetPointer(tid, AAPTR_DEFAULT, AAPTR_TARGET, p->tid);
      }
   }
}

//
// Lith_SurgeOfDestiny
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_SurgeOfDestiny(void)
{
   for(int i = 0; i < (35 * 7) / 2; i++) {
      ACS_GiveInventory("Lith_SurgeOfDestiny", 1);
      ACS_Delay(2);
   }
}

//
// Lith_Cercle
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_Cercle(void)
{
   withplayer(LocalPlayer)
   {
      fixed ax, ay, az;

      __with(int pufftid;)
      {
         ACS_LineAttack(0, p->yaw, p->pitch, 0, "Lith_Dummy", "Lith_NoDamage", 1024,
            FHF_NORANDOMPUFFZ|FHF_NOIMPACTDECAL, pufftid = ACS_UniqueTID());

         ax = ACS_GetActorX(pufftid);
         ay = ACS_GetActorY(pufftid);
         az = ACS_GetActorFloorZ(pufftid);
      }

      world.freeze(true);
      ACS_Delay(2); // necessary so sounds may play

      ACS_AmbientSound("weapons/cercle/begin", 127);

      int fxtid = ACS_UniqueTID();
      for(int i = 0; i < 100; i++)
      {
         fixed px = ACS_Cos(i / 100.0) * 77;
         fixed py = ACS_Sin(i / 100.0) * 77;
         int tid;

         ACS_SpawnForced("Lith_CircleParticle", ax + px, ay + py, az + 7, tid = ACS_UniqueTID());

         ACS_SetActorAngle(tid, i / 100.0);
         Lith_SetPointer(tid, AAPTR_DEFAULT, AAPTR_TARGET, p->tid);
         ACS_Thing_ChangeTID(tid, fxtid);

         ACS_Delay(i % 2 * (i / 30.0));
      }

      ACS_Delay(35);

      int fxtid2 = ACS_UniqueTID();
      ACS_SetUserVariable(fxtid, "user_trigger", true);
      ACS_SetUserVariable(fxtid, "user_fxtid", fxtid2);

      ACS_AmbientSound("weapons/cercle/attack", 127);

      ACS_Delay(35);
      ACS_SetUserVariable(fxtid2, "user_trigger", true);

      ACS_Delay(7);
      ACS_SetUserVariable(fxtid2, "user_trigger", true);

      ACS_Delay(35);

      int fxtid3 = ACS_UniqueTID();
      for(int i = 0; i < 3; i++)
      {
         fixed px = ACS_Cos(i / 3.0) * 60;
         fixed py = ACS_Sin(i / 3.0) * 60;
         int tid;

         ACS_SpawnForced("Lith_CircleSpearThrower", ax + px, ay + py, az + 24, tid = ACS_UniqueTID());

         ACS_SetActorAngle(tid, i / 3.0);
         Lith_SetPointer(tid, AAPTR_DEFAULT, AAPTR_TARGET, p->tid);
         ACS_Thing_ChangeTID(tid, fxtid3);

         ACS_Delay(7);
      }

      ACS_Delay(10);

      // NB: The projectiles take the TIDs of the throwers, so this is actually triggering them.
      ACS_SetUserVariable(fxtid3, "user_trigger", true);

      // Just in case.
      ACS_Thing_Remove(fxtid);
      ACS_Thing_Remove(fxtid2);

      world.freeze(false);
   }
}

//
// Lith_GetWRF
//
[[__call("ScriptS"), __extern("ACS")]]
int Lith_GetWRF(void)
{
   enum {
      WRF_NOBOB = 1,
      WRF_NOSWITCH = 2,
      WRF_NOPRIMARY = 4,
      WRF_NOSECONDARY = 8,
      WRF_NOFIRE = WRF_NOPRIMARY | WRF_NOSECONDARY,
      WRF_ALLOWRELOAD = 16,
      WRF_ALLOWZOOM = 32,
      WRF_DISABLESWITCH = 64,
      WRF_ALLOWUSER1 = 128,
      WRF_ALLOWUSER2 = 256,
      WRF_ALLOWUSER3 = 512,
      WRF_ALLOWUSER4 = 1024
   };

   player_t *p = LocalPlayer;
   if(NoPlayer(p)) return 0;

   int flags = 0;
   if(p->semifrozen)           flags |= WRF_NOFIRE;
   if(p->pclass == pcl_marine) flags |= WRF_ALLOWUSER4;
   return flags;
}

// EOF
