#include "lith_common.h"
#include "lith_player.h"
#include "lith_list.h"
#include "lith_hudid.h"
#include "lith_hud.h"
#include "lith_cbi.h"
#include <math.h>

player_t players[MAX_PLAYERS];

// ---------------------------------------------------------------------------
// Scripts.
//

[[__call("ScriptI")]]
static
void Lith_GetWeaponType(player_t *p)
{
   struct weaponid { __str class; int type, flag; };
   static struct weaponid const weaponids[] = {
      { "Lith_Pistol",         weapon_pistol,         weaponf_pistol         },
      { "Lith_Shotgun",        weapon_shotgun,        weaponf_shotgun        },
      { "Lith_SuperShotgun",   weapon_supershotgun,   weaponf_supershotgun   },
      { "Lith_CombatRifle",    weapon_combatrifle,    weaponf_combatrifle    },
      { "Lith_RocketLauncher", weapon_rocketlauncher, weaponf_rocketlauncher },
      { "Lith_PlasmaRifle",    weapon_plasmarifle,    weaponf_plasmarifle    },
      { "Lith_BFG9000",        weapon_bfg9000,        weaponf_bfg9000        }
   };
   
   p->weapontype = weapon_unknown;
   p->weapons = 0;
   
   for(int i = 0; i < sizeof(weaponids) / sizeof(*weaponids); i++)
   {
      struct weaponid const *id = &weaponids[i];
      
      if(ACS_CheckInventory(id->class))
         p->weapons |= id->flag;
      
      if(p->weapontype == weapon_unknown && !ACS_StrICmp(p->weaponclass, id->class))
         p->weapontype = id->type;
   }
}

[[__call("ScriptI")]]
static
void Lith_GetArmorType(player_t *p)
{
   static struct { __str class; int type; } const armorids[] = {
      { "None",                   armor_none  },
      { "ArmorBonus",             armor_bonus },
      { "GreenArmor",             armor_green },
      { "BlueArmor",              armor_blue  },
      { "BlueArmorForMegasphere", armor_blue  }
   };
   
   p->armortype = armor_unknown;
   for(int i = 0; i < sizeof(armorids) / sizeof(*armorids); i++)
      if(!ACS_StrICmp(p->armorclass, armorids[i].class))
      {
         p->armortype = armorids[i].type;
         break;
      }
}

[[__call("ScriptI")]]
static
void Lith_PlayerRender(player_t *p)
{
   if(p->lastscopetoken && !p->scopetoken)
   {
      if(p->hudstrstack)
      {
         DList_Free(p->hudstrstack);
         p->hudstrstack = null;
      }
      
      for(int i = hid_scope_clearS; i <= hid_scope_clearE; i++)
      {
         HudMessage("");
         HudMessagePlain(i, 0.0, 0.0, 0.0);
      }
   }
   else if(p->scopetoken && !p->lastscopetoken)
   {
      p->hudstrstack = DList_Create();
      
      for(int i = 0; i < hudstrstack_max; i++)
         DList_InsertBack(p->hudstrstack, (listdata_t){
            .str = StrParam("%x", Random(0x1000, 0x7FFF))
         });
   }
   
   if(p->scopetoken)
   {
      Lith_RenderHUDWaves(p);
      Lith_RenderHUDStringStack(p);
      
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Subtract);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, ACS_GetCVarFixed("lith_weapons_scopealpha"));
   }
   else
   {
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Translucent);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, ACS_GetCVarFixed("lith_weapons_alpha"));
   }
}

[[__call("ScriptI")]]
static
void Lith_PlayerMove(player_t *p)
{
   fixed grounddist = p->z - p->floorz;
   
   if(p->rocketcharge < rocketcharge_max)
      p->rocketcharge++;
   
   if(p->slidecharge < slidecharge_max)
      p->slidecharge++;
   
   if(p->slidecharge >= slidecharge_max)
   {
      if(grounddist == 0.0)
         p->leaped = false;
      
      if(p->buttons & BT_SPEED && grounddist <= 16.0)
      {
         fixed angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);
         
         ACS_PlaySound(0, "player/slide");
         ACS_SetActorVelocity(0,
            p->velx + (ACS_Cos(angle) * 24.0),
            p->vely + (ACS_Sin(angle) * 24.0),
            -30.0,
            false, true);
         
         p->slidecharge = 0;
      }
   }
   
   if(ButtonPressed(p, BT_JUMP) &&
      !ACS_CheckInventory("Lith_RocketBooster") && !p->leaped &&
      ((grounddist <= 16.0 && p->slidecharge < slidecharge_max) || grounddist > 16.0))
   {
      fixed angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);
      
      ACS_PlaySound(0, "player/doublejump");
      ACS_SetActorVelocity(0,
         p->velx + (ACS_Cos(angle) * 12.0),
         p->vely + (ACS_Sin(angle) * 12.0),
         10.0,
         false, true);
      
      p->leaped = true;
   }
   
   if(ButtonPressed(p, BT_SPEED) && grounddist > 16.0 &&
      p->rocketcharge >= rocketcharge_max)
   {
      fixed angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);
      
      ACS_PlaySound(0, "player/rocketboost");
      ACS_GiveInventory("Lith_RocketBooster", 1);
      ACS_SetActorVelocity(0,
         p->velx + (ACS_Cos(angle) * 16.0),
         p->vely + (ACS_Sin(angle) * 16.0),
         10.0,
         false, true);
      
      p->rocketcharge = 0;
      p->leaped = false;
   }
}

