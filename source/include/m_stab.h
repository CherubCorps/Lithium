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

#if defined(X)

X(st_nil, "")

/* Font Names */
X(sf_areaname,      "areaname")
X(sf_bigupper,      "bigupper")
X(sf_lhudfontsmall, "lhudfontsmall")
X(sf_lmidfont,      "lmidfont")
X(sf_ltrmfont,      "ltrmfont")
X(sf_smallfnt,      "smallfnt")

/* Console Variables */
X(sc_fun,                    DCVAR "fun")
X(sc_gui_buyfiller,          CVAR "gui_buyfiller")
X(sc_gui_cursor,             CVAR "gui_cursor")
X(sc_gui_defcr,              CVAR "gui_defcr")
X(sc_gui_theme,              CVAR "gui_theme")
X(sc_gui_xmul,               CVAR "gui_xmul")
X(sc_gui_ymul,               CVAR "gui_ymul")
X(sc_hud_expbar,             CVAR "hud_expbar")
X(sc_hud_logfromtop,         CVAR "hud_logfromtop")
X(sc_hud_logsize,            CVAR "hud_logsize")
X(sc_hud_showarmorind,       CVAR "hud_showarmorind")
X(sc_hud_showlog,            CVAR "hud_showlog")
X(sc_hud_showlvl,            CVAR "hud_showlvl")
X(sc_hud_showscore,          CVAR "hud_showscore")
X(sc_hud_showweapons,        CVAR "hud_showweapons")
X(sc_k_opencbi,              CVAR "k_opencbi")
X(sc_light_battery,          CVAR "light_battery")
X(sc_light_regen,            CVAR "light_regen")
X(sc_player_ammolog,         CVAR "player_ammolog")
X(sc_player_bosstexts,       CVAR "player_bosstexts")
X(sc_player_damagebob,       CVAR "player_damagebob")
X(sc_player_damagebobmul,    CVAR "player_damagebobmul")
X(sc_player_footstepvol,     CVAR "player_footstepvol")
X(sc_player_invertmouse,     CVAR "player_invertmouse")
X(sc_player_lvsys,           CVAR "player_lvsys")
X(sc_player_resultssound,    CVAR "player_resultssound")
X(sc_player_scorelog,        CVAR "player_scorelog")
X(sc_player_scoresound,      CVAR "player_scoresound")
X(sc_player_stupidpickups,   CVAR "player_stupidpickups")
X(sc_player_teleshop,        CVAR "player_teleshop")
X(sc_player_viewtilt,        CVAR "player_viewtilt")
X(sc_psave,                  CVAR "psave")
X(sc_scanner_altfont,        CVAR "scanner_altfont")
X(sc_scanner_bar,            CVAR "scanner_bar")
X(sc_scanner_color,          CVAR "scanner_color")
X(sc_scanner_slide,          CVAR "scanner_slide")
X(sc_scanner_xoffs,          CVAR "scanner_xoffs")
X(sc_scanner_yoffs,          CVAR "scanner_yoffs")
X(sc_sv_minhealth,           CVAR "sv_minhealth")
X(sc_sv_autosave,            CVAR "sv_autosave")
X(sc_sv_difficulty,          CVAR "sv_difficulty")
X(sc_sv_failtime,            CVAR "sv_failtime")
X(sc_sv_nobossdrop,          CVAR "sv_nobossdrop")
X(sc_sv_nobosses,            CVAR "sv_nobosses")
X(sc_sv_rain,                CVAR "sv_rain")
X(sc_sv_revenge,             CVAR "sv_revenge")
X(sc_sv_scoremul,            CVAR "sv_scoremul")
X(sc_sv_sky,                 CVAR "sv_sky")
X(sc_sv_wepdrop,             CVAR "sv_wepdrop")
X(sc_version,                DCVAR "version")
X(sc_weapons_magicselanims,  CVAR "weapons_magicselanims")
X(sc_weapons_riflemodeclear, CVAR "weapons_riflemodeclear")
X(sc_weapons_riflescope,     CVAR "weapons_riflescope")
X(sc_weapons_slot3ammo,      CVAR "weapons_slot3ammo")
X(sc_weapons_zoomfactor,     CVAR "weapons_zoomfactor")
X(sc_xhair_style,            CVAR "xhair_style")

