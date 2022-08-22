// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Map metadata reading.                                                    │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#include "m_engine.h"
#include "w_world.h"
#include "m_file.h"
#include "m_tokbuf.h"
#include "m_trie.h"

noinit static struct map_info mi_all[128];
noinit static i32 mi_num;
static void ReadKeys(struct tokbuf *tb, struct err *res) {
   noinit static char k[16], v[64];
   struct map_info *mi = &mi_all[mi_num];
   while(tb_kv(tb, res, k, v)) {
      unwrap(res);
      i32 key = MapInfoKeyName(k);
      set_bit(mi->use, key);
      switch(key) {
      case _mi_key_name:
      case _mi_key_hash:
         mi->keys[key] = (i32)fast_strdup(v);
         break;
      case _mi_key_humidity:
      case _mi_key_seed:
      case _mi_key_temperature:
      case _mi_key_windspeed:
         mi->keys[key] = faststrtoi32(v);
         break;
      case _mi_key_environment:
         mi->keys[key] = MapInfoEnvName(v);
         break;
      case _mi_key_sky:
         mi->keys[key] = MapInfoSkyName(v);
         break;
      case _mi_key_flags:
         mi->keys[key] = tb_rflag(tb, res, v, MapInfoFlagName);
         unwrap(res);
         break;
      default:
         tb_err(tb, res, "invalid key %s", nil, _f, k);
         unwrap_retn();
      }
   }
   if(!get_bit(mi->use, _mi_key_name) && !get_bit(mi->use, _mi_key_hash)) {
      tb_err(tb, res, "requires name or hash field", nil, _f);
      unwrap_retn();
   }
   mi_num++;
}
script struct map_info *GetMapInfo(void) {
   noinit static bool mi_init;
   if(!mi_init) {
      i32 prev = 0;
      for(FILE *fp; (fp = W_OpenIter(sp_LITHMAP, 't', &prev));) {
         struct tokbuf tb;
         tb_ctor(&tb, fp, "LITHMAP");
         struct err res = {};
         do {
            ReadKeys(&tb, &res);
            unwrap_do(&res, {
               PrintErr(_p(res.err));
               break;
            });
         } while(!tb_drop(&tb, tok_eof) && !res.some);
         tb_dtor(&tb);
         fclose(fp);
      }
      mi_init = true;
   }
   str hash = EDataS(_edt_maphash);
   for(i32 i = 0; i < mi_num; ++i) {
      struct map_info *mi = &mi_all[i];
      bool ok = true;
      if(get_bit(mi->use, _mi_key_hash)) {
         ok = ok && hash == (str)mi->keys[_mi_key_hash];
      }
      if(get_bit(mi->use, _mi_key_name)) {
         ok = ok && ml.lump == (str)mi->keys[_mi_key_name];
      }
      if(ok) {
         return mi;
      }
   }
   return &mi_all[mi_num];
}
