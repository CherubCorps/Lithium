/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * ACS string table.
 *
 * ---------------------------------------------------------------------------|
 */

#if defined(stab_x)
stab_x(st_null, "(null)")
stab_x(st_none, "None")

stab_x(st_ob_crush,    "(crush)")
stab_x(st_ob_default,  "(default)")
stab_x(st_ob_drowning, "(drowning)")
stab_x(st_ob_exit,     "(exit)")
stab_x(st_ob_falling,  "(falling)")
stab_x(st_ob_fire,     "(fire)")
stab_x(st_ob_slime,    "(slime)")
stab_x(st_ob_suicide,  "(suicide)")
stab_x(st_six_tag,     "\Cg6")

/* Font Names */
stab_x(sf_areaname,  "areaname")
stab_x(sf_bigupper,  "bigupper")
stab_x(sf_italic,    "italic")
stab_x(sf_jiskan16,  "jiskan16")
stab_x(sf_k6x8,      "k6x8")
stab_x(sf_ljtrmfont, "ljtrmfont")
stab_x(sf_lmidfont,  "lmidfont")
stab_x(sf_lsmlhfnt,  "lsmlhfnt")
stab_x(sf_ltrmfont,  "ltrmfont")
stab_x(sf_misakig,   "misakig")
stab_x(sf_misakim,   "misakim")
stab_x(sf_smallfnt,  "smallfnt")

/* Console Variables */
stab_x(sc_debug_flags, DCVAR "debug_flags")
stab_x(sc_debug_level, DCVAR "debug_level")
stab_x(sc_fun,         DCVAR "fun")
stab_x(sc_version,     DCVAR "version")

stab_x(sc_gui_buyfiller, CVAR "gui_buyfiller")
stab_x(sc_gui_cursor,    CVAR "gui_cursor")
stab_x(sc_gui_defcr,     CVAR "gui_defcr")
stab_x(sc_gui_theme,     CVAR "gui_theme")
stab_x(sc_gui_xmul,      CVAR "gui_xmul")
stab_x(sc_gui_ymul,      CVAR "gui_ymul")

stab_x(sc_hud_expbar,       CVAR "hud_expbar")
stab_x(sc_hud_logcolor,     CVAR "hud_logcolor")
stab_x(sc_hud_logfromtop,   CVAR "hud_logfromtop")
stab_x(sc_hud_logsize,      CVAR "hud_logsize")
stab_x(sc_hud_showarmorind, CVAR "hud_showarmorind")
stab_x(sc_hud_showlog,      CVAR "hud_showlog")
stab_x(sc_hud_showlvl,      CVAR "hud_showlvl")
stab_x(sc_hud_showscore,    CVAR "hud_showscore")
stab_x(sc_hud_showweapons,  CVAR "hud_showweapons")
stab_x(sc_hud_type,         CVAR "hud_type")

stab_x(sc_light_battery, CVAR "light_battery")
stab_x(sc_light_regen,   CVAR "light_regen")

stab_x(sc_player_ammodisp,      CVAR "player_ammodisp")
stab_x(sc_player_bosstexts,     CVAR "player_bosstexts")
stab_x(sc_player_damagebob,     CVAR "player_damagebob")
stab_x(sc_player_damagebobmul,  CVAR "player_damagebobmul")
stab_x(sc_player_footstepvol,   CVAR "player_footstepvol")
stab_x(sc_player_itemdisp,      CVAR "player_itemdisp")
stab_x(sc_player_lvsys,         CVAR "player_lvsys")
stab_x(sc_player_pronouns,      CVAR "player_pronouns")
stab_x(sc_player_resultssound,  CVAR "player_resultssound")
stab_x(sc_player_scoredisp,     CVAR "player_scoredisp")
stab_x(sc_player_scoresound,    CVAR "player_scoresound")
stab_x(sc_player_sillypickups,  CVAR "player_sillypickups")
stab_x(sc_player_teleshop,      CVAR "player_teleshop")
stab_x(sc_player_viewtilt,      CVAR "player_viewtilt")

stab_x(sc_psave, CVAR "psave")

stab_x(sc_scanner_bar,     CVAR "scanner_bar")
stab_x(sc_scanner_color,   CVAR "scanner_color")
stab_x(sc_scanner_font,    CVAR "scanner_font")
stab_x(sc_scanner_slide,   CVAR "scanner_slide")
stab_x(sc_scanner_xoffs,   CVAR "scanner_xoffs")
stab_x(sc_scanner_yoffs,   CVAR "scanner_yoffs")

stab_x(sc_sv_autosave,   CVAR "sv_autosave")
stab_x(sc_sv_difficulty, CVAR "sv_difficulty")
stab_x(sc_sv_extrahard,  CVAR "sv_extrahard")
stab_x(sc_sv_failtime,   CVAR "sv_failtime")
stab_x(sc_sv_minhealth,  CVAR "sv_minhealth")
stab_x(sc_sv_nobossdrop, CVAR "sv_nobossdrop")
stab_x(sc_sv_nobosses,   CVAR "sv_nobosses")
stab_x(sc_sv_rain,       CVAR "sv_rain")
stab_x(sc_sv_revenge,    CVAR "sv_revenge")
stab_x(sc_sv_sky,        CVAR "sv_sky")
stab_x(sc_sv_wepdrop,    CVAR "sv_wepdrop")

stab_x(sc_weapons_magicselanims,  CVAR "weapons_magicselanims")
stab_x(sc_weapons_riflemodeclear, CVAR "weapons_riflemodeclear")
stab_x(sc_weapons_riflescope,     CVAR "weapons_riflescope")
stab_x(sc_weapons_zoomfactor,     CVAR "weapons_zoomfactor")

stab_x(sc_xhair_style, CVAR "xhair_style")

stab_x(sc_invertmouse,          "invertmouse")
stab_x(sc_sv_cooploseinventory, "sv_cooploseinventory")
stab_x(sc_sv_weaponstay,        "sv_weaponstay")
stab_x(sc_toggleconsole,        "toggleconsole")

stab_x(sc_drla_is_using_monsters, "DRLA_is_using_monsters")

stab_x(sc_attack,    "+attack")
stab_x(sc_k_opencbi, CVAR "k_opencbi")
stab_x(sc_use,       "+use")

