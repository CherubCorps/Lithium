// Copyright © 2017-2018 Graham Sanderson, all rights reserved.
#include "lith_common.h"
#include "lith_player.h"
#include "lith_world.h"

// Static Functions ----------------------------------------------------------|

//
// BagItem_Tick
//
script
static void BagItem_Tick(item_t *_item)
{
   bagitem_t *item = (bagitem_t *)_item;

   foritem(item->content)
      if(it->Tick) it->Tick(it);
}

//
// BagItem_Place
//
script
static void BagItem_Place(item_t *_item, container_t *cont)
{
   bagitem_t *item = (bagitem_t *)_item;

   Lith_Item_Place(&item->item, cont);

   item->content.user = item->user;

   foritem(item->content)
      it->Place(it, &item->content);
}

//
// BagItem_Destroy
//
script
static void BagItem_Destroy(item_t *_item)
{
   bagitem_t *item = (bagitem_t *)_item;

   foritem(item->content)
      it->Destroy(it);

   Lith_Item_Destroy(&item->item);
}

//
// ItemCanPlace
//
static bool ItemCanPlace(container_t *cont, item_t *item, int x, int y)
{
   if(x < 0 || y < 0)
      return false;

   int x2 = x + item->w;
   int y2 = y + item->h;

   if(x2 > cont->w || y2 > cont->h)
      return false;

   int z = x + item->w;
   int w = y + item->h;

   foritem(*cont) if(it != item)
   {
      int const u = it->x;
      int const v = it->y;
      int const s = u + it->w;
      int const t = v + it->h;

      if(aabb(u, v, s, t, x, y) || aabb(u, v, s, t, z - 1, w - 1) ||
         aabb(x, y, z, w, u, v) || aabb(x, y, z, w, s - 1, t - 1))
         return false;
   }

   return true;
}

//
// ItemCanPlaceAny
//
static bool ItemCanPlaceAny(container_t *cont, item_t *item)
{
   int xn = cont->w / item->w;
   int yn = cont->h / item->h;

   for(int y = 0; y < yn; y++) for(int x = 0; x < xn; x++)
      if(ItemCanPlace(cont, item, x * item->w, y * item->h))
         return true;

   return false;
}

// Extern Functions ----------------------------------------------------------|

//
// Lith_Item_Init
//
void Lith_Item_Init(item_t *item, itemdata_t const *data)
{
   item->link.construct(item);

   if(data) item->data = *data;
   else     Log("invalid item, developer is an idiot");

   if(!item->Destroy) item->Destroy = Lith_Item_Destroy;
   if(!item->Place  ) item->Place   = Lith_Item_Place;
}

//
// Lith_Item_New
//
item_t *Lith_Item_New(itemdata_t const *data)
{
   item_t *item = Salloc(item_t);

   Lith_Item_Init(item, data);

   return item;
}

//
// Lith_Item_Destroy
//
script
void Lith_Item_Destroy(item_t *item)
{
   LogDebug(log_dev, "Lith_Item_Destroy: destroying item %p", item);

   HERMES("DeleteItem", item);

   withplayer(item->user)
   {
      if(p->useitem == item) p->useitem = null;
      if(p->selitem == item) p->selitem = null;
      p->movitem = false;
   }

   Lith_Item_Unlink(item);
   Dalloc(item);
}

//
// Lith_Item_Use
//
script
bool Lith_Item_Use(item_t *item)
{
   return HERMES("UseItem", item);
}

//
// Lith_Item_Place
//
script
void Lith_Item_Place(item_t *item, container_t *cont)
{
   Lith_Item_Unlink(item);
   item->link.link(&cont->items);
   item->container = cont;
   item->user = cont->user;
}

//
// Lith_Item_Unlink
//
void Lith_Item_Unlink(item_t *item)
{
   if(item->container)
   {
      item->link.unlink();
      item->container = null;
   }
}

//
// Lith_BagItem_New
//
bagitem_t *Lith_BagItem_New(int w, int h, __str bg, itemdata_t const *data)
{
   bagitem_t *item = Salloc(bagitem_t);

   Lith_Item_Init(&item->item, data);

   item->link.construct(item);

   item->content.w  = w;
   item->content.h  = h;
   item->content.bg = bg;

   item->Tick    = BagItem_Tick;
   item->Destroy = BagItem_Destroy;
   item->Place   = BagItem_Place;

   return item;
}