/* Methods / Members */
X(sm_AdrenalineCheck,    "AdrenalineCheck")
X(sm_AlertMonsters,      "AlertMonsters")
X(sm_AutoReload,         "AutoReload")
X(sm_DelearSprite,       "DelearSprite")
X(sm_DeleteItem,         "DeleteItem")
X(sm_DieMonster,         "DieMonster")
X(sm_DieNow,             "DieNow")
X(sm_DmgFac,             "m_DmgFac")
X(sm_FindFontColor,      "FindFontColor")
X(sm_FindLump,           "FindLump")
X(sm_ForceDraw,          "ForceDraw")
X(sm_FxTID,              "m_FxTID")
X(sm_GetAttackHeight,    "GetAttackHeight")
X(sm_GetBipName,         "GetBipName")
X(sm_GetBossLevel,       "GetBossLevel")
X(sm_GetCurMag,          "GetCurMag")
X(sm_GetLogName,         "GetLogName")
X(sm_GetMaxMag,          "GetMaxMag")
X(sm_GetObituary,        "GetObituary")
X(sm_GetOutOfDodge,      "GetOutOfDodge")
X(sm_GetPaused,          "GetPaused")
X(sm_GetRifleGrenade,    "GetRifleGrenade")
X(sm_GetSpawnHealth,     "GetSpawnHealth")
X(sm_GetViewHeight,      "GetViewHeight")
X(sm_GivePoison,         "GivePoison")
X(sm_GrabInput,          "m_GrabInput")
X(sm_Health,             "Health")
X(sm_Height,             "Height")
X(sm_InvName,            "m_InvName")
X(sm_IonizeFX,           OBJ "IonizeFX")
X(sm_IsRampancy,         "IsRampancy")
X(sm_JumpZ,              "JumpZ")
X(sm_LC,                 "LC")
X(sm_LE,                 "LE")
X(sm_LF,                 "LF")
X(sm_LL,                 "LL")
X(sm_LR,                 "LR")
X(sm_LS,                 "LS")
X(sm_LT,                 "LT")
X(sm_LX,                 "LX")
X(sm_LY,                 "LY")
X(sm_LZ,                 "LZ")
X(sm_MapNoRain,          "user_" OBJ "NoRain")
X(sm_MapUseRain,         "user_" OBJ "UseRain")
X(sm_MonsterBarrierLook, "MonsterBarrierLook")
X(sm_Nuke,               "Nuke")
X(sm_OnGround,           "m_OnGround")
X(sm_Overdrive,          "m_Overdrive")
X(sm_PhantomOut,         "PhantomOut")
X(sm_PhantomTeleport,    "PhantomTeleport")
X(sm_PlayerDeath,        "PlayerDeath")
X(sm_PlayerDeathNuke,    "PlayerDeathNuke")
X(sm_PlayerInit,         "PlayerInit")
X(sm_PoisonFX,           "PoisonFX")
X(sm_PureSteggleEnergy,  "PureSteggleEnergy")
X(sm_Radius,             "Radius")
X(sm_ReadLump,           "ReadLump")
X(sm_Set,                "Set")
X(sm_SetFrozen,          "SetFrozen")
X(sm_SetTeleFogTo,       "SetTeleFogTo")
X(sm_SpawnBoss,          "SpawnBoss")
X(sm_SpawnRain,          "SpawnRain")
X(sm_Speed,              "Speed")
X(sm_StartSound,         "StartSound")
X(sm_SurgeOfDestiny,     "SurgeOfDestiny")
X(sm_TraceFrom,          "TraceFrom")
X(sm_Trigger,            "m_Trigger")
X(sm_TriggerBoss,        "TriggerBoss")
X(sm_Unproject,          "Unproject")
X(sm_UpgradeBody,        "m_UpgradeBody")
X(sm_UpgradeExtr,        "m_UpgradeExtr")
X(sm_UpgradeId,          "m_UpgradeId")
X(sm_UpgradeWeap,        "m_UpgradeWeap")
X(sm_UseItem,            "UseItem")
X(sm_UserX,              "user_X")
X(sm_UserY,              "user_Y")
X(sm_UserZ,              "user_Z")
X(sm_WaterLevel,         "WaterLevel")