/* Methods / Members */
stab_x(sm_ActuallyEndTheGame, "ActuallyEndTheGame")
stab_x(sm_AddItemPop,         "addItemPop")
stab_x(sm_AddScoreNum,        "addScoreNum")
stab_x(sm_AddSection,         "addSection")
stab_x(sm_AddStatFx,          "addStatFx")
stab_x(sm_AdrenalineCheck,    "AdrenalineCheck")
stab_x(sm_AlertMonsters,      "AlertMonsters")
stab_x(sm_AmmoText,           "m_ammoText")
stab_x(sm_AutoReload,         "AutoReload")
stab_x(sm_CheckLump,          "CheckLump")
stab_x(sm_DelearSprite,       "DelearSprite")
stab_x(sm_DeleteItem,         "DeleteItem")
stab_x(sm_DieMonster,         "DieMonster")
stab_x(sm_DieNow,             "DieNow")
stab_x(sm_DmgFac,             "m_DmgFac")
stab_x(sm_FindFontColor,      "FindFontColor")
stab_x(sm_FindLump,           "FindLump")
stab_x(sm_FxTID,              "m_FxTID")
stab_x(sm_GetAttackHeight,    "GetAttackHeight")
stab_x(sm_GetBipName,         "GetBipName")
stab_x(sm_GetBossLevel,       "GetBossLevel")
stab_x(sm_GetCurMag,          "GetCurMag")
stab_x(sm_GetLogName,         "GetLogName")
stab_x(sm_GetMaxMag,          "GetMaxMag")
stab_x(sm_GetObituary,        "GetObituary")
stab_x(sm_GetOutOfDodge,      "GetOutOfDodge")
stab_x(sm_GetPaused,          "GetPaused")
stab_x(sm_GetRifleGrenade,    "GetRifleGrenade")
stab_x(sm_GetSpawnHealth,     "GetSpawnHealth")
stab_x(sm_GetViewHeight,      "GetViewHeight")
stab_x(sm_GivePoison,         "GivePoison")
stab_x(sm_GrabInput,          "GrabInput")
stab_x(sm_H,                  "m_H")
stab_x(sm_HasStatFx,          "hasStatFx")
stab_x(sm_Health,             "Health")
stab_x(sm_Height,             "Height")
stab_x(sm_InvEquip,           "m_InvEquip")
stab_x(sm_InvH,               "m_InvH")
stab_x(sm_InvName,            "m_InvName")
stab_x(sm_InvSell,            "m_InvSell")
stab_x(sm_InvType,            "m_InvType")
stab_x(sm_InvW,               "m_InvW")
stab_x(sm_IonizeFX,           "IonizeFX")
stab_x(sm_IsRampancy,         "IsRampancy")
stab_x(sm_JumpZ,              "JumpZ")
stab_x(sm_LC,                 "LC")
stab_x(sm_LE,                 "LE")
stab_x(sm_LF,                 "LF")
stab_x(sm_LL,                 "LL")
stab_x(sm_LR,                 "LR")
stab_x(sm_LS,                 "LS")
stab_x(sm_LT,                 "LT")
stab_x(sm_LW,                 "LW")
stab_x(sm_LX,                 "LX")
stab_x(sm_LY,                 "LY")
stab_x(sm_LZ,                 "LZ")
stab_x(sm_Localize,           "localize")
stab_x(sm_LogText,            "m_logText")
stab_x(sm_MapNoRain,          "user_" OBJ "NoRain")
stab_x(sm_MapUseRain,         "user_" OBJ "UseRain")
stab_x(sm_MonsterBarrierLook, "MonsterBarrierLook")
stab_x(sm_Nuke,               "Nuke")
stab_x(sm_OnGround,           "m_OnGround")
stab_x(sm_Overdrive,          "m_Overdrive")
stab_x(sm_PhantomOut,         "PhantomOut")
stab_x(sm_PhantomTeleport,    "PhantomTeleport")
stab_x(sm_PlayerDeath,        "PlayerDeath")
stab_x(sm_PlayerDeathNuke,    "PlayerDeathNuke")
stab_x(sm_PlayerInit,         "PlayerInit")
stab_x(sm_PoisonFX,           "PoisonFX")
stab_x(sm_Project,            "Project")
stab_x(sm_PureSteggleEnergy,  "PureSteggleEnergy")
stab_x(sm_Radius,             "Radius")
stab_x(sm_ReadLump,           "ReadLump")
stab_x(sm_RemStatFx,          "remStatFx")
stab_x(sm_Set,                "Set")
stab_x(sm_SetFrozen,          "SetFrozen")
stab_x(sm_SetTeleFogTo,       "SetTeleFogTo")
stab_x(sm_SpawnBoss,          "SpawnBoss")
stab_x(sm_SpawnRain,          "SpawnRain")
stab_x(sm_Speed,              "Speed")
stab_x(sm_StartSound,         "StartSound")
stab_x(sm_SurgeOfDestiny,     "SurgeOfDestiny")
stab_x(sm_TraceFrom,          "TraceFrom")
stab_x(sm_Trigger,            "m_Trigger")
stab_x(sm_TriggerBoss,        "TriggerBoss")
stab_x(sm_UpgradeBody,        "m_UpgradeBody")
stab_x(sm_UpgradeExtr,        "m_UpgradeExtr")
stab_x(sm_UpgradeId,          "m_UpgradeId")
stab_x(sm_UpgradeWeap,        "m_UpgradeWeap")
stab_x(sm_UseItem,            "UseItem")
stab_x(sm_UserX,              "user_X")
stab_x(sm_UserY,              "user_Y")
stab_x(sm_UserZ,              "user_Z")
stab_x(sm_W,                  "m_W")
stab_x(sm_WaterLevel,         "WaterLevel")

stab_x(sm_boss,      "BOSS")
stab_x(sm_countkill, "COUNTKILL")
stab_x(sm_dropped,   "DROPPED")
stab_x(sm_invuln,    "INVULNERABLE")
stab_x(sm_nodamage,  "NODAMAGE")
stab_x(sm_shadow,    "SHADOW")
stab_x(sm_solid,     "SOLID")

