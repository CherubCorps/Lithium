// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ CyberLegs upgrade.                                                       │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "u_all.h"

void Upgr_CyberLegs_Update(void) {
   pl.speedmul  += 20;
   pl.jumpboost += 50;

   k32 absvel = fastabsk(pl.old.velz) * 10.0k;

   if(pl.velz == 0 && absvel > 160) {
      for(k32 i = absvel; i >= 100; i -= 100) {
         i32 tid;
         ACS_SpawnForced(so_ExplodoBoots, pl.x, pl.y, pl.z, tid = ACS_UniqueTID());
         ACS_SetActivator(tid);
         ACS_SetPointer(AAPTR_TARGET, pl.tid);
         pl.setActivator();
      }
   }
}

/* EOF */