/* Objects */
X(so_AssassinPlayer,      OBJ "AssassinPlayer")
X(so_BarrierFX,           OBJ "BarrierFX")
X(so_Blade,               OBJ "Blade")
X(so_BloodRainDrop,       OBJ "BloodRainDrop")
X(so_BossReward1,         OBJ "BossReward1")
X(so_BossReward2,         OBJ "BossReward2")
X(so_BossReward3,         OBJ "BossReward3")
X(so_BossReward4,         OBJ "BossReward4")
X(so_BossReward5,         OBJ "BossReward5")
X(so_BossReward6,         OBJ "BossReward6")
X(so_BoughtItem,          OBJ "BoughtItem")
X(so_CameraHax,           OBJ "CameraHax")
X(so_CannonAmmo,          OBJ "CannonAmmo")
X(so_Cercle,              OBJ "Cercle")
X(so_CircleParticle,      OBJ "CircleParticle")
X(so_CircleSpearThrower,  OBJ "CircleSpearThrower")
X(so_ClawOfImp,           OBJ "ClawOfImp")
X(so_CyberMagePlayer,     OBJ "CyberMagePlayer")
X(so_DarkLordPlayer,      OBJ "DarkLordPlayer")
X(so_Delear,              OBJ "Delear")
X(so_Draw,                OBJ "URANUS")
X(so_Dummy,               OBJ "Dummy")
X(so_EXPLOOOSION,         OBJ "EXPLOOOSION")
X(so_ExplodoBoots,        OBJ "ExplodoBoots")
X(so_Feuer,               OBJ "Feuer")
X(so_FeuerExplosion,      OBJ "FeuerExplosion")
X(so_FeuerFinal,          OBJ "FeuerFinal")
X(so_FeuerTest,           OBJ "FeuerTest")
X(so_FistCharge,          OBJ "FistCharge")
X(so_GEExplosion,         OBJ "GEExplosion")
X(so_GTFO,                OBJ "GTFO")
X(so_Hulgyon,             OBJ "Hulgyon")
X(so_InformantPlayer,     OBJ "InformantPlayer")
X(so_Ionized,             OBJ "Ionized")
X(so_Mana,                OBJ "Mana")
X(so_ManaLeak,            OBJ "ManaLeak")
X(so_ManaPickup,          OBJ "ManaPickup")
X(so_MarinePlayer,        OBJ "MarinePlayer")
X(so_MegaProtection,      OBJ "MegaProtection")
X(so_MonsterBarrier,      OBJ "MonsterBarrier")
X(so_MonsterHeptaura,     OBJ "MonsterHeptaura")
X(so_MonsterID,           OBJ "MonsterID")
X(so_MonsterInvalid,      OBJ "MonsterInvalid")
X(so_MonsterLevelUp,      OBJ "MonsterLevelUp")
X(so_MonsterSoul,         OBJ "MonsterSoul")
X(so_PlasmaAmmo,          OBJ "PlasmaAmmo")
X(so_Player,              OBJ "Player")
X(so_PoisonFXReset,       OBJ "PoisonFXReset")
X(so_PoisonFXTimer,       OBJ "PoisonFXTimer")
X(so_PowerStrength,       "PowerStrength")
X(so_PunctuatorExplosion, OBJ "PunctuatorExplosion")
X(so_PunctuatorPuff,      OBJ "PunctuatorPuff")
X(so_RainDrop,            OBJ "RainDrop")
X(so_RandomSpawn,         OBJ "RandomSpawn")
X(so_Rend,                OBJ "Rend")
X(so_RocketAmmo,          OBJ "RocketAmmo")
X(so_RocketBooster,       OBJ "RocketBooster")
X(so_SMGHeat,             OBJ "SMGHeat")
X(so_Serv,                OBJ "HERMES")
X(so_ShellAmmo,           OBJ "ShellAmmo")
X(so_StarShot,            OBJ "StarShot")
X(so_StealthSystem,       OBJ "StealthSystem")
X(so_Steggles,            OBJ "Steggles")
X(so_Sword,               OBJ "Sword")
X(so_TargetMarker,        OBJ "TargetMarker")
X(so_TeleFog5,            OBJ "TeleFog5")
X(so_TeleFog6,            OBJ "TeleFog6")
X(so_TeleFog7,            OBJ "TeleFog7")
X(so_ThothPlayer,         OBJ "ThothPlayer")
X(so_UniqueID,            OBJ "UniqueID")
X(so_WandererPlayer,      OBJ "WandererPlayer")
X(so_WeaponScopedToken,   OBJ "WeaponScopedToken")

