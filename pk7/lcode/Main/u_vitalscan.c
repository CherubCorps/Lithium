#include "lith_upgrades_common.h"
#include "lith_world.h"
#include "lith_monster.h"

#define UData UData_VitalScan(upgr)


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Update
//
[[__call("ScriptS")]]
void Upgr_VitalScan_Update(player_t *p, upgrade_t *upgr)
{
   ACS_SetActivator(0, AAPTR_PLAYER_GETTARGET);
   
   bool six = ACS_StrCmp(ACS_GetActorClass(0), "RLDeVileSix", 11) == 0;
   
   bool validtarget =
      six ||
      ACS_CheckFlag(0, "COUNTKILL") ||
      ACS_PlayerNumber() != -1;
   
   if(ACS_GetActorProperty(0, APROP_Health) <= 0)
      UData.target = UData.oldtarget = 0;
   
   else if(validtarget)
   {
      bool legendary = world.legendoom && ACS_CheckInventory("LDLegendaryMonsterToken");
      bool henshin   = world.legendoom && ACS_CheckInventory("LDLegendaryMonsterTransformed");
      
      bool phantom = ACS_CheckInventory("Lith_IsPhantom");
      
      bool freak  = ACS_CheckFlag(0, "INVULNERABLE") || ACS_CheckFlag(0, "NODAMAGE");
      bool boss   = ACS_CheckFlag(0, "BOSS");
      bool shadow = ACS_CheckFlag(0, "SHADOW");
      
      int id = Lith_UniqueID();
      dmon_t *m = DmonPtr();
      
      if((freak || boss) && !phantom)
      {
         extern __str Lith_RandomName(int id);
         
         UData.tagstr    = Lith_RandomName(freak ? 0 : id);
         UData.oldhealth = UData.health = ACS_Random(0, 666666);
         UData.maxhealth = ACS_Random(0, 666666);
      }
      else
      {
         char color = Lith_GetPCVarInt(p, "lith_scanner_color") & 0x7F;
         
              if(six)     UData.tagstr = "\Cg6";
         else if(henshin) UData.tagstr = StrParam("\CgLegendary\C%c %tS", color, 0);
         else if(phantom) UData.tagstr = StrParam("\Cg%tS", 0);
         else             UData.tagstr = StrParam("\C%c%tS", color, 0);
         
         UData.oldhealth = UData.health;
         UData.health    = ACS_GetActorProperty(0, APROP_Health);
         UData.maxhealth = ACS_GetActorProperty(0, APROP_SpawnHealth);
      }

      if(m)
      {
         int level = shadow ? m->level - ACS_Random(-5, 5) : m->level;
         UData.tagstr = StrParam("%S lv.%i", UData.tagstr, level);
         UData.rank = m->rank;
      }
      
      UData.freak = six || freak || phantom || boss;
      UData.angle = atan2f(p->y - ACS_GetActorY(0), p->x - ACS_GetActorX(0));
      
      if((UData.oldtarget = UData.target) != (UData.target = id))
         UData.oldhealth = UData.health;
   }
}

//
// Render
//
void Upgr_VitalScan_Render(player_t *p, upgrade_t *upgr)
{
   if(!p->hudenabled || !UData.target) return;
   
   int ox = Lith_GetPCVarInt(p, "lith_scanner_xoffs");
   int oy = Lith_GetPCVarInt(p, "lith_scanner_yoffs");
   
   if(Lith_GetPCVarInt(p, "lith_scanner_slide"))
   {
      float diff = p->yawf - UData.angle;
      ox += (UData.old = lerpf(UData.old, atan2f(sinf(diff), cosf(diff)), 0.1)) * 64;
   }
   
   if(UData.health < UData.oldhealth)
   {
      int delta = UData.oldhealth - UData.health;
      
      HudMessageF("CBIFONT", "-%i", delta);
      HudMessageParams(HUDMSG_FADEOUT, hid_vitalscanhitS, CR_RED, 160.4 + ox, 196.2 + oy, 0.1, 0.4);
      
      for(int i = 1; i < 5 && delta >= 100 * i; i++)
      {
         HudMessageF("CBIFONT", "-%i", delta);
         HudMessageParams(HUDMSG_FADEOUT|HUDMSG_ADDBLEND, hid_vitalscanhitS - i, CR_RED, 160.4 + ox, 196.2 + oy, 0.1, 0.4);
      }
   }
   
   __str font = Lith_GetPCVarInt(p, "lith_scanner_altfont") ? "SMALLFONT" : "CBIFONT";
   
   if(UData.rank)
   {
      for(int i = 1; i <= UData.rank; i++)
      {
         DrawSpriteFade(StrParam("lgfx/UI/Rank%i.png", i), hid_vitalscanrankS - (i - 1),
            108.1 + ox + (i * 6), 181.1 + oy, 0.1, 0.1);
      }
   }
   
   HudMessageF(font, "%S", UData.tagstr);
   HudMessageParams(HUDMSG_FADEOUT, hid_vitalscannertag, CR_WHITE, 160.4 + ox, 180.2 + oy, 0.1, 0.4);
   
   HudMessageF(UData.freak ? "ALIENFONT" : font, "%i/%i", UData.health, UData.maxhealth);
   HudMessageParams(HUDMSG_FADEOUT, hid_vitalscannerhp, CR_WHITE, 160.4 + ox, 188.2 + oy, 0.1, 0.4);
}

// EOF

