// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Flashlight upgrade.                                                      │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "u_all.h"

#include <math.h>

#define udata pl.upgrdata.flashlight

void Upgr_Flashlight_Enter(void) {
   udata.on        = false;
   udata.battery   = 0xffff;
   udata.intensity = 0.0;
   udata.target    = 0.0;
   udata.speed     = 1.0;
}

void Upgr_Flashlight_Deactivate(void) {
   udata.on        = false;
   udata.intensity = 0.0;
   udata.target    = 0.0;
   udata.speed     = 1.0;
}

void Upgr_Flashlight_Update(void) {
   i32 bat_life  = CVarGetI(sc_light_battery) * 35;
   i32 bat_regen = CVarGetI(sc_light_regen);

   bool was_on = udata.on;

   if(udata.was_on != udata.on) {
      AmbientSound(udata.on ? ss_player_lighton : ss_player_lightoff, 1.0);

      if(udata.was_on) {
         udata.target = 0.0;
         udata.speed  = 0.05;
      } else {
         udata.target = 1.0;
         udata.speed  = 0.15;
      }
   }

   if(bat_life != 0) {
      if(udata.on) {udata.battery--;}
      else         {udata.battery += bat_regen;}

      if(udata.battery > bat_life) {
         udata.battery = bat_life;
      } else if(udata.battery < 0) {
         udata.battery = 0;
         udata.on = false;
      } else if(udata.battery < bat_life / 4 && ACS_Timer() % 10 == 0) {
         udata.intensity = ACS_RandomFixed(0.25, 0.75);
      }
   }

   if(udata.intensity != udata.target) {
      udata.intensity = lerplk(udata.intensity, udata.target, udata.speed);
   }

   udata.was_on = was_on;
}

script_str ext("ACS") addr(OBJ "GetFlashlightIntensity")
k32 Z_GetFlashlightIntensity(void) {
   return udata.intensity;
}

script_str type("net") ext("ACS") addr(OBJ "KeyLight")
void Z_KeyLight(void) {
   udata.on = !udata.on;
}

/* EOF */