/* Paths */
X(sp_Dialogue_Back,        ":Dialogue:Back")
X(sp_HUD_C_BarSmall,       ":HUD_C:BarSmall")
X(sp_HUD_C_BarSmall2,      ":HUD_C:BarSmall2")
X(sp_HUD_C_MagicIcon,      ":HUD_C:MagicIcon")
X(sp_HUD_C_ManaBar1,       ":HUD_C:ManaBar1")
X(sp_HUD_C_ManaBar2,       ":HUD_C:ManaBar2")
X(sp_HUD_H_B3,             ":HUD:H_B3")
X(sp_HUD_H_C1,             ":HUD:H_C1")
X(sp_HUD_H_C2,             ":HUD:H_C2")
X(sp_HUD_H_D14,            ":HUD:H_D14")
X(sp_HUD_H_D16,            ":HUD:H_D16")
X(sp_HUD_H_D21,            ":HUD:H_D21")
X(sp_HUD_H_D24,            ":HUD:H_D24")
X(sp_HUD_H_D41,            ":HUD:H_D41")
X(sp_HUD_H_D44,            ":HUD:H_D44")
X(sp_HUD_H_D46,            ":HUD:H_D46")
X(sp_HUD_H_KC1,            ":HUD:H_KC1")
X(sp_HUD_H_KC2,            ":HUD:H_KC2")
X(sp_HUD_H_KC3,            ":HUD:H_KC3")
X(sp_HUD_H_KS1,            ":HUD:H_KS1")
X(sp_HUD_H_KS2,            ":HUD:H_KS2")
X(sp_HUD_H_KS3,            ":HUD:H_KS3")
X(sp_HUD_H_W3,             ":HUD:H_W3")
X(sp_Terminal_Teleport,    ":Terminal:Teleport")
X(sp_UI_AttrBar1,          ":UI:AttrBar1")
X(sp_UI_AttrBar2,          ":UI:AttrBar2")
X(sp_UI_AttrBar4,          ":UI:AttrBar4")
X(sp_UI_Bag,               ":UI:Bag")
X(sp_UI_Body,              ":UI:Body")
X(sp_UI_Cursor,            ":UI:Cursor")
X(sp_UI_CursorBlue,        ":UI:CursorBlue")
X(sp_UI_CursorOrange,      ":UI:CursorOrange")
X(sp_UI_CursorOutline,     ":UI:CursorOutline")
X(sp_UI_CursorOutline2,    ":UI:CursorOutline2")
X(sp_UI_CursorOutline2Inv, ":UI:CursorOutline2Inv")
X(sp_UI_CursorOutlineInv,  ":UI:CursorOutlineInv")
X(sp_UI_CursorPink,        ":UI:CursorPink")
X(sp_UI_CursorRed,         ":UI:CursorRed")
X(sp_UI_CursorWhite,       ":UI:CursorWhite")
X(sp_UI_InvSel,            ":UI:InvSel")
X(sp_UI_LogList,           ":UI:LogList")
X(sp_UI_MagicSelectBack,   ":UI:MagicSelectBack")
X(sp_UI_TextBoxBack,       ":UI:TextBoxBack")
X(sp_UI_bip,               ":UI:bip")
X(sp_lfiles_Cyberspace,    "lfiles/Cyberspace.txt")
X(sp_lmusic_DOGS,          "lmusic/DOGS.ogg")
X(sp_lmusic_Unce,          "lmusic/Unce.ogg")
X(sp_lsounds_Silence,      "lsounds/Silence")
X(sp_star,                 "*")