[[__call("ScriptI")]]
static
void Lith_PlayerDamageBob(player_t *p)
{
   if(!p->berserk && p->health < p->prevhealth)
   {
      float angle = RandomFloat(tau, -tau);
      float distance;
      
      distance = (p->prevhealth - p->health) / (float)p->maxhealth;
      distance *= 0.2f;
      distance *= (200 - p->armor) / 200.0f;
      
      p->bobyaw = sinf(angle) * distance;
      p->bobpitch = cosf(angle) * distance;
   }
   
   p->bobyaw = lerpf(p->bobyaw, 0.0f, 0.1f);
   p->bobpitch = lerpf(p->bobpitch, 0.0f, 0.1f);
}

[[__call("ScriptI")]]
static
void Lith_ResetPlayer(player_t *p)
{
   p->active = true;
   ACS_Thing_ChangeTID(0, p->tid = ACS_UniqueTID());
   
   if(!p->cbi.wasinit)
      Lith_PlayerInitCBI(p);
   
   // pls not exit map with murder thingies out
   // is bad practice
   ACS_TakeInventory("Lith_PistolScopedToken", 999);
   ACS_TakeInventory("Lith_ShotgunScopedToken", 999);
   ACS_TakeInventory("Lith_MinigunWindup", 999);
   ACS_TakeInventory("Lith_MinigunWinddown", 999);
   ACS_TakeInventory("Lith_SuperShotgunCharge", 999);
   ACS_TakeInventory("Lith_SuperShotgunChargeIter", 999);
   
   // i cri tears of pain for APROP_SpawnHealth
   if(!p->maxhealth)
      p->maxhealth = ACS_GetActorProperty(0, APROP_Health);
   
   if(p->hudstrstack)
   {
      DList_Free(p->hudstrstack);
      p->hudstrstack = null;
   }
   
   p->lastscopetoken = false;
   
   p->slidecharge = slidecharge_max;
   p->rocketcharge = rocketcharge_max;
   p->leaped = false;
   
   p->bobyaw = 0.0f;
   p->bobpitch = 0.0f;
   
   p->addyaw = 0.0f;
   p->addpitch = 0.0f;
   
   p->scoreaccum = 0;
}

[[__call("ScriptI")]]
static
void Lith_GiveSecretScore(int playernum, int mul)
{
   [[__call("ScriptS"), __extern("ACS")]]
   extern void Lith_UpdateScore();
   
   ACS_SetActivator(players[playernum].tid);
   ACS_GiveInventory("Lith_ScoreCount", 9000 * mul);
   Lith_UpdateScore();
}

[[__call("ScriptI")]]
static
void Lith_PlayerView(player_t *p)
{
   if(ACS_GetCVar("lith_player_damagebob"))
   {
      float bobmul = ACS_GetCVarFixed("lith_player_damagebobmul");
      p->addyaw = p->bobyaw * bobmul;
      p->addpitch = p->bobpitch * bobmul;
   }
}

[[__call("ScriptI")]]
static
void Lith_PlayerScore(player_t *p)
{
   if(p->health < 1)
      p->score = 0;
   
   if(!p->scoreaccumtime || p->score < p->prevscore)
   {
      p->scoreaccum = 0;
      p->scoreaccumtime = 0;
   }
   
   if(p->scoreaccumtime > 0)
      p->scoreaccumtime--;
   else if(p->scoreaccumtime < 0)
      p->scoreaccumtime++;
}

//
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Callback scripts.
//

[[__call("ScriptI"), __script("Open")]]
void Lith_World()
{
   int maxsecrets = ACS_GetLevelInfo(LEVELINFO_TOTAL_SECRETS);
   int prevsecrets = 0;
   
   ACS_Delay(1);
   
   for(;;)
   {
      int secrets = ACS_GetLevelInfo(LEVELINFO_FOUND_SECRETS);
      
      if(secrets > prevsecrets)
         for(int i = 0; i < MAX_PLAYERS; i++)
            if(players[i].active)
               Lith_GiveSecretScore(i, secrets - prevsecrets);
      
      prevsecrets = secrets;
      
      ACS_Delay(1);
   }
}