/* Objects */
stab_x(so_AssassinPlayer,         OBJ "AssassinPlayer")
stab_x(so_BarrierFX,              OBJ "BarrierFX")
stab_x(so_BloodRainDrop,          OBJ "BloodRainDrop")
stab_x(so_BossReward1,            OBJ "BossReward1")
stab_x(so_BossReward2,            OBJ "BossReward2")
stab_x(so_BossReward3,            OBJ "BossReward3")
stab_x(so_BossReward4,            OBJ "BossReward4")
stab_x(so_BossReward5,            OBJ "BossReward5")
stab_x(so_BossReward6,            OBJ "BossReward6")
stab_x(so_BoughtItem,             OBJ "BoughtItem")
stab_x(so_BulletAmmo,             OBJ "BulletAmmo")
stab_x(so_CannonAmmo,             OBJ "CannonAmmo")
stab_x(so_Cercle,                 OBJ "Cercle")
stab_x(so_CircleParticle,         OBJ "CircleParticle")
stab_x(so_CircleSpearThrower,     OBJ "CircleSpearThrower")
stab_x(so_ClawOfImp,              OBJ "ClawOfImp")
stab_x(so_CreditsMenu,            OBJ "CreditsMenu")
stab_x(so_CyberMagePlayer,        OBJ "CyberMagePlayer")
stab_x(so_DarkLordPlayer,         OBJ "DarkLordPlayer")
stab_x(so_DivisionSigil,          OBJ "DivisionSigil")
stab_x(so_Draw,                   OBJ "URANUS")
stab_x(so_Dummy,                  OBJ "Dummy")
stab_x(so_EXPLOOOSION,            OBJ "EXPLOOOSION")
stab_x(so_ExplodoBoots,           OBJ "ExplodoBoots")
stab_x(so_Feuer,                  OBJ "Feuer")
stab_x(so_FeuerExplosion,         OBJ "FeuerExplosion")
stab_x(so_FeuerFinal,             OBJ "FeuerFinal")
stab_x(so_FeuerTest,              OBJ "FeuerTest")
stab_x(so_FistCharge,             OBJ "FistCharge")
stab_x(so_GEExplosion,            OBJ "GEExplosion")
stab_x(so_GTFO,                   OBJ "GTFO")
stab_x(so_Hulgyon,                OBJ "Hulgyon")
stab_x(so_InformantPlayer,        OBJ "InformantPlayer")
stab_x(so_Ionized,                OBJ "Ionized")
stab_x(so_ManaAmmo,               OBJ "ManaAmmo")
stab_x(so_ManaLeak,               OBJ "ManaLeak")
stab_x(so_ManaPickup,             OBJ "ManaPickup")
stab_x(so_MarinePlayer,           OBJ "MarinePlayer")
stab_x(so_MegaProtection,         OBJ "MegaProtection")
stab_x(so_MonsterBarrier,         OBJ "MonsterBarrier")
stab_x(so_MonsterFavlosaura,      OBJ "MonsterFavlosaura")
stab_x(so_MonsterFosaura,         OBJ "MonsterFosaura")
stab_x(so_MonsterHeptaura,        OBJ "MonsterHeptaura")
stab_x(so_MonsterID,              OBJ "MonsterID")
stab_x(so_MonsterLevelUp,         OBJ "MonsterLevelUp")
stab_x(so_MonsterSkotosaura,      OBJ "MonsterSkotosaura")
stab_x(so_MonsterSoul,            OBJ "MonsterSoul")
stab_x(so_Motra,                  OBJ "Motra")
stab_x(so_PlasmaAmmo,             OBJ "PlasmaAmmo")
stab_x(so_Player,                 OBJ "Player")
stab_x(so_PoisonFXReset,          OBJ "PoisonFXReset")
stab_x(so_PoisonFXTimer,          OBJ "PoisonFXTimer")
stab_x(so_PunctuatorExplosion,    OBJ "PunctuatorExplosion")
stab_x(so_PunctuatorPuff,         OBJ "PunctuatorPuff")
stab_x(so_RainDrop,               OBJ "RainDrop")
stab_x(so_RandomSpawn,            OBJ "RandomSpawn")
stab_x(so_Rend,                   OBJ "Rend")
stab_x(so_RocketAmmo,             OBJ "RocketAmmo")
stab_x(so_SMGHeat,                OBJ "SMGHeat")
stab_x(so_Serv,                   OBJ "HERMES")
stab_x(so_ShellAmmo,              OBJ "ShellAmmo")
stab_x(so_StarShot,               OBJ "StarShot")
stab_x(so_StatAdrenaline,         OBJ "StatAdrenaline")
stab_x(so_StatAdrenalineCharged,  OBJ "StatAdrenalineCharged")
stab_x(so_StatAdrenalineCharging, OBJ "StatAdrenalineCharging")
stab_x(so_StatJetBooster,         OBJ "StatJetBooster")
stab_x(so_StatJetLag,             OBJ "StatJetLag")
stab_x(so_StealthSystem,          OBJ "StealthSystem")
stab_x(so_Steggles,               OBJ "Steggles")
stab_x(so_Sword,                  OBJ "Sword")
stab_x(so_TargetMarker,           OBJ "TargetMarker")
stab_x(so_TeleFog5,               OBJ "TeleFog5")
stab_x(so_TeleFog6,               OBJ "TeleFog6")
stab_x(so_TeleFog7,               OBJ "TeleFog7")
stab_x(so_ThothPlayer,            OBJ "ThothPlayer")
stab_x(so_UniqueID,               OBJ "UniqueID")
stab_x(so_WandererPlayer,         OBJ "WandererPlayer")
stab_x(so_WeaponScopedToken,      OBJ "WeaponScopedToken")

stab_x(so_LDLegendaryMonsterMarker,      "LDLegendaryMonsterMarker")
stab_x(so_LDLegendaryMonsterTransformed, "LDLegendaryMonsterTransformed")
stab_x(so_RLDeVileSix,                   "RLDeVileSix")

stab_x(so_Chaingun,      "Chaingun")
stab_x(so_PowerFlight,   "PowerFlight")
stab_x(so_PowerStrength, "PowerStrength")
stab_x(so_Shotgun,       "Shotgun")

#define GlobalCr(name) stab_x(so_##name, OBJ #name)
#include "m_drawing.h"

