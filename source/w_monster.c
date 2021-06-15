/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Monster entry points.
 *
 * ---------------------------------------------------------------------------|
 */

#include "w_monster.h"
#include "p_player.h"
#include "w_world.h"
#include "m_file.h"
#include "m_tokbuf.h"

#define HasResistances(m) ((m)->rank >= 2)

/* Types ------------------------------------------------------------------- */

enum {
   _max_rank  = 5,
   _max_level = 150,
};

enum {
   #define monster_flag_x(flg) _mif_##flg,
   #include "w_monster.h"
};

struct monster_preset {
   char prename[64];
   u64  exp;
   i96  score;
};

struct monster_info {
   anonymous struct monster_preset pre;
   enum mtype type;
   char       name[64];
   i32        flags;
};

/* Static Objects ---------------------------------------------------------- */

noinit static struct monster_preset monsterpreset[128];
noinit static struct monster_info   monsterinfo[256];
noinit static mem_size_t monsterpresetnum, monsterinfonum;

StrAry(dmgtype_names,
       s"Bullets",
       s"Energy",
       s"Fire",
       s"Magic",
       s"Melee",
       s"Shrapnel");

/* Static Functions -------------------------------------------------------- */

alloc_aut(0) stkcall static
void GetInfo(dmon_t *m) {
   m->x = GetX(0);
   m->y = GetY(0);
   m->z = GetZ(0);

   m->r = GetRadius(0);
   m->h = GetHeight(0);

   m->health = GetHealth(0);
}

script static
void ApplyLevels(dmon_t *m, i32 prev) {
   GetInfo(m);

   for(i32 i = prev + 1; i <= m->level; i++) {
      if(i % 10 == 0) {
         /* if we have resistances, randomly pick a resistance we
          * already have */
         if(HasResistances(m)) {
            i32 r;
            do {r = ACS_Random(0, dmgtype_max-1);} while(m->resist[r] == 0);
            m->resist[r] += 2;
         }
      }
   }

   if(m->level >= 5) {
      k32 rn = m->rank / 10.0;
      i96 delt = m->level - prev;
      i96 hp10 = m->spawnhealth / 10;
      i32 newh = delt * hp10 * (i96)(ACS_RandomFixed(rn - 0.1, rn + 0.1) * 0xfff) / 0xfff;
      Dbg_Log(log_dmonV, "monster %i: newh %i", m->id, newh);
      SetHealth(0, m->health + newh);
      m->maxhealth += newh;
   }

   for(i32 i = 0; i < dmgtype_max; i++) {
      ifauto(i32, resist, m->resist[i] / 15.0) {
         InvGive(StrParam(OBJ "M_%S%i", dmgtype_names[i], min(resist, _max_rank)), 1);
      }
   }

   Dbg_Log(log_dmon, "monster %i leveled up (%i -> %i)", m->id, prev, m->level);
}

script static
void ShowBarrier(dmon_t const *m, k32 alpha) {
   bool anyplayer = false;

   /* Optimization: Check for players nearby first. */
   if(aabb_point(m->x - 192, m->y - 192, 384, 384, pl.x, pl.y)) {
      anyplayer = true;
   }

   if(!anyplayer)
      return;

   BeginAngles(m->x, m->y);
   ServCallI(sm_MonsterBarrierLook);

   for(i32 i = 0; i < a_cur; i++)
   {
      struct polar *a = &a_angles[i];

      k32 dst = m->r / 2 + a->dst / 4;
      k32 x   = m->x + ACS_Cos(a->ang) * dst;
      k32 y   = m->y + ACS_Sin(a->ang) * dst;
      i32 tid = ACS_UniqueTID();
      str bar = m->rank >= 5 ? so_MonsterHeptaura : so_MonsterBarrier;

      ACS_SpawnForced(bar, x, y, m->z + m->h / 2, tid);
      SetAlpha(tid, (1 - a->dst / (256 * (m->rank - 1))) * alpha);
   }
}