/* Sounds */
X(ss_amb_rain,                 "amb/rain")
X(ss_amb_wind,                 "amb/wind")
X(ss_enemies_boss_talk,        "enemies/boss/talk")
X(ss_enemies_phantom_escape,   "enemies/phantom/escape")
X(ss_enemies_phantom_spawned,  "enemies/phantom/spawned")
X(ss_marathon_pickup,          "marathon/pickup")
X(ss_misc_telein,              "misc/telein")
X(ss_misc_teleout,             "misc/teleout")
X(ss_player_YOUVEGOTMAIL,      "player/YOUVEGOTMAIL")
X(ss_player_adren_inj,         "player/adren/inj")
X(ss_player_adren_ready,       "player/adren/ready")
X(ss_player_cbi_auto_invalid,  "player/cbi/auto/invalid")
X(ss_player_cbi_dlgopen,       "player/cbi/dlgopen")
X(ss_player_cbi_find,          "player/cbi/find")
X(ss_player_cbi_finddone,      "player/cbi/finddone")
X(ss_player_cbi_findfail,      "player/cbi/findfail")
X(ss_player_cbi_invcur,        "player/cbi/invcur")
X(ss_player_cbi_invmov,        "player/cbi/invmov")
X(ss_player_cbi_invrem,        "player/cbi/invrem")
X(ss_player_cbi_keypress,      "player/cbi/keypress")
X(ss_player_cbi_mail,          "player/cbi/mail")
X(ss_player_cbi_scroll,        "player/cbi/scroll")
X(ss_player_cbi_scrollend,     "player/cbi/scrollend")
X(ss_player_death1,            "player/death1")
X(ss_player_doublejump,        "player/doublejump")
X(ss_player_levelup,           "player/levelup")
X(ss_player_manafull,          "player/manafull")
X(ss_player_pickup_item,       "player/pickup/item")
X(ss_player_pickup_upgrbody,   "player/pickup/upgrbody")
X(ss_player_pickup_upgrextr,   "player/pickup/upgrextr")
X(ss_player_pickup_upgrweap,   "player/pickup/upgrweap")
X(ss_player_rarmor_mode,       "player/rarmor/mode")
X(ss_player_rocketboost,       "player/rocketboost")
X(ss_player_score,             "player/score")
X(ss_player_slide,             "player/slide")
X(ss_player_trmopen,           "player/trmopen")
X(ss_player_trmswitch,         "player/trmswitch")
X(ss_player_zoomin,            "player/zoomin")
X(ss_player_zoomout,           "player/zoomout")
X(ss_weapons_cercle_attack,    "weapons/cercle/attack")
X(ss_weapons_cercle_begin,     "weapons/cercle/begin")
X(ss_weapons_rifle_firemode,   "weapons/rifle/firemode")
X(ss_weapons_rocket_mark,      "weapons/rocket/mark")
X(ss_weapons_subweapon_charge, "weapons/subweapon/charge")

#undef X
#elif !defined(m_stab_h)
#define m_stab_h

#define X(n, s) extern str const n;
#include "m_stab.h"

#define Str(n, s) static str const n = s
#define StrAry(n, ...) static str const n[] = {__VA_ARGS__}

#endif