//
// Lith_ItemPlace
//
bool Lith_ItemPlace(container_t *cont, item_t *item, int x, int y)
{
   if(!ItemCanPlace(cont, item, x, y)) return false;

   item->Place(item, cont);

   item->x = x;
   item->y = y;

   return true;
}

//
// Lith_ItemPlaceFirst
//
script
bool Lith_ItemPlaceFirst(container_t *cont, item_t *item)
{
   for(int y = 0; y < cont->h; y++) for(int x = 0; x < cont->w; x++)
      if(Lith_ItemPlace(cont, item, x, y))
         return true;

   return false;
}

//
// Lith_PlayerAddItem
//
bool Lith_PlayerAddItem(struct player *p, item_t *item)
{
   for(int i = 0; i < countof(p->inv); i++)
      if(Lith_ItemPlaceFirst(&p->inv[i], item))
         return true;

   return false;
}

//
// Lith_Container
//
void Lith_Container(gui_state_t *g, container_t *cont, int sx, int sy)
{
   struct player *p = cont->user;

   __str bg = cont->bg ? cont->bg : ":UI:InvBack";
   int h = cont->h * 8;
   int w = cont->w * 8;

   for(int y = 0; y < h; y += 8) for(int x = 0; x < w; x += 8)
      PrintSpriteA(bg, sx+x,1, sy+y,1, 0.8);

   if(p && p->movitem && g->clicklft && aabb(sx, sy, sx+w, sy+h, g->cx, g->cy))
      if(Lith_ItemPlace(cont, p->selitem, (g->cx - sx) / 8, (g->cy - sy) / 8))
   {
      p->movitem = false;
      ACS_LocalAmbientSound("player/cbi/invmov", 127);
   }

   foritem(*cont)
   {
      int x = sx + it->x * 8;
      int y = sy + it->y * 8;
      int ex = x + it->w * 8;
      int ey = y + it->h * 8;

      PrintSprite(it->spr, x,1, y,1);

      if(!p || p->movitem) continue;

      if(p->selitem != it && g->clicklft && aabb(x, y, ex, ey, g->cx, g->cy))
      {
         p->selitem = it;
         ACS_LocalAmbientSound("player/cbi/invcur", 127);
      }

      if(p->selitem == it)
      {
         fixed a = (ACS_Sin(ACS_Timer() / 105.0) * 0.5 + 1.2) / 4;

         for(int xx = x; y < ey; y += 8) for(x = xx; x < ex; x += 8)
            PrintSpriteA(":UI:InvSel", x,1, y,1, a);
      }
   }
}

//
// Lith_PlayerUpdateInventory
//
script
void Lith_PlayerUpdateInventory(struct player *p)
{
   if(p->useitem)
   {
      item_t *item = p->useitem;

      LogDebug(log_dev, "using %S (%p)", item->name, item);
      if(item->Use && !item->Use(item))
         ACS_LocalAmbientSound("player/cbi/auto/invalid", 127);

      p->useitem = null;
   }

   for(int i = 0; i < countof(p->inv); i++)
      foritem(p->inv[i])
         if(it->Tick) it->Tick(it);

   foritem(p->misc)
      if(it->Tick) it->Tick(it);
}

//
// Lith_ItemCreate
//
script ext("ACS")
void *Lith_ItemCreate(int w, int h)
{
   __str type = getmems(0, "m_invtype");
   __str tag  = getmems(0, "m_invname");
   u32   scr  = getmem (0, "m_invsell");
   __str spr  = StrParam(":ItemSpr:%S", tag);
   __str name = ACS_GetActorPropertyString(0, APROP_NameTag);

   LogDebug(log_dev, "Lith_ItemCreate: creating %S (%S) %S", type, tag, spr);

   #define Type(t, ...) \
      if(type == t) \
         return Lith_Item_New(&(itemdata_t const){name, spr, tag, w, h, scr, __VA_ARGS__})

   Type("SlottedItem", .Use = Lith_Item_Use);
   Type("Armor",       .Use = Lith_Item_Use);

   return null;
}