script static
void BaseMonsterLevel(dmon_t *m) {
   k32 rlv = ACS_RandomFixed(1, _max_level);
   k32 bias;

   bias = mapscleared / 40.0;

   bias *= bias;
   bias += ACS_GameSkill() / (k32)skill_nightmare * 0.1;
   bias += CVarGetI(sc_sv_difficulty) / 100.0;
   bias *= ACS_RandomFixed(1, 1.5);

   if(get_bit(m->mi->flags, _mif_angelic)) {
      m->rank  = 7;
      m->level = 7 + rlv * bias;
   } else if(get_bit(m->mi->flags, _mif_dark)) {
      m->rank  = 6;
      m->level = 6 + rlv * bias;
   } else if(GetFun() & lfun_ragnarok) {
      m->rank  = _max_rank;
      m->level = _max_level + rlv * bias;
   } else {
      k32 rrn = ACS_RandomFixed(1, _max_rank);

      rlv += pl.attr.level / 2.0;

      m->rank  = minmax(rrn * bias * 2, 1, _max_rank);
      m->level = minmax(rlv * bias    , 1, _max_level);
   }

   switch(m->rank) {
      case 5: ServCallI(sm_SetTeleFogTo, so_TeleFog5); break;
      case 6: ServCallI(sm_SetTeleFogTo, so_TeleFog6); break;
      case 7: ServCallI(sm_SetTeleFogTo, so_TeleFog7); break;
   }

   if(HasResistances(m)) for(i32 i = 0; i < m->rank; i++)
      m->resist[ACS_Random(1, dmgtype_max) - 1] += 5;

   ApplyLevels(m, 0);

   Dbg_Log(log_dmon, "monster %-4i \Cdr%i \Cgl%-3i \C-running on %S",
      m->id, m->rank, m->level, ACS_GetActorClass(0));

   #ifndef NDEBUG
   if(get_bit(dbglevel, log_dmonV))
      PrintMonsterInfo(m);
   #endif
}

/* Spawn a Monster Soul and temporarily set the species of it until the
 * actor is no longer solid, so it won't explode immediately.
 */
alloc_aut(0) script static
void SoulCleave(dmon_t *m) {
   Str(sm_solid, s"SOLID");

   i32 tid = ACS_UniqueTID();
   ACS_SpawnForced(so_MonsterSoul, m->x, m->y, m->z + 16, tid);
   SetDamage(tid, m->level / 8 * 7);

   PtrSet(tid, AAPTR_DEFAULT, AAPTR_TARGET, pl.tid);
   SetSpecies(tid, GetSpecies(0));

   for(i32 i = 0; ACS_CheckFlag(0, sm_solid) && i < 15; i++) ACS_Delay(1);

   SetSpecies(tid, so_Player);
}

static
void SpawnManaPickup(dmon_t *m) {
   i32 i = 0;
   do {
      i32 tid = ACS_UniqueTID();
      i32 x   = m->x + ACS_Random(-16, 16);
      i32 y   = m->y + ACS_Random(-16, 16);
      ACS_Spawn(so_ManaPickup, x, y, m->z + 4, tid);
      PtrSet(tid, AAPTR_DEFAULT, AAPTR_TRACER, pl.tid);
      PtrSet(tid, AAPTR_DEFAULT, AAPTR_TARGET, pl.tid);
      i += 150;
   } while(i < m->maxhealth);
}