/* Paths */
stab_x(sp_Bars_ExpBar0,         ":Bars:ExpBar0")
stab_x(sp_Bars_ExpBar1,         ":Bars:ExpBar1")
stab_x(sp_Dialogue_Back,        ":Dialogue:Back")
stab_x(sp_HUD_A_LeftBack,       ":HUD_A:LeftBack")
stab_x(sp_HUD_A_RightBack,      ":HUD_A:RightBack")
stab_x(sp_HUD_C_AMMO,           ":HUD_C:AMMO")
stab_x(sp_HUD_C_Back,           ":HUD_C:Back")
stab_x(sp_HUD_C_Bar,            ":HUD_C:Bar")
stab_x(sp_HUD_C_BarSmall,       ":HUD_C:BarSmall")
stab_x(sp_HUD_C_BarSmall2,      ":HUD_C:BarSmall2")
stab_x(sp_HUD_C_HeatBar,        ":HUD_C:HeatBar")
stab_x(sp_HUD_C_MAG,            ":HUD_C:MAG")
stab_x(sp_HUD_C_MagicIcon,      ":HUD_C:MagicIcon")
stab_x(sp_HUD_C_ManaBar1,       ":HUD_C:ManaBar1")
stab_x(sp_HUD_C_ManaBar2,       ":HUD_C:ManaBar2")
stab_x(sp_HUD_C_SplitBack,      ":HUD_C:SplitBack")
stab_x(sp_HUD_C_SplitFront,     ":HUD_C:SplitFront")
stab_x(sp_HUD_C_VIT,            ":HUD_C:VIT")
stab_x(sp_HUD_D_Ammo2Back,      ":HUD_D:Ammo2Back")
stab_x(sp_HUD_D_AmmoBack,       ":HUD_D:AmmoBack")
stab_x(sp_HUD_D_HPBack,         ":HUD_D:HPBack")
stab_x(sp_HUD_D_WepBack,        ":HUD_D:WepBack")
stab_x(sp_HUD_H_A1,             ":HUD:H_A1")
stab_x(sp_HUD_H_A2,             ":HUD:H_A2")
stab_x(sp_HUD_H_B1,             ":HUD:H_B1")
stab_x(sp_HUD_H_B2,             ":HUD:H_B2")
stab_x(sp_HUD_H_B3,             ":HUD:H_B3")
stab_x(sp_HUD_H_B4,             ":HUD:H_B4")
stab_x(sp_HUD_H_C1,             ":HUD:H_C1")
stab_x(sp_HUD_H_C2,             ":HUD:H_C2")
stab_x(sp_HUD_H_D14,            ":HUD:H_D14")
stab_x(sp_HUD_H_D16,            ":HUD:H_D16")
stab_x(sp_HUD_H_D21,            ":HUD:H_D21")
stab_x(sp_HUD_H_D24,            ":HUD:H_D24")
stab_x(sp_HUD_H_D41,            ":HUD:H_D41")
stab_x(sp_HUD_H_D44,            ":HUD:H_D44")
stab_x(sp_HUD_H_D46,            ":HUD:H_D46")
stab_x(sp_HUD_H_KC1,            ":HUD:H_KC1")
stab_x(sp_HUD_H_KC2,            ":HUD:H_KC2")
stab_x(sp_HUD_H_KC3,            ":HUD:H_KC3")
stab_x(sp_HUD_H_KS1,            ":HUD:H_KS1")
stab_x(sp_HUD_H_KS2,            ":HUD:H_KS2")
stab_x(sp_HUD_H_KS3,            ":HUD:H_KS3")
stab_x(sp_HUD_H_W1,             ":HUD:H_W1")
stab_x(sp_HUD_H_W3,             ":HUD:H_W3")
stab_x(sp_HUD_I_AMAG,           ":HUD_I:AMAG")
stab_x(sp_HUD_I_AMMO,           ":HUD_I:AMMO")
stab_x(sp_HUD_I_AmmoBack,       ":HUD_I:AmmoBack")
stab_x(sp_HUD_I_AmmoExtend,     ":HUD_I:AmmoExtend")
stab_x(sp_HUD_I_HPBack,         ":HUD_I:HPBack")
stab_x(sp_HUD_I_NMAG,           ":HUD_I:NMAG")
stab_x(sp_HUD_I_ScopeOverlay,   ":HUD_I:ScopeOverlay")
stab_x(sp_HUD_M_Bar,            ":HUD_M:Bar")
stab_x(sp_HUD_M_BarBig,         ":HUD_M:BarBig")
stab_x(sp_HUD_M_SplitBack,      ":HUD_M:SplitBack")
stab_x(sp_HUD_M_SplitBackRed,   ":HUD_M:SplitBackRed")
stab_x(sp_HUD_M_SplitFront,     ":HUD_M:SplitFront")
stab_x(sp_HUD_M_SplitLeft,      ":HUD_M:SplitLeft")
stab_x(sp_HUD_M_SplitRight,     ":HUD_M:SplitRight")
stab_x(sp_HUD_RifleScope,       ":HUD:RifleScope")
stab_x(sp_HUD_Vignette,         ":HUD:Vignette")
stab_x(sp_HUD_ZoomOverlay,      ":HUD:ZoomOverlay")
stab_x(sp_MagicSel_BG,          ":MagicSel:BG")
stab_x(sp_SubWepBack,           ":HUD_D:SubWepBack")
stab_x(sp_SubWepBar1,           ":HUD_D:SubWepBar1")
stab_x(sp_SubWepBar2,           ":HUD_D:SubWepBar2")
stab_x(sp_SubWepUse,            ":HUD_D:SubWepUse")
stab_x(sp_Terminal_Teleport,    ":Terminal:Teleport")
stab_x(sp_UI_ArmorInter,        ":UI:ArmorInter")
stab_x(sp_UI_AttrBar1,          ":UI:AttrBar1")
stab_x(sp_UI_AttrBar2,          ":UI:AttrBar2")
stab_x(sp_UI_AttrBar4,          ":UI:AttrBar4")
stab_x(sp_UI_Bag,               ":UI:Bag")
stab_x(sp_UI_Body,              ":UI:Body")
stab_x(sp_UI_CPU1,              ":UI:CPU1")
stab_x(sp_UI_CPU2,              ":UI:CPU2")
stab_x(sp_UI_CPU3,              ":UI:CPU3")
stab_x(sp_UI_Cursor,            ":UI:Cursor")
stab_x(sp_UI_CursorBlue,        ":UI:CursorBlue")
stab_x(sp_UI_CursorOrange,      ":UI:CursorOrange")
stab_x(sp_UI_CursorOutline,     ":UI:CursorOutline")
stab_x(sp_UI_CursorOutline2,    ":UI:CursorOutline2")
stab_x(sp_UI_CursorOutline2Inv, ":UI:CursorOutline2Inv")
stab_x(sp_UI_CursorOutlineInv,  ":UI:CursorOutlineInv")
stab_x(sp_UI_CursorPink,        ":UI:CursorPink")
stab_x(sp_UI_CursorRed,         ":UI:CursorRed")
stab_x(sp_UI_CursorWhite,       ":UI:CursorWhite")
stab_x(sp_UI_InvBackBody,       ":UI:InvBackBody")
stab_x(sp_UI_InvBackLowerArms,  ":UI:InvBackLowerArms")
stab_x(sp_UI_InvBackStore,      ":UI:InvBackStore")
stab_x(sp_UI_InvBackUpperArms,  ":UI:InvBackUpperArms")
stab_x(sp_UI_InvSel,            ":UI:InvSel")
stab_x(sp_UI_LogList,           ":UI:LogList")
stab_x(sp_UI_MagicSelectBack,   ":UI:MagicSelectBack")
stab_x(sp_UI_New,               ":UI:New")
stab_x(sp_UI_RDistInter,        ":UI:RDistInter")
stab_x(sp_UI_ResultFrame,       ":UI:ResultFrame")
stab_x(sp_UI_Slot1Spell,        ":UI:Slot1Spell")
stab_x(sp_UI_Slot2Spell,        ":UI:Slot2Spell")
stab_x(sp_UI_Slot3Spell,        ":UI:Slot3Spell")
stab_x(sp_UI_Slot4Spell,        ":UI:Slot4Spell")
stab_x(sp_UI_Slot5Spell,        ":UI:Slot5Spell")
stab_x(sp_UI_Slot6Spell,        ":UI:Slot6Spell")
stab_x(sp_UI_Slot7Spell,        ":UI:Slot7Spell")
stab_x(sp_UI_WeapnInte2,        ":UI:WeapnInte2")
stab_x(sp_UI_WeapnInter,        ":UI:WeapnInter")
stab_x(sp_UI_bip,               ":UI:bip")
stab_x(sp_Weapon_Blade,         ":Weapon:Blade")
stab_x(sp_Weapon_BladeHit,      ":Weapon:BladeHit")

