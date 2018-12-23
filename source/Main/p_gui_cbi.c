// Copyright © 2017-2018 Alison Sanderson, all rights reserved.
#include "lith_common.h"
#include "lith_player.h"
#include "lith_world.h"

StrEntOFF

#define Upgr(name) if(world.cbiupgr[cupg_##name])

#define InfoStart int y  = 70
#define InfoSep       y += 20

#define Info(...) \
   do { \
      char text[128]; \
      int l = sprintf(text, __VA_ARGS__); \
      PrintTextChr(text, l); \
      PrintText(s"cbifont", CR_WHITE, 23,1, y,1); \
      y += 10; \
   } while(0)

#define Slot(name, x, y) PrintSprite(s":UI:" name, 300-x*48,2, 48*y-20,1)
#define CPU(num) PrintSprite(s":UI:CPU" #num, 0,1, 0,1)

// Static Functions ----------------------------------------------------------|

static void CBITab_Marine(gui_state_t *g, struct player *p)
{
   int ram;
   char const *name;

        Upgr(hasupgr2) {CPU(1); ram = 150; name = LC(LANG "CBI_CPU3");}
   else Upgr(hasupgr1) {CPU(2); ram = 100; name = LC(LANG "CBI_CPU2");}
   else                {CPU(3); ram =  50; name = LC(LANG "CBI_CPU1");}

   PrintTextChr(name, strlen(name));
   PrintText(s"cbifont", CR_WHITE, 20,1, 60,1);

   InfoStart;

   Info(LC(LANG "CBI_PERF"), world.cbiperf);
   Info(LC(LANG "CBI_PUSE"), p->cbi.pruse);
   Info(LC(LANG "CBI_WRAM"), ram);

   InfoSep;

   Info(LC(LANG "CBI_INTERFACES"));
   Upgr(armorinter) Info("> %s", LC(LANG "CBI_ArmorInter"));
   Upgr(weapninter) Info("> %s", LC(LANG "CBI_WeapnInter"));
   Upgr(weapninte2) Info("> %s", LC(LANG "CBI_WeapnInte2"));
   Upgr(rdistinter) Info("> %s", LC(LANG "CBI_RDistInter"));

   Upgr(armorinter) Slot("ArmorInter", 0, 1);
   Upgr(weapninter) Slot("WeapnInter", 0, 2);
   Upgr(weapninte2) Slot("WeapnInte2", 0, 3);
   Upgr(rdistinter) Slot("RDistInter", 0, 4);
}

static void CBITab_CyberMage(gui_state_t *g, struct player *p)
{
   char const *name = LC(LANG "CBI_CPU4");

   CPU(2);
   PrintTextChr(name, strlen(name));
   PrintText(s"cbifont", CR_WHITE, 20,1, 60,1);

   InfoStart;

   Info(LC(LANG "CBI_PERF"), 34);
   Info(LC(LANG "CBI_PUSE"), p->cbi.pruse);
   Info(LC(LANG "CBI_WRAM"), 19);

   InfoSep;

   Info(LC(LANG "CBI_INTERFACES"));
                      Info("> %s", LC(LANG "CBI_Slot1Spell"));
                      Info("> %s", LC(LANG "CBI_Slot2Spell"));
   Upgr(c_slot3spell) Info("> %s", LC(LANG "CBI_Slot3Spell"));
   Upgr(c_slot4spell) Info("> %s", LC(LANG "CBI_Slot4Spell"));
   Upgr(c_slot5spell) Info("> %s", LC(LANG "CBI_Slot5Spell"));
   Upgr(c_slot6spell) Info("> %s", LC(LANG "CBI_Slot6Spell"));
   Upgr(c_slot7spell) Info("> %s", LC(LANG "CBI_Slot7Spell"));
   Upgr(c_rdistinter) Info("> %s", LC(LANG "CBI_RDistInter"));

                      Slot("Slot1Spell", 0, 1);
                      Slot("Slot2Spell", 0, 2);
   Upgr(c_slot3spell) Slot("Slot3Spell", 0, 3);
   Upgr(c_slot4spell) Slot("Slot4Spell", 0, 4);
   Upgr(c_slot5spell) Slot("Slot5Spell", 1, 1);
   Upgr(c_slot6spell) Slot("Slot6Spell", 1, 2);
   Upgr(c_slot7spell) Slot("Slot7Spell", 1, 3);
   Upgr(c_rdistinter) Slot("RDistInter", 1, 4);
}

// Extern Functions ----------------------------------------------------------|

void Lith_CBITab_CBI(gui_state_t *g, struct player *p)
{
   switch(p->pclass) {
   case pcl_marine:    CBITab_Marine   (g, p); break;
   case pcl_cybermage: CBITab_CyberMage(g, p); break;
   }
}

// EOF
