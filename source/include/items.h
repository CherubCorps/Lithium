/* ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 *
 * Inventory items.
 *
 * ---------------------------------------------------------------------------|
 */

#ifndef items_h
#define items_h

enum ZscName(Container) {
   _cont_store,
   _cont_arms_u,
   _cont_arms_l,
   _cont_body,
};

#if !ZscOn
#define for_item(cont) for_list(struct item *it, (cont)->head)

enum {
   _inv_backpack,
   _inv_arm_upper_l,
   _inv_arm_upper_r,
   _inv_arm_lower_l,
   _inv_arm_lower_r,
   _inv_belt,
   _inv_leg_l,
   _inv_leg_r,
   _inv_torso,
   _inv_legs,
   _inv_num,
};

enum {
   _if_equippable,
   _if_openable,
};

struct itemdata {
   str name, spr;
   u32 w, h;
   u32 equip;
   i96 scr;
   u32 flags;

   /* these need to be scripts or they'll disappear on map load
    * (we shouldn't use a MInit callback because it would cause too much
    * recursion for the VM to handle)
    */
   script bool (*Use)(struct item *);
   script void (*Tick)(struct item *);
   script void (*Show)(struct item *, struct gui_state *, i32);
   script void (*Destroy)(struct item *);
   script void (*Place)(struct item *, struct container *);
};

struct item {
   anonymous struct itemdata data;

   u32 x, y;

   struct container *container;

   struct list link;
};

struct container {
   u32          w, h;
   cstr         name;
   i32          type;
   struct list *head;
};

struct bagitem {
   anonymous struct item item;
   struct container content;
};

optargs(1) struct item *P_Item_New(struct itemdata const *data);
void P_Item_Unlink(struct item *item);

script void P_Item_Destroy(struct item *item);
script bool P_Item_Use(struct item *item);
script void P_Item_Place(struct item *item, struct container *cont);

optargs(1) struct bagitem *P_BagItem_New(i32 w, i32 h, i32 type, struct itemdata const *data);

void P_Inv_PInit(void);
void P_Inv_PQuit(void);

bool P_Inv_Place(struct container *cont, struct item *item, i32 x, i32 y);
script bool P_Inv_PlaceFirst(struct container *cont, struct item *item);
script bool P_Inv_SwapFirst(struct container *cont, struct item *lhs);
bool P_Inv_Swap(struct item *lhs, struct item *rhs);
bool P_Inv_Add(struct item *item);
#endif

#endif