stab_x(sp_lfiles_Cyberspace, "lfiles/Cyberspace.txt")
stab_x(sp_lmusic_DOGS,       "lmusic/DOGS.ogg")
stab_x(sp_lmusic_Shredder,   "lmusic/Shredder.ogg")
stab_x(sp_lmusic_Unce,       "lmusic/Unce.ogg")
stab_x(sp_lmusic_miniwip2,   "lmusic/miniwip2.ogg")
stab_x(sp_lsnd_Silence,      "lsnd/Silence")

stab_x(sp_F_SKY1,   "F_SKY1")
stab_x(sp_LITHCAM1, "LITHCAM1")
stab_x(sp_LITHCAM2, "LITHCAM2")
stab_x(sp_LITHCAM3, "LITHCAM3")
stab_x(sp_LITHEND,  "LITHEND")
stab_x(sp_LITHINFO, "LITHINFO")
stab_x(sp_LITHMONS, "LITHMONS")
stab_x(sp_LITHSKDE, "LITHSKDE")
stab_x(sp_LITHSKRD, "LITHSKRD")
stab_x(sp_LITHSKS1, "LITHSKS1")

stab_x(sp_star, "*")

/* Language Strings */
stab_x(sl_activate,            LANG "ACTIVATE")
stab_x(sl_autogroups,          LANG "AUTOGROUPS")
stab_x(sl_bad_timeline,        LANG "BAD_TIMELINE")
stab_x(sl_begin,               LANG "BEGIN")
stab_x(sl_bip_available,       LANG "BIP_AVAILABLE")
stab_x(sl_bip_back,            LANG "BIP_BACK")
stab_x(sl_bip_categs,          LANG "BIP_CATEGS")
stab_x(sl_bip_header,          LANG "BIP_HEADER")
stab_x(sl_bip_no_results,      LANG "BIP_NO_RESULTS")
stab_x(sl_buy,                 LANG "BUY")
stab_x(sl_cat_all,             LANG "CAT_ALL")
stab_x(sl_cat_filter,          LANG "CAT_FILTER")
stab_x(sl_cbi_armorinter,      LANG "CBI_ArmorInter")
stab_x(sl_cbi_cpu1,            LANG "CBI_CPU1")
stab_x(sl_cbi_cpu2,            LANG "CBI_CPU2")
stab_x(sl_cbi_cpu3,            LANG "CBI_CPU3")
stab_x(sl_cbi_cpu4,            LANG "CBI_CPU4")
stab_x(sl_cbi_interfaces,      LANG "CBI_INTERFACES")
stab_x(sl_cbi_perf,            LANG "CBI_PERF")
stab_x(sl_cbi_puse,            LANG "CBI_PUSE")
stab_x(sl_cbi_rdistinter,      LANG "CBI_RDistInter")
stab_x(sl_cbi_reactarmor,      LANG "CBI_ReactArmor")
stab_x(sl_cbi_slot1spell,      LANG "CBI_Slot1Spell")
stab_x(sl_cbi_slot2spell,      LANG "CBI_Slot2Spell")
stab_x(sl_cbi_slot3spell,      LANG "CBI_Slot3Spell")
stab_x(sl_cbi_slot4spell,      LANG "CBI_Slot4Spell")
stab_x(sl_cbi_slot5spell,      LANG "CBI_Slot5Spell")
stab_x(sl_cbi_slot6spell,      LANG "CBI_Slot6Spell")
stab_x(sl_cbi_slot7spell,      LANG "CBI_Slot7Spell")
stab_x(sl_cbi_weapninte2,      LANG "CBI_WeapnInte2")
stab_x(sl_cbi_weapninter,      LANG "CBI_WeapnInter")
stab_x(sl_cbi_wram,            LANG "CBI_WRAM")
stab_x(sl_charge_fmt,          LANG "CHARGE_FMT")
stab_x(sl_clear,               LANG "CLEAR")
stab_x(sl_close,               LANG "CLOSE")
stab_x(sl_credits,             LANG "CREDITS")
stab_x(sl_deactivate,          LANG "DEACTIVATE")
stab_x(sl_discard,             LANG "DISCARD")
stab_x(sl_div_get,             LANG "DIV_SIGIL_GET")
stab_x(sl_div_warning,         LANG "DIV_SIGIL_WARNING")
stab_x(sl_edit,                LANG "EDIT")
stab_x(sl_effect,              LANG "EFFECT")
stab_x(sl_empty,               LANG "EMPTY")
stab_x(sl_enter_fmt,           LANG "ENTER_FMT")
stab_x(sl_equip,               LANG "EQUIP")
stab_x(sl_free,                LANG "FREE")
stab_x(sl_gui_textbox,         LANG "GUI_TEXTBOX")
stab_x(sl_inv_hints,           LANG "INV_HINTS")
stab_x(sl_jet,                 LANG "JET")
stab_x(sl_log_activearmor,     LANG "LOG_ActiveArmor")
stab_x(sl_log_adrenalineready, LANG "LOG_AdrenalineReady")
stab_x(sl_log_adrenalineused,  LANG "LOG_AdrenalineUsed")
stab_x(sl_log_bosshear1,       LANG "LOG_BossHear1")
stab_x(sl_log_bosshear2,       LANG "LOG_BossHear2")
stab_x(sl_log_bosshear3,       LANG "LOG_BossHear3")
stab_x(sl_log_bought,          LANG "LOG_Bought")
stab_x(sl_log_cbi,             LANG "LOG_CBI")
stab_x(sl_log_delivered,       LANG "LOG_Delivered")
stab_x(sl_log_mailrecv,        LANG "LOG_MailRecv")
stab_x(sl_log_nodeliver,       LANG "LOG_NoDeliver")
stab_x(sl_log_sell,            LANG "LOG_Sell")
stab_x(sl_log_sellorder,       LANG "LOG_SellOrder")
stab_x(sl_log_startgame,       LANG "LOG_StartGame")
stab_x(sl_log_version,         LANG "LOG_Version")
stab_x(sl_mail_template,       LANG "MAIL_TEMPLATE")
stab_x(sl_mana_charge,         LANG "MANA_CHARGE")
stab_x(sl_move,                LANG "MOVE")
stab_x(sl_note_fmt,            LANG "NOTE_FMT")
stab_x(sl_off,                 LANG "OFF")
stab_x(sl_on,                  LANG "ON")
stab_x(sl_open,                LANG "OPEN")
stab_x(sl_opener,              LANG "OPENER")
stab_x(sl_pk_get_000,          LANG "PK_GET_000")
stab_x(sl_pk_get_num,          LANG "PK_GET_NUM")
stab_x(sl_pk_uncertain_num,    LANG "PK_UNCERTAIN_NUM")
stab_x(sl_required,            LANG "REQUIRED")
stab_x(sl_res_account,         LANG "RES_ACCOUNT")
stab_x(sl_res_closed,          LANG "RES_CLOSED")
stab_x(sl_res_item,            LANG "RES_ITEM")
stab_x(sl_res_item100,         LANG "RES_ITEM100")
stab_x(sl_res_kill,            LANG "RES_KILL")
stab_x(sl_res_kill100,         LANG "RES_KILL100")
stab_x(sl_res_par,             LANG "RES_PAR")
stab_x(sl_res_payment,         LANG "RES_PAYMENT")
stab_x(sl_res_results,         LANG "RES_RESULTS")
stab_x(sl_res_scrt,            LANG "RES_SCRT")
stab_x(sl_res_scrt100,         LANG "RES_SCRT100")
stab_x(sl_res_sponsor,         LANG "RES_SPONSOR")
stab_x(sl_res_subtotal,        LANG "RES_SUBTOTAL")
stab_x(sl_res_tax,             LANG "RES_TAX")
stab_x(sl_res_total,           LANG "RES_TOTAL")
stab_x(sl_res_total,           LANG "RES_TOTAL")
stab_x(sl_sell,                LANG "SELL")
stab_x(sl_shop_activate_uses,  LANG "SHOP_ACTIVATE_USES")
stab_x(sl_shop_cant_activate,  LANG "SHOP_CANT_ACTIVATE")
stab_x(sl_shop_disable_saves,  LANG "SHOP_DISABLE_SAVES")
stab_x(sl_shop_mul_disable,    LANG "SHOP_MUL_DISABLE")
stab_x(sl_shop_mul_enable,     LANG "SHOP_MUL_ENABLE")
stab_x(sl_skip_intro_1,        LANG "SKIP_INTRO_1")
stab_x(sl_skip_intro_2,        LANG "SKIP_INTRO_2")
stab_x(sl_st_labl_gui,         LANG "st_labl_gui")
stab_x(sl_st_labl_hud,         LANG "st_labl_hud")
stab_x(sl_st_labl_itm,         LANG "st_labl_itm")
stab_x(sl_st_labl_ply,         LANG "st_labl_ply")
stab_x(sl_st_labl_wep,         LANG "st_labl_wep")
stab_x(sl_st_labl_wld,         LANG "st_labl_wld")
stab_x(sl_st_name_unknown,     LANG "st_name_unknown")
stab_x(sl_stat_boom,           LANG "STAT_Boom")
stab_x(sl_stat_brouzouf,       LANG "STAT_Brouzouf")
stab_x(sl_stat_healthsum,      LANG "STAT_HealthSum")
stab_x(sl_stat_healthused,     LANG "STAT_HealthUsed")
stab_x(sl_stat_items,          LANG "STAT_Items")
stab_x(sl_stat_mail,           LANG "STAT_Mail")
stab_x(sl_stat_scoremul,       LANG "STAT_ScoreMul")
stab_x(sl_stat_scoresum,       LANG "STAT_ScoreSum")
stab_x(sl_stat_scoreused,      LANG "STAT_ScoreUsed")
stab_x(sl_stat_secrets,        LANG "STAT_Secrets")
stab_x(sl_stat_time,           LANG "STAT_Time")
stab_x(sl_stat_truemail,       LANG "STAT_TrueMail")
stab_x(sl_stat_units,          LANG "STAT_Units")
stab_x(sl_stat_upgrades,       LANG "STAT_Upgrades")
stab_x(sl_stat_weapons,        LANG "STAT_Weapons")
stab_x(sl_status_cl,           LANG "STATUS_CL")
stab_x(sl_status_hp,           LANG "STATUS_HP")
stab_x(sl_status_levelup,      LANG "STATUS_LEVELUP")
stab_x(sl_status_lv,           LANG "STATUS_LV")
stab_x(sl_status_mp,           LANG "STATUS_MP")
stab_x(sl_status_nx,           LANG "STATUS_NX")
stab_x(sl_status_xp,           LANG "STATUS_XP")
stab_x(sl_tab_attributes,      LANG "TAB_ATTRIBUTES")
stab_x(sl_tab_bip,             LANG "TAB_BIP")
stab_x(sl_tab_cbi,             LANG "TAB_CBI")
stab_x(sl_tab_info,            LANG "TAB_INFO")
stab_x(sl_tab_inventory,       LANG "TAB_INVENTORY")
stab_x(sl_tab_log,             LANG "TAB_LOG")
stab_x(sl_tab_notes,           LANG "TAB_NOTES")
stab_x(sl_tab_settings,        LANG "TAB_SETTINGS")
stab_x(sl_tab_shop,            LANG "TAB_SHOP")
stab_x(sl_tab_statistics,      LANG "TAB_STATISTICS")
stab_x(sl_tab_status,          LANG "TAB_STATUS")
stab_x(sl_tab_upgrades,        LANG "TAB_UPGRADES")
stab_x(sl_term_disconnecting,  LANG "TERM_DISCONNECTING")
stab_x(sl_term_ip,             LANG "TERM_IP")
stab_x(sl_term_open_connect,   LANG "TERM_OPEN_CONNECT")
stab_x(sl_term_remote,         LANG "TERM_REMOTE")
stab_x(sl_term_sgxline,        LANG "TERM_SGXLINE")
stab_x(sl_term_use_to_ack,     LANG "TERM_USE_TO_ACK")
stab_x(sl_time_fmt_date,       LANG "TIME_FMT_DATE")
stab_x(sl_time_fmt_long,       LANG "TIME_FMT_LONG")
stab_x(sl_time_fmt_short,      LANG "TIME_FMT_SHORT")
stab_x(sl_use,                 LANG "USE")