[[__call("ScriptI"), __script("Disconnect")]]
void Lith_PlayerDisconnect()
{
   player_t *p = &players[ACS_PlayerNumber()];
   p->active = false;
}

[[__call("ScriptI"), __script("Enter")]]
void Lith_Player()
{
   register player_t *p = &players[ACS_PlayerNumber()];
   
   Lith_ResetPlayer(p);
   
   while(p->active)
   {
      // Status data
      p->x = ACS_GetActorX(0);
      p->y = ACS_GetActorY(0);
      p->z = ACS_GetActorZ(0);
      p->velx = ACS_GetActorVelX(0);
      p->vely = ACS_GetActorVelY(0);
      p->velz = ACS_GetActorVelZ(0);
      p->yaw = ACS_GetActorAngle(0) - p->addyaw;
      p->pitch = ACS_GetActorPitch(0) - p->addpitch;
      p->pitchv = ACS_GetPlayerInputFixed(0, INPUT_PITCH);
      p->yawv = ACS_GetPlayerInputFixed(0, INPUT_YAW);
      p->forwardv = ACS_GetPlayerInputFixed(0, INPUT_FORWARDMOVE);
      p->sidev = ACS_GetPlayerInputFixed(0, INPUT_SIDEMOVE);
      p->upv = ACS_GetPlayerInputFixed(0, INPUT_UPMOVE);
      p->floorz = ACS_GetActorFloorZ(0);
      p->buttons = ACS_GetPlayerInput(0, INPUT_BUTTONS);
      p->oldbuttons = ACS_GetPlayerInput(0, INPUT_OLDBUTTONS);
      p->name = StrParam("%tS", 0);
      
      p->health = ACS_GetActorProperty(0, APROP_Health);
      p->armor = ACS_CheckInventory("BasicArmor");
      score_t curscore = p->score; // This can be changed any time, so save it here
      
      // Type / class
      p->weaponclass = ACS_GetWeapon();
      p->armorclass = ACS_GetArmorInfoString(ARMORINFO_CLASSNAME);
      
      Lith_GetWeaponType(p);
      Lith_GetArmorType(p);
      
      // Misc. / inventory
      p->berserk = ACS_CheckInventory("PowerStrength");
      p->scopetoken = ACS_CheckInventory("Lith_ShotgunScopedToken") ||
         ACS_CheckInventory("Lith_PistolScopedToken");
      
      p->keys = 0;
      p->keys |= ACS_CheckInventory("RedCard")     << key_red_bit;
      p->keys |= ACS_CheckInventory("YellowCard")  << key_yellow_bit;
      p->keys |= ACS_CheckInventory("BlueCard")    << key_blue_bit;
      p->keys |= ACS_CheckInventory("RedSkull")    << key_redskull_bit;
      p->keys |= ACS_CheckInventory("YellowSkull") << key_yellowskull_bit;
      p->keys |= ACS_CheckInventory("BlueSkull")   << key_blueskull_bit;
      
      // Run scripts
      
      // -- Logic
      Lith_PlayerScore(p);
      Lith_PlayerDamageBob(p);
      
      if(p->health > 0)
      {
         Lith_PlayerUpdateCBI(p);
         
         if(!p->frozen)
            Lith_PlayerMove(p);
      }
      else
         p->cbi.open = false;
      
      ACS_SetPlayerProperty(0, p->frozen > 0, PROP_TOTALLYFROZEN);
      Lith_PlayerView(p);
      
      // -- Rendering
      Lith_PlayerHUD(p);
      Lith_PlayerRender(p);
      
      if(p->cbi.open)
         Lith_PlayerDrawCBI(p);
      
      // Update view
      ACS_SetActorAngle(0, ACS_GetActorAngle(0) - p->addyaw);
      ACS_SetActorPitch(0, ACS_GetActorPitch(0) - p->addpitch);
      
      // Tic passes
      ACS_Delay(1);
      
      // Update previous-tic values
      p->lastscopetoken = p->scopetoken;
      
      p->prevhealth = p->health;
      p->prevarmor = p->armor;
      p->prevscore = curscore;
      
      // Reset view for next tic
      ACS_SetActorAngle(0, ACS_GetActorAngle(0) + p->addyaw);
      ACS_SetActorPitch(0, ACS_GetActorPitch(0) + p->addpitch);
   }
}

//
// ---------------------------------------------------------------------------