static
void OnFinalize(dmon_t *m) {
   if(!P_None() && PtrPlayerNumber(0, AAPTR_TARGET) >= 0) {
      if(pl.sgacquired) {
         bool high_level_imp =
            m->mi->type == mtype_imp && m->level >= 70 && m->rank >= 4;
         if(high_level_imp && ACS_Random(0, 100) < 4)
            ACS_SpawnForced(so_ClawOfImp, m->x, m->y, m->z);
      }

      if(!m->finalized) {
         if(get_bit(pl.upgrades[UPGR_Magic].flags, _ug_active) &&
            pl.mana != pl.manamax &&
            (m->mi->type != mtype_zombie || ACS_Random(0, 50) < 10)) {
            SpawnManaPickup(m);
         }

         if(CVarGetI(sc_sv_wepdrop)) {
            Str(so_sgun, s"Shotgun");
            Str(so_cgun, s"Chaingun");
            str sp = snil;
            switch(m->mi->type) {
               case mtype_zombiesg: if(!pl.weapon.slot[3]) sp = so_sgun; break;
               case mtype_zombiecg: if(!pl.weapon.slot[4]) sp = so_cgun; break;
            }
            if(sp) {
               Str(sm_dropped, s"DROPPED");
               i32 tid = ACS_UniqueTID();
               ACS_SpawnForced(sp, m->x, m->y, m->z, tid);
               ACS_SetActorFlag(tid, sm_dropped, false);
            }
         }

         if(get_bit(pl.upgrades[UPGR_SoulCleaver].flags, _ug_active))
            SoulCleave(m);
      }

           if(pl.health <  5) P_Lv_GiveEXP(50);
      else if(pl.health < 15) P_Lv_GiveEXP(25);
      else if(pl.health < 25) P_Lv_GiveEXP(10);

      P_GiveEXP(m->mi->exp + m->level + (m->rank - 1) * 10);
   }

   m->finalized = true;
}

script static
void OnDeath(dmon_t *m) {
   Dbg_Log(log_dmon, "monster %i is ded", m->id);

   m->wasdead = true;

   OnFinalize(m);

   P_GiveScore(m->mi->score + m->rank * 500, false);
}

script static
void OnResurrect(dmon_t *m) {
   Dbg_Log(log_dmon, "monster %i resurrected", m->id);

   m->resurrect = false;
}

_Noreturn alloc_aut(0) script static
void MonsterMain(dmon_t *m) {
   InvGive(so_MonsterID, m->id + 1);

   GetInfo(m);
   m->spawnhealth = m->maxhealth = m->health;

   BaseMonsterLevel(m);

   for(i32 tic = 0;; tic++) {
      GetInfo(m);

      if(m->health <= 0) {
         OnDeath(m);

         do ACS_Delay(15); while(!m->resurrect);

         OnResurrect(m);
      }

      if(m->exp > _monster_level_exp) {
         i32 prev = m->level;

         div_t d = __div(m->exp, _monster_level_exp);
         m->level += d.quot;
         m->exp    = d.rem;

         ACS_SpawnForced(so_MonsterLevelUp, m->x, m->y, m->z);
         ApplyLevels(m, prev);
      }

      if(HasResistances(m) && m->level >= 20) {
         ShowBarrier(m, m->level / 100.0);
      }

      if(InvNum(so_Ionized) && tic % 5 == 0) {
         ServCallI(sm_IonizeFX);
      }

      ACS_Delay(2);
   }
}

/* Extern Functions -------------------------------------------------------- */

#ifndef NDEBUG
void PrintMonsterInfo(dmon_t *m) {
   Log("%p (%p) %s active: %u id: %.3u\n"
       "wasdead: %u finalized: %u painwait: %i\n"
       "level: %.3i rank: %i exp: %i\n"
       "health: %i/%i\n"
       "x: %k y: %k z: %k\n"
       "r: %k h: %k\n"
       "mi->exp: %lu mi->score: %lli\n"
       "mi->flags: %i mi->type: %i",
       m, m->mi, m->mi->name, m->active, m->id,
       m->wasdead, m->finalized, m->painwait,
       m->level, m->rank, m->exp,
       m->health, m->maxhealth,
       m->x, m->y, m->z,
       m->r, m->h,
       m->mi->exp, m->mi->score,
       m->mi->flags, m->mi->type);

   for(i32 i = 0; i < countof(m->resist); i++)
      Log("resist %S: %i", dmgtype_names[i], m->resist[i]);
}
#endif