/* Sounds */
stab_x(ss_amb_rain,                 "amb/rain")
stab_x(ss_amb_wind,                 "amb/wind")
stab_x(ss_enemies_boss_talk,        "enemies/boss/talk")
stab_x(ss_enemies_phantom_escape,   "enemies/phantom/escape")
stab_x(ss_enemies_phantom_spawned,  "enemies/phantom/spawned")
stab_x(ss_marathon_pickup,          "marathon/pickup")
stab_x(ss_misc_telein,              "misc/telein")
stab_x(ss_misc_teleout,             "misc/teleout")
stab_x(ss_player_YOUVEGOTMAIL,      "player/YOUVEGOTMAIL")
stab_x(ss_player_adren_inj,         "player/adren/inj")
stab_x(ss_player_cbi_auto_buy,      "player/cbi/auto/buy")
stab_x(ss_player_cbi_auto_invalid,  "player/cbi/auto/invalid")
stab_x(ss_player_cbi_auto_toggle,   "player/cbi/auto/toggle")
stab_x(ss_player_cbi_close,         "player/cbi/close")
stab_x(ss_player_cbi_dlgopen,       "player/cbi/dlgopen")
stab_x(ss_player_cbi_find,          "player/cbi/find")
stab_x(ss_player_cbi_finddone,      "player/cbi/finddone")
stab_x(ss_player_cbi_findfail,      "player/cbi/findfail")
stab_x(ss_player_cbi_invcur,        "player/cbi/invcur")
stab_x(ss_player_cbi_invmov,        "player/cbi/invmov")
stab_x(ss_player_cbi_invrem,        "player/cbi/invrem")
stab_x(ss_player_cbi_keypress,      "player/cbi/keypress")
stab_x(ss_player_cbi_mail,          "player/cbi/mail")
stab_x(ss_player_cbi_open,          "player/cbi/open")
stab_x(ss_player_cbi_scroll,        "player/cbi/scroll")
stab_x(ss_player_cbi_scrollend,     "player/cbi/scrollend")
stab_x(ss_player_cbi_win95,         "player/cbi/win95")
stab_x(ss_player_counter,           "player/counter")
stab_x(ss_player_counterdone,       "player/counterdone")
stab_x(ss_player_death1,            "player/death1")
stab_x(ss_player_doublejump,        "player/doublejump")
stab_x(ss_player_glare,             "player/glare")
stab_x(ss_player_levelup,           "player/levelup")
stab_x(ss_player_lightoff,          "player/lightoff")
stab_x(ss_player_lighton,           "player/lighton")
stab_x(ss_player_manafull,          "player/manafull")
stab_x(ss_player_pickup_item,       "player/pickup/item")
stab_x(ss_player_pickup_upgrbody,   "player/pickup/upgrbody")
stab_x(ss_player_pickup_upgrextr,   "player/pickup/upgrextr")
stab_x(ss_player_pickup_upgrweap,   "player/pickup/upgrweap")
stab_x(ss_player_rarmor_mode,       "player/rarmor/mode")
stab_x(ss_player_rocketboost,       "player/rocketboost")
stab_x(ss_player_score,             "player/score")
stab_x(ss_player_showtext,          "player/showtext")
stab_x(ss_player_slide,             "player/slide")
stab_x(ss_player_startgame,         "player/startgame")
stab_x(ss_player_trmopen,           "player/trmopen")
stab_x(ss_player_trmswitch,         "player/trmswitch")
stab_x(ss_player_zoomin,            "player/zoomin")
stab_x(ss_player_zoomout,           "player/zoomout")
stab_x(ss_weapons_cercle_attack,    "weapons/cercle/attack")
stab_x(ss_weapons_cercle_begin,     "weapons/cercle/begin")
stab_x(ss_weapons_rifle_firemode,   "weapons/rifle/firemode")
stab_x(ss_weapons_rocket_mark,      "weapons/rocket/mark")
stab_x(ss_weapons_subweapon_charge, "weapons/subweapon/charge")

