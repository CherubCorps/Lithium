// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ World entry points.                                                      │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "common.h"
#include "p_player.h"
#include "w_world.h"
#include "w_monster.h"
#include "m_version.h"

__addrdef __mod_arr lmvar;

noinit struct world            wl;
noinit struct map_locals lmvar ml;
noinit struct cvars            cv;

script void SpawnBosses(score_t sum, bool force);

i32 UniqueID(i32 tid) {
   i32 pn;

   /* Negative values are for players. */
   if((pn = PtrPlayerNumber(tid)) != -1) return -(pn + 1);

   /* If we already have a unique identifier, return that. */
   i32 id = PtrInvNum(tid, so_UniqueID);

   /* Otherwise we have to give a new unique identifier. */
   if(id == 0) PtrInvGive(tid, so_UniqueID, id = ++ml.id);

   return id;
}

static void CheckModCompat(void) {
   i32 tid;

   if(EDataI(_edt_legendoom))              set_bit(wl.compat, _comp_legendoom);
   if(EDataI(_edt_rampancy))               set_bit(wl.compat, _comp_rampancy);
   if(CVarGetI(sc_drla_is_using_monsters)) set_bit(wl.compat, _comp_drla);
   if(EDataI(_edt_colorfulhell))           set_bit(wl.compat, _comp_ch);
}

static void UpdateGame(void) {
   i32 ver_num = version_name_to_num(tmpstr(CVarGetS(sc_version)));

   if(ver_num < vernum_1_5_2_0 && CVarGetI(sc_sv_difficulty) == 1) {
      CVarSetI(sc_sv_difficulty, 10);
   }

   if(ver_num < vernum_1_6_0_0 && CVarGetK(sc_player_footstepvol) == 1.0k) {
      CVarSetK(sc_player_footstepvol, 0.2k);
   }

   if(ver_num < vernum_1_6_1_0 && CVarGetK(sc_weapons_zoomfactor) == 3.0k) {
      CVarSetK(sc_weapons_zoomfactor, 1.5);
   }

   if(ver_num < vernum_1_6_3_0 && CVarGetI(sc_xhair_style) >= 10) {
      CVarSetI(sc_xhair_style, 0);
   }

   CVarSetS(sc_version, st_ver);
}

script static void GInit(void) {
   Dbg_Log(log_dev, _l(__func__));
   CheckModCompat();
   UpdateGame();
   wl.cbiperf = 10;
   Mon_Init();
   Wep_GInit();
   wl.init = true;
}

