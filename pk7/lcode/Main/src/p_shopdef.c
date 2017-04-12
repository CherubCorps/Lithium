#include "lith_common.h"
#include "lith_shopdef.h"
#include "lith_player.h"
#include "lith_world.h"


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Lith_ShopGetCost
//
score_t Lith_ShopGetCost(player_t *p, shopdef_t const *def)
{
   return PlayerDiscount(def->cost);
}

//
// Lith_ShopCanBuy
//
bool Lith_ShopCanBuy(player_t *p, shopdef_t const *def, void *obj)
{
   return
      p->score - Lith_ShopGetCost(p, def) >= 0 &&
      (def->shopCanBuy ? def->shopCanBuy(p, def, obj) : true);
}

//
// Lith_ShopBuy
//
bool Lith_ShopBuy(player_t *p, shopdef_t const *def, void *obj, __str namefmt, bool nodelivery)
{
   if(!Lith_ShopCanBuy(p, def, obj))
      return false;
   
   p->logF("> Bought %S", Language(namefmt, def->name));
   
   if(def->bipunlock)
      p->bip.unlock(def->bipunlock);
   
   p->takeScore(Lith_ShopGetCost(p, def));
   
   bool delivered = false;
   
   if(!nodelivery && Lith_GetPCVarInt(p, "lith_player_teleshop"))
   {
      int pufftid;
      int tid;
      
      ACS_LineAttack(0, p->yaw, p->pitch, 0, "Lith_Dummy", "Lith_NoDamage", 128.0, FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, pufftid = ACS_UniqueTID());
      
      fixed x = ACS_GetActorX(pufftid);
      fixed y = ACS_GetActorY(pufftid);
      fixed z = ACS_GetActorZ(pufftid);
      
      if((x || y || z) && ACS_Spawn("Lith_BoughtItem", x, y, z, tid = ACS_UniqueTID()))
      {
         p->logH("> \CjItem delivered.");
         def->shopGive(tid, def, obj);
         delivered = true;
      }
      else
         p->logH("> \CgCouldn't deliver item\C-, placing directly in inventory.");
   }
   
   if(!delivered) def->shopBuy(p, def, obj);
   
   return true;
}

// EOF

