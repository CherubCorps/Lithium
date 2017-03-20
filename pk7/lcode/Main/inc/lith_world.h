#ifndef LITH_WORLD_H
#define LITH_WORLD_H

#include <stdbool.h>

enum
{
   skill_tourist,
   skill_easy,
   skill_normal,
   skill_hard,
   skill_extrahard,
   skill_nightmare
};

typedef struct payoutinfo_s
{
   int killnum, killmax;
   int itemnum, itemmax;
   
   fixed64_t killpct;
   fixed64_t itempct;
   
   int killscr;
   int itemscr;
   
   int total;
   int tax;
} payoutinfo_t;

__str Lith_CanonTime(bool shorttime);

enum game_s
{
   Game_Doom2,
   Game_Episodic,
};

typedef struct worldinfo_s
{
   property mapsecrets {get: ACS_GetLevelInfo(LEVELINFO_FOUND_SECRETS)}
   property mapkills   {get: ACS_GetLevelInfo(LEVELINFO_KILLED_MONSTERS)}
   property mapitems   {get: ACS_GetLevelInfo(LEVELINFO_FOUND_ITEMS)}
   property mapnum     {get: ACS_GetLevelInfo(LEVELINFO_LEVELNUM)}
   property mapkillmax {get: ACS_GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)}
   property mapitemmax {get: ACS_GetLevelInfo(LEVELINFO_TOTAL_ITEMS)}
   property cluster    {get: ACS_GetLevelInfo(LEVELINFO_CLUSTERNUM)}
   property canontime  {get: Lith_CanonTime(false)}
   property game       {get: ACS_GetCVar("__lith_game")}
   property canontimeshort {get: Lith_CanonTime(true)}
   
   int prevcluster;
   int mapseed;
   
   int secretsfound;
   double scoremul;
   
   long ticks;
   
   // Mod compat
   bool legendoom;
} worldinfo_t;

__addrdef extern __mod_arr Lith_MapVariable;
__addrdef extern __hub_arr Lith_WorldVariable;

extern bool Lith_MapVariable mapinit;

extern worldinfo_t world;
extern payoutinfo_t payout;

[[__optional_args(1)]] int Lith_UniqueID(int tid);

#endif