bool MonInfo_Kv(struct tokbuf *tb, struct tbuf_err *res, cstr *kp, cstr *vp) {
   noinit
   static char k[64], v[64];
   struct token *tok = tb->expc2(res, tb->get(), tok_semico, tok_identi);
   unwrap(res);
   if(tok->type == tok_semico) {
      return false;
   } else {
      faststrcpy(k, tok->textV);
      tb->expdr(res, tok_col);
      unwrap(res);
      tok = tb->expc3(res, tb->get(), tok_identi, tok_number, tok_string);
      unwrap(res);
      faststrcpy(v, tok->textV);
      *kp = k;
      *vp = v;
      return true;
   }
}

script
void MonInfo_Preset(struct tokbuf *tb, struct tbuf_err *res) {
   struct monster_preset *pre = &monsterpreset[monsterpresetnum++];
   cstr k = nil, v = nil;

   while(MonInfo_Kv(tb, res, &k, &v)) {
      unwrap(res);
      if(faststrcmp(k, "name") == 0) {
         faststrcpy(pre->prename, v);
      } else if(faststrcmp(k, "exp") == 0) {
         pre->exp = faststrtou64(v);
      } else if(faststrcmp(k, "scr") == 0) {
         pre->score = faststrtoi96(v);
      } else {
         tb->err(res, "%s MonInfo_Preset: invalid key '%s'", TokPrint(tb->reget()), k);
         unwrap(res);
      }
   }
   unwrap(res);

   Dbg_Log(log_dmonV, "preset %s added: exp = %lu, score = %lli",
           pre->prename, pre->exp, pre->score);
}