//
// Lith_ItemAttach
//
script ext("ACS")
bool Lith_ItemAttach(void *_item)
{
   item_t *item = _item;

   LogDebug(log_dev, "Lith_ItemAttach: attaching item %p", item);

   withplayer(LocalPlayer)
   {
      p->bipUnlock(item->tag);
      return p->addItem(item);
   }

   return false;
}

//
// Lith_ItemDetach
//
script ext("ACS")
void Lith_ItemDetach(void *_item)
{
   item_t *item = _item;

   LogDebug(log_dev, "Lith_ItemDetach: detaching item %p", item);

   item->Destroy(item);
}

//
// Lith_ItemUnlink
//
script ext("ACS")
void Lith_ItemUnlink(void *_item)
{
   item_t *item = _item;

   LogDebug(log_dev, "Lith_ItemUnlink: unlinking item %p", item);

   withplayer(LocalPlayer)
   {
      item->Place(item, &p->misc);
      item->x = item->y = 0;

      p->selitem = null;
      p->movitem = false;
   }
}

//
// Lith_ItemCanPlace
//
script ext("ACS")
bool Lith_ItemCanPlace(void *_item)
{
   item_t *item = _item;

   withplayer(LocalPlayer)
      for(int i = 0; i < countof(p->inv); i++)
         if(ItemCanPlaceAny(&p->inv[i], item))
            return true;

   return false;
}

//
// Lith_CBITab_Items
//
void Lith_CBITab_Items(gui_state_t *g, struct player *p)
{
   static int const x[] = {
      155+8*-14, // Backpack
      155+8*  1, // L Upper Arm
      155+8*  8, // R Upper Arm
      155+8*  0, // L Lower Arm
      155+8*  9, // R Lower Arm
      155+8*  3, // Belt
      155+8*  1, // L Leg
      155+8*  7, // R Leg
   };

   static int const y[] = {
      80+8*-1, // Backpack
      80+8*-2, // L Upper Arm
      80+8*-2, // R Upper Arm
      80+8* 2, // L Lower Arm
      80+8* 2, // R Lower Arm
      80+8* 5, // Belt
      80+8* 9, // L Leg
      80+8* 9, // R Leg
   };

   PrintSpriteA(":UI:Body", 151,1, 40,1, 0.6);
   PrintSpriteA(":UI:Bag",  47 ,1, 44,1, 0.6);

   for(int i = 0; i < countof(p->inv); i++)
      Lith_Container(g, &p->inv[i], x[i], y[i]);

   item_t *sel = p->selitem;

   if(sel)
   {
      int x_ = x[0];
      int y_ = y[0] + 60;

      PrintTextStr(sel->name);
      PrintText("CBIFONT", CR_WHITE, x_,1, y_,1);
      y_ += 8;

      if(g->clickrgt && !g->old.clickrgt)
         p->movitem = !p->movitem;

      if(Lith_GUI_Button(g, "Move", x_, y_, Pre(btnclear)))
         p->movitem = !p->movitem;
      y_ += 8;

      if(sel->Use)
      {
         if(Lith_GUI_Button(g, "Use", x_, y_, Pre(btnclear)))
            p->useitem = sel;
         y_ += 8;
      }

      if(sel->scr)
      {
         PrintTextFmt("(%S\Cnscr\C-)", scoresep(sel->scr));
         PrintText("CBIFONT", CR_WHITE, x_+18,1, y_,1);
      }

      if(Lith_GUI_Button(g, sel->scr ? "Sell" : "Discard", x_, y_, Pre(btnclear)))
      {
         if(sel->scr) p->giveScore(sel->scr, true);
         sel->Destroy(sel);
         ACS_LocalAmbientSound("player/cbi/invrem", 127);
      }
   }

   PrintTextFmt("Equipped (%S)", HERMES_S("GetArmorDT"));
   PrintText("CBIFONT", CR_WHITE, 40,1, 38,1);

   for(int i = 0; i < aslot_max; i++)
      ifw(__str name = HERMES_S("GetArmorSlot", i), name != "")
   {
      PrintTextStr(name);
      PrintText("CBIFONT", CR_WHITE, 40,1, 45+7*i,1);
   }
}

// EOF
