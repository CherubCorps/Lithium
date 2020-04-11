/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Generalized HUD functions.
 *
 * ---------------------------------------------------------------------------|
 */

#ifndef p_hud_h
#define p_hud_h

#include "w_world.h"

void HUD_WeaponSlots(struct player *p, i32 cr_one, i32 cr_many, i32 cr_none, i32 cr_cur, i32 _x, i32 _y);
void HUD_Score(struct player *p, cstr fmt, i96 scr, str font, i32 cr, i32 x, i32 xa);
void HUD_KeyInd(struct player *p, i32 x, i32 y, bool horz, k32 a);
script void HUD_Log(struct player *p, i32 cr, i32 x, i32 yy);
void HUD_DrawHealth(struct player *p, i32 health, i32 x, i32 y, i32 cr, i32 cr_fade);

#endif