script
void MonInfo_Monster(struct tokbuf *tb, struct tbuf_err *res, i32 flags) {
   struct monster_info *mi = &monsterinfo[monsterinfonum++];
   cstr k = nil, v = nil;

   mi->flags = flags;

   while(MonInfo_Kv(tb, res, &k, &v)) {
      unwrap(res);
      if(faststrcmp(k, "filter") == 0) {
         faststrcpy(mi->name, v);
      } else if(faststrcmp(k, "exp") == 0) {
         mi->exp = faststrtou64(v);
      } else if(faststrcmp(k, "scr") == 0) {
         mi->score = faststrtoi96(v);
      } else if(faststrcmp(k, "pre") == 0) {
         for(struct monster_preset *pre = monsterpreset;; ++pre) {
            if(faststrcmp(pre->prename, v) == 0) {
               mi->pre = *pre;
               break;
            }
         }
      } else if(faststrcmp(k, "type") == 0) {
         #define monster_type_x(name) \
            if(faststrcmp(v, #name) == 0) { \
               mi->type = mtype_##name; \
               continue; \
            }
         #include "w_monster.h"
      } else {
         tb->err(res, "%s MonInfo_Monster: invalid key '%s'", TokPrint(tb->reget()), k);
         unwrap(res);
      }
   }
   unwrap(res);

   Dbg_Log(log_dmonV, "monster %s added: type = %i, flags = %i",
           mi->name, mi->type, mi->flags);
}

script
i32 MonInfo_Flags(struct tokbuf *tb, struct tbuf_err *res) {
   i32 flags = 0;

   for(;;) {
      struct token *tok = tb->expc2(res, tb->get(), tok_identi, tok_parenc);
      unwrap(res);

      if(tok->type == tok_parenc) {
         break;
      }
      #define monster_flag_x(name) \
      else if(faststrcmp(tok->textV, #name) == 0) {\
         set_bit(flags, _mif_##name); \
      }
      #include "w_monster.h"
      else {
         tb->err(res, "%s MonInfo_Flags: invalid flag; expected "
                 #define monster_flag_x(name) #name ", "
                 #include "w_monster.h"
                 "or `)'",
                 TokPrint(tok));
         unwrap(res);
      }
   }

   return flags;
}

script
void MonInfo_Compile(struct tokbuf *tb, struct tbuf_err *res) {
   for(;;) {
      struct token *tok =
         tb->expc3(res, tb->get(), tok_pareno, tok_identi, tok_eof);
      unwrap(res);

      switch(tok->type) {
      default:
      case tok_eof:
         return;
      case tok_pareno: {
         i32 flags = MonInfo_Flags(tb, res);
         unwrap(res);
         tb->expdr(res, tok_braceo);
         unwrap(res);
         while(!tb->drop(tok_bracec)) {
            MonInfo_Monster(tb, res, flags);
            unwrap(res);
         }
         break;
      }
      case tok_identi:
         if(faststrcmp(tok->textV, "presets") == 0) {
            tb->expdr(res, tok_braceo);
            unwrap(res);
            while(!tb->drop(tok_bracec)) {
               MonInfo_Preset(tb, res);
               unwrap(res);
            }
         } else {
            tb->err(res, "%s MonInfo_Compile: invalid toplevel identifier", TokPrint(tok));
            unwrap(res);
         }
         break;
      }
   }
}

script
void Mon_Init(void) {
   Str(sp_LITHMONS, s"LITHMONS");

   Dbg_Log(log_dev, "Mon_Init");

   FILE *fp;
   i32 prev = 0;

   monsterpresetnum = 0;
   monsterinfonum   = 0;

   while((fp = W_OpenIter(sp_LITHMONS, 't', &prev))) {
      struct tokbuf tb;
      TBufCtor(&tb, fp);

      struct tbuf_err res = {};
      MonInfo_Compile(&tb, &res);
      unwrap_print(&res);

      TBufDtor(&tb);
      fclose(fp);
   }
}

/* Scripts ----------------------------------------------------------------- */

script ext("ACS") addr(lsc_monstertype)
i32 Sc_GetMonsterType() {
   ifauto(dmon_t *, m, DmonSelf()) return m->mi->type;
   else                            return mtype_unknown;
}

script_str ext("ACS") addr(OBJ "GiveMonsterEXP")
void Sc_GiveMonsterEXP(i32 amt) {
   ifauto(dmon_t *, m, DmonSelf()) m->exp += amt;
}

script_str ext("ACS") addr(OBJ "ResurrectMonster")
void Sc_ResurrectMonster(i32 amt) {
   ifauto(dmon_t *, m, DmonSelf()) m->resurrect = true;
}

#ifndef NDEBUG
script static
void LogError(cstr cname) {
   Dbg_Log(log_dmon, "ERROR no monster %s", cname);
}
#endif

alloc_aut(0) script ext("ACS") addr(lsc_monsterinfo)
void Sc_MonsterInfo(void) {
   if(!gblinit) {
      ACS_Delay(2);
   }

   static char cname[64];
   faststrcpy_str(cname, ACS_GetActorClass(0));

   if(faststrstr(cname, "RLAdaptive") || faststrstr(cname, "RLCyberdemonMkII"))
      return;

   for(i32 i = 0; i < monsterinfonum; i++) {
      struct monster_info const *mi = &monsterinfo[i];
      bool init;

      if(get_bit(mi->flags, _mif_full)) {
         init = faststrcmp(cname, mi->name) == 0;
      } else {
         init = faststrstr(cname, mi->name);
      }

      if(init) {
         ACS_Delay(1);

         /* make sure it isn't already dead first */
         if(GetHealth(0) > 0) {
            dmon_t *m = AllocDmon();
            m->mi = mi;
            MonsterMain(m);
         }

         return;
      }
   }

   #ifndef NDEBUG
   LogError(cname);
   #endif
}

script_str ext("ACS") addr(OBJ "MonsterFinalized")
void Sc_MonsterFinalized(void) {
   ifauto(dmon_t *, m, DmonSelf())
      OnFinalize(m);
}

/* EOF */
