// Copyright © 2016-2017 Alison Sanderson, all rights reserved.
#include "u_common.h"

// Extern Functions ----------------------------------------------------------|

script
void Upgr_Implying_Update(struct player *p, struct upgrade *upgr)
{
   static str strings[] = {
      s"\Cd>implying",
      s"\Cd>doombabbies",
      s"\Cd>implying",
      s"\Cd>doom shitters",
      s"\Cd>>>>>>>clip",
      s"\Cd>implying",
      s"\CjReport and ignore.",
      s"\Cjcaleb when?",
      s"\Cd>implying",
      s"\Cd>",
      s"\Cd>>>",
      s"\Cd>>>>>>",
      s"\Cjis this compatible with brutal doom?",
      s"\Cd>>>>>>>>>",
      s"\Cd>>>>>>>>>>>>",
      s"\Cd>>>>>>>>>>>>>>>",
      s"\Cd>doom >2012",
      s"\Cq<",
   };

   srand(p->ticks / 7);

   for(i32 i = 0; i < 40; i++)
      PrintTextX_str(strings[rand() % countof(strings)], s_dbigfont, 0, rand() % 320,0, rand() % 240,0);
}

// EOF