script static void MInit(void) {
   Dbg_Log(log_dev, _l(__func__));
   /* Init a random seed for the map. */
   ml.seed = ACS_Random(0, INT32_MAX);
   /* Set the air control because ZDoom's default sucks. */
   ACS_SetAirControl(0.77);
   /* Set up everything else about the map. -T */
   if(ACS_GameType() == GAME_TITLE_MAP) {
      set_msk(ml.flag, _mapf_kind, _mapk_title);
   } else if(MapNum == LithMapEnd) {
      set_msk(ml.flag, _mapf_kind, _mapk_end);
   } else {
      set_msk(ml.flag, _mapf_kind, _mapk_normal);
   }
   ml.soulsfreed = 0;
   srand(ml.seed);
   ml.lump = strp(ACS_PrintName(PRINTNAME_LEVEL));
   ml.name = strp(ACS_PrintName(PRINTNAME_LEVELNAME));
   ml.boss = EDataI(_edt_bosslevel);
   if(MapNum >= LithMapBeg && MapNum <= LithMapEnd) {
      set_msk(ml.flag, _mapf_cat,  _mapc_lithium);
      set_msk(ml.flag, _mapf_boss, _mapb_nospawn);
   }
   if(cv.sv_nobosses) {
      set_msk(ml.flag, _mapf_boss, _mapb_nospawn);
   }
   if(ml.boss == boss_iconofsin && GetFun() & lfun_tainted) {
      set_bit(ml.flag, _mapf_corrupted);
   }
   /* TODO: check for map's built-in dewpoint and temp */
   i32 dewpoint    = rand() % 11 - 1;
   ml.temperature  = rand() % 100;
   ml.humidity     = ml.temperature + dewpoint;
   ml.humidity     = mini(ml.humidity * ml.humidity / 90, 100);
   ml.temperature -= 30;
   /* TODO: check for map's built-in thunder and snow settings */
   i32 thunder_chk = rand() % 99;
   i32    snow_chk = rand() % 99;
   if(ml.humidity > 0) {
      if(ml.temperature >= 12 + dewpoint && thunder_chk < 33) {
         set_bit(ml.flag, _mapf_thunder);
      }
      switch(CVarGetI(sc_sv_rain)) {
      case 1:
         if(ml.humidity > 60 + dewpoint) {
         case 2:
            set_msk(ml.flag, _mapf_rain, _mapr_rain);
         } else if(ml.temperature <= 0 && snow_chk < 11) {
         case 3:
            set_msk(ml.flag, _mapf_rain, _mapr_snow);
         }
      }
   } else {
      set_bit(ml.flag, _mapf_vacuum);
      ml.humidity    = 0;
      ml.temperature = -270;
   }
   /* TODO: check for map's sky setting instead */
   if(CVarGetI(sc_sv_sky) && get_msk(ml.flag, _mapf_cat) != _mapc_lithium) {
      set_bit(ml.flag, _mapf_skyreplace);
   }
   str sky = fast_strupper(EDataS(_edt_sky1));
   if(sky == sp_SKY2 || sky == sp_RSKY2) {
      set_msk(ml.flag, _mapf_cat, _mapc_interstice);
   } else if(sky == sp_SKY3 || sky == sp_SKY4 || sky == sp_RSKY3) {
      set_msk(ml.flag, _mapf_cat, _mapc_hell);
   }
   Dlg_MInit();
   SpawnBosses(pl.scoresum, false);
   if(get_bit(ml.flag, _mapf_skyreplace)) {
      if(get_msk(ml.flag, _mapf_cat) == _mapc_hell) {
         ACS_ChangeSky(sp_LITHSKRD, sp_LITHSKRD);
         ACS_SetSkyScrollSpeed(1, 0.01);
      } else if(get_msk(ml.flag, _mapf_cat) == _mapc_interstice) {
         ACS_ChangeSky(sp_LITHSKDE, sp_LITHSKDE);
      } else {
         ACS_ChangeSky(sp_LITHSKS1, sp_LITHSKS1);
      }
   }
}

static void MInitPst(void) {
   Dbg_Log(log_dev, _l(__func__));

   if(wl.hubscleared != 0) {
      Scr_MInit();
   }

   if(pl.pclass & pcl_outcasts) {
      if(wl.hubscleared == 10) P_BIP_Unlock(P_BIP_NameToPage("MCluster1"), false);
      if(wl.hubscleared == 20) P_BIP_Unlock(P_BIP_NameToPage("MCluster2"), false);
      if(wl.hubscleared == 25) P_BIP_Unlock(P_BIP_NameToPage("MCluster3"), false);
   }

   /* TODO: figure out what to do with these in a hub setup */
   wl.pay.par = ACS_GetLevelInfo(LEVELINFO_PAR_TIME) * 35;

   wl.pay.killmax += ACS_GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
   wl.pay.itemmax += ACS_GetLevelInfo(LEVELINFO_TOTAL_ITEMS);
   wl.pay.scrtmax += ACS_GetLevelInfo(LEVELINFO_TOTAL_SECRETS);
}