/* Arrays */
stab_ary_bgn_x(sa_ranks)
stab_ary_ent_x(":UI:Rank1") stab_ary_ent_x(":UI:Rank2")
stab_ary_ent_x(":UI:Rank3") stab_ary_ent_x(":UI:Rank4")
stab_ary_ent_x(":UI:Rank5") stab_ary_ent_x(":UI:Rank6")
stab_ary_ent_x(":UI:Rank7") stab_ary_ent_x(":UI:Rank8")
stab_ary_end_x()

stab_ary_bgn_x(sa_healthbars)
stab_ary_ent_x(":Bars:HealthBar1")  stab_ary_ent_x(":Bars:HealthBar2")
stab_ary_ent_x(":Bars:HealthBar3")  stab_ary_ent_x(":Bars:HealthBar4")
stab_ary_ent_x(":Bars:HealthBar5")  stab_ary_ent_x(":Bars:HealthBar6")
stab_ary_ent_x(":Bars:HealthBar7")  stab_ary_ent_x(":Bars:HealthBar8")
stab_ary_ent_x(":Bars:HealthBar9")  stab_ary_ent_x(":Bars:HealthBar10")
stab_ary_ent_x(":Bars:HealthBar11") stab_ary_ent_x(":Bars:HealthBar12")
stab_ary_ent_x(":Bars:HealthBar13") stab_ary_ent_x(":Bars:HealthBar14")
stab_ary_ent_x(":Bars:HealthBar15") stab_ary_ent_x(":Bars:HealthBar16")
stab_ary_ent_x(":Bars:HealthBar17") stab_ary_ent_x(":Bars:HealthBar18")
stab_ary_ent_x(":Bars:HealthBar19") stab_ary_ent_x(":Bars:HealthBar20")
stab_ary_ent_x(":Bars:HealthBar21") stab_ary_ent_x(":Bars:HealthBar22")
stab_ary_ent_x(":Bars:HealthBar23") stab_ary_ent_x(":Bars:HealthBar24")
stab_ary_ent_x(":Bars:HealthBar25") stab_ary_ent_x(":Bars:HealthBar26")
stab_ary_ent_x(":Bars:HealthBar27") stab_ary_ent_x(":Bars:HealthBar28")
stab_ary_ent_x(":Bars:HealthBar29") stab_ary_ent_x(":Bars:HealthBar30")
stab_ary_ent_x(":Bars:HealthBar31") stab_ary_ent_x(":Bars:HealthBar32")
stab_ary_end_x()