dynam_aut script void W_World(void) {
   Dbg_Log(log_dev, _l(__func__));
   MInitPst();
   P_Player();
   /* Main loop. */
   for(;;) {
      if(ACS_Timer() > cv.sv_failtime * 35 * 60 * 60) {
         F_Start(_finale_time_out);
         return;
      }

      #define cvar_tic(ty, na) cv.na = cvar_get(na);
      #define cvar_x(ev, na, ty) cvar_##ev(ty, na)
      #include "common.h"

      i32 scrts = ACS_GetLevelInfo(LEVELINFO_FOUND_SECRETS);
      i32 kills = ACS_GetLevelInfo(LEVELINFO_KILLED_MONSTERS);
      i32 items = ACS_GetLevelInfo(LEVELINFO_FOUND_ITEMS);

      if(kills > ml.prevkills)  wl.pay.killnum  += kills - ml.prevkills;
      if(items > ml.previtems)  wl.pay.itemnum  += items - ml.previtems;
      if(scrts > ml.prevscrts) {wl.pay.scrtnum  += scrts - ml.prevscrts;
                                wl.secretsfound += scrts - ml.prevscrts;}

      ml.prevscrts = scrts;
      ml.prevkills = kills;
      ml.previtems = items;

      if(ACS_Timer() % 5 == 0 && ml.missionkill < kills) {
         if(++ml.missionprc >= 150) {
            SpawnBosses(0, true);
            ml.missionprc = 0;
         }
         ml.missionkill = kills;
      }

      Dbg_Stat(_l("mission%: "), _p(ml.missionprc), _c('\n'));

      ACS_Delay(1);

      #ifndef NDEBUG
      wl.dbgstatnum = 0;
      #endif

      if(cv.sv_autosave && ACS_Timer() && ACS_Timer() % (35 * 60 * cv.sv_autosave) == 0) {
         ACS_Autosave();
      }
   }
}

alloc_aut(0) script static void W_WrongConfig() {
   Dbg_Log(log_dev, _l(__func__));
   for(;;) {
      SetSize(320, 240);
      SetClipW(0, 0, 320, 240, 320);
      PrintTextChL(
         "The configuration for this mod has been wiped, or you accidentally "
         "set '" CVAR "sv_failtime' to 0 manually. If you did the latter, "
         "please set it to something else. Otherwise, please follow these "
         "instructions to fix your configuration:\n"
         "\n"
         "1. Navigate to your GZDoom folder.\n"
         "2. Find the configuration settings file (if you have extensions "
         "shown it will be the ini file in the folder) and open it.\n"
         "3. Find the heading '[Doom.Player.Mod]' and delete any lines "
         "starting with '" CVAR "' or '" DCVAR "' under it.\n"
         "4. Find the heading '[Doom.LocalServerInfo.Mod]' and delete any "
         "lines starting with '" CVAR "' or '" DCVAR "' under it.\n"
         "5. Save the file and start GZDoom again. If the issue persists "
         "try these steps again or delete your GZDoom configuration.");
      PrintText(sf_ltrmfont, CR_WHITE, 0,1, 16,1);
      ClearClip();
      ACS_Delay(1);
   }
}

script ext("ACS") addr(lsc_gsinit) void GsInit(void);

alloc_aut(0) script ext("ACS") addr(lsc_worldopen) void Z_World(void) {
   if(!wl.init) {
      GsInit();
      StrInit();
      DrawInit();
      #define cvar_gbl(ty, na) cv.na = cvar_get(na);
      #define cvar_x(ev, na, ty) cvar_##ev(ty, na)
      #include "common.h"
   }

   /* TODO: handle map cvars properly */
   #define cvar_map(ty, na) cv.na = cvar_get(na);
   #define cvar_tic(ty, na) cv.na = cvar_get(na);
   #define cvar_x(ev, na, ty) cvar_##ev(ty, na)
   #include "common.h"

   Dbg_Log(log_dev, _l(__func__));

   if(CVarGetI(sc_sv_failtime) == 0) {
      W_WrongConfig();
      return;
   }

   if(!wl.init) {
      GInit();
   }
   MInit();

   /* choose main-loop function */
   switch(get_msk(ml.flag, _mapf_kind)) {
   case _mapk_title: W_Title(); break;
   case _mapk_end:   F_Run();   break;
   default:          W_World(); break;
   }
}

script type("unloading") static void Z_WorldUnload(void) {
   Dbg_Log(log_dev, _l(__func__));
   pl.setActivator();
   P_Upg_PDeinit();
   P_GUI_Close();
   P_Dat_PTickPst();
}

script ext("ACS") addr(lsc_hubclear) void Z_HubCleared(void) {
   Dbg_Log(log_dev, _l(__func__));
   wl.hubscleared++;
}

/* EOF */