stab_ary_bgn_x(sa_wgfx)
stab_ary_ent_x(":HUD:H_D27") stab_ary_ent_x(":HUD:H_D28")
stab_ary_ent_x(":HUD:H_D24") stab_ary_ent_x(":HUD:H_D23")
stab_ary_ent_x(":HUD:H_D22") stab_ary_ent_x(":HUD:H_D21")
stab_ary_ent_x(":HUD:H_D25") stab_ary_ent_x(":HUD:H_D26")
stab_ary_end_x()

stab_ary_bgn_x(sa_fgfx)
stab_ary_ent_x(":HUD:H_D11") stab_ary_ent_x(":HUD:H_D12")
stab_ary_ent_x(":HUD:H_D13") stab_ary_ent_x(":HUD:H_D14")
stab_ary_ent_x(":HUD:H_D15")
stab_ary_end_x()

stab_ary_bgn_x(sa_scopes)
stab_ary_ent_x(":HUD_C:ScopeOverlay1") stab_ary_ent_x(":HUD_C:ScopeOverlay2")
stab_ary_ent_x(":HUD_C:ScopeOverlay3") stab_ary_ent_x(":HUD_C:ScopeOverlay4")
stab_ary_end_x()

stab_ary_bgn_x(sa_rifle_modes)
stab_ary_ent_x(":HUD:H_W4") stab_ary_ent_x(":HUD:H_W5")
stab_ary_ent_x(":HUD:H_W6")
stab_ary_end_x()

stab_ary_bgn_x(sa_rend_nh)
stab_ary_ent_x(":Weapon:Rend1") stab_ary_ent_x(":Weapon:Rend2")
stab_ary_ent_x(":Weapon:Rend3") stab_ary_ent_x(":Weapon:Rend4")
stab_ary_end_x()

stab_ary_bgn_x(sa_rend_ht)
stab_ary_ent_x(":Weapon:RendHit1") stab_ary_ent_x(":Weapon:RendHit2")
stab_ary_ent_x(":Weapon:RendHit3") stab_ary_ent_x(":Weapon:RendHit4")
stab_ary_end_x()

stab_ary_bgn_x(sa_groups)
stab_ary_ent_x(":UI:Group1") stab_ary_ent_x(":UI:Group2")
stab_ary_ent_x(":UI:Group3") stab_ary_ent_x(":UI:Group4")
stab_ary_end_x()

stab_ary_bgn_x(sa_dmgtype_names)
stab_ary_ent_x("Bullets")
stab_ary_ent_x("Energy")
stab_ary_ent_x("Fire")
stab_ary_ent_x("Magic")
stab_ary_ent_x("Melee")
stab_ary_ent_x("Shrapnel")
stab_ary_end_x()

stab_ary_bgn_x(sa_subwep_act)
stab_ary_ent_x(":HUD_D:SubWep0") stab_ary_ent_x(":HUD_D:SubWep1")
stab_ary_ent_x(":HUD_D:SubWep2") stab_ary_ent_x(":HUD_D:SubWep3")
stab_ary_ent_x(":HUD_D:SubWep4") stab_ary_ent_x(":HUD_D:SubWep5")
stab_ary_end_x()

stab_ary_bgn_x(sa_upgr_categ)
#define upgr_category_x(name) stab_ary_ent_x(LANG "CAT_" #name)
#include "u_data.h"
stab_ary_end_x()

stab_ary_bgn_x(sa_ammo_types)
stab_ary_ent_x(OBJ "ManaAmmo")
stab_ary_ent_x(OBJ "BulletAmmo")
stab_ary_ent_x(OBJ "ShellAmmo")
stab_ary_ent_x(OBJ "RocketAmmo")
stab_ary_ent_x(OBJ "PlasmaAmmo")
stab_ary_ent_x(OBJ "CannonAmmo")
stab_ary_end_x()

stab_ary_bgn_x(sa_filler)
stab_ary_ent_x(":UI:Filler0")
stab_ary_ent_x(":UI:Filler1")
stab_ary_ent_x(":UI:Filler2")
stab_ary_ent_x(":UI:Filler3")
stab_ary_ent_x(":UI:Filler4")
stab_ary_ent_x(":UI:Filler5")
stab_ary_ent_x(":UI:Filler6")
stab_ary_ent_x(":UI:Filler7")
stab_ary_ent_x(":UI:Filler8")
stab_ary_end_x()

#undef stab_ary_bgn_x
#undef stab_ary_ent_x
#undef stab_ary_end_x
#undef stab_x
#elif !defined(m_stab_h)
#define m_stab_h

#define stab_ary_bgn_x(name) extern str name[0
#define stab_ary_ent_x(strn) + 1
#define stab_ary_end_x()     ];
#define stab_x(n, s) extern str n;
#include "m_stab.h"

#endif
