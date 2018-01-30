// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
#include "lith_common.h"
#include "lith_player.h"
#include "lith_bip.h"
#include "lith_list.h"
#include "lith_world.h"

#include <ctype.h>

#define ForCategory() for(int categ = BIPC_NONE + 1; categ < BIPC_MAX; categ++)
#define ForPage() forlist(bippage_t *page, bip->infogr[categ])
#define ForCategoryAndPage() ForCategory() ForPage()

// Extern Objects ------------------------------------------------------------|

extern struct page_initializer const bip_pages[];

// Types ---------------------------------------------------------------------|

struct page_info
{
   __str shname;
   __str flname;
   __str body;
};

// Static Functions ----------------------------------------------------------|

//
// DecryptBody
//
static __str DecryptBody(char __str_ars const *str)
{
   ACS_BeginPrint();
   for(; *str; str++)
      ACS_PrintChar(!isprint(*str) ? *str : *str ^ 7);
   return ACS_EndStrParam();
}

//
// GetPageInfo
//
static struct page_info GetPageInfo(bippage_t const *page)
{
   struct page_info pinf;

   pinf.shname = page->category == BIPC_MAIL
      ? page->name
      : Language("LITH_TXT_INFO_SHORT_%S", page->name);

   pinf.body = page->body
      ? page->body
      : Language("LITH_TXT_INFO_DESCR_%S", page->name);

   pinf.flname = page->title
      ? page->title
      : Language("LITH_TXT_INFO_TITLE_%S", page->name);

   if(page->category == BIPC_EXTRA)
      pinf.body = DecryptBody(pinf.body);

   return pinf;
}

//
// SetCurPage
//
static void SetCurPage(gui_state_t *g, bip_t *bip, bippage_t *page, __str body)
{
   bip->curpage = page;

   Lith_GUI_TypeOn(g, &CBIState(g)->biptypeon, body);
   Lith_GUI_ScrollReset(g, &CBIState(g)->bipinfoscr);
}

//
// UnlockPage
//
static void UnlockPage(bip_t *bip, bippage_t *page, int pclass)
{
   bip->pageavail++;
   bip->categoryavail[page->category]++;
   page->unlocked = true;

   for(int i = 0; i < countof(page->unlocks) && page->unlocks[i]; i++)
      bip->unlock(page->unlocks[i], pclass);
}

//
// AddToBIP
//
[[__optional_args(1)]]
static void AddToBIP(bip_t *bip, int categ, int pclass, struct page_initializer const *pinit, bool isfree)
{
   __str image = LanguageNull("LITH_TXT_INFO_IMAGE_%S", pinit->name);
   int height = strtoi_str(Language("LITH_TXT_INFO_CSIZE_%S", pinit->name), null, 0);

   bippage_t *page = salloc(bippage_t);

   page->name     = pinit->name;
   page->category = categ;
   page->unlocked = false;
   page->image    = image;
   page->height   = height;
   memmove(page->unlocks, pinit->unlocks, sizeof(page->unlocks));

   page->link.construct(page);
   page->link.link(&bip->infogr[categ]);

   if(isfree) UnlockPage(bip, page, pclass);
}

// Extern Functions ----------------------------------------------------------|

//
// Lith_PlayerInitBIP
//
[[__call("ScriptS")]]
void Lith_PlayerInitBIP(player_t *p)
{
   bip_t *bip = &p->bip;
   int total = 0;

   ForCategory()
      bip->infogr[categ].free(free);

   __with(int categ; bool catfree = false;)
      for(struct page_initializer const *page = bip_pages;
         page->category || page->pclass; page++)
   {
      if(page->category) {
         categ   = page->category;
         catfree = page->isfree;
      } else {
         if(page->pclass & p->pclass)
            AddToBIP(bip, categ, p->pclass, page, page->isfree || catfree);
         total++;
      }
   }

   if(world.dbgLevel) p->logH("> There are %i info pages!", total);

   ForCategory()
      bip->pagemax += bip->categorymax[categ] = bip->infogr[categ].size;

   if(world.dbgBIP)
   {
      bip->pageavail = bip->pagemax;

      ForCategory()        bip->categoryavail[categ] = bip->categorymax[categ];
      ForCategoryAndPage() page->unlocked = true;
   }

   bip->init = true;
}

//
// Lith_DeliverMail
//
[[__call("ScriptS")]]
void Lith_DeliverMail(player_t *p, __str title, int flags)
{
   if(p->pclass & pcl_mods) return;

   p->setActivator();

   flags |= strtoi_str(Language("LITH_TXT_MAIL_FLAG_%S", title), null, 0);

   ifauto(__str, discrim, p->discrim) {
      flags |= strtoi_str(Language("LITH_TXT_MAIL_FLAG_%S%S", title, discrim), null, 0);
      if(!(flags & MAILF_AllPlayers))
         title = StrParam("%S%S", title, discrim);
   }

   bip_t *bip = &p->bip;

   bippage_t *page = salloc(bippage_t);

   __str date = LanguageNull("LITH_TXT_MAIL_TIME_%S", title);
   __str size = LanguageNull("LITH_TXT_MAIL_SIZE_%S", title);
   __str send = LanguageNull("LITH_TXT_MAIL_SEND_%S", title);
   __str name = LanguageNull("LITH_TXT_MAIL_NAME_%S", title);
   __str body = Language    ("LITH_TXT_MAIL_BODY_%S", title);

   if(!send) send = "<internal>";

   page->name  = date ? date : world.canontimeshort;
   page->title = name ? name : "<title omitted>";
   page->body  = StrParam(Language("LITH_TXT_MAIL_TEMPLATE"), send, page->name, body);
   page->category = BIPC_MAIL;
   page->unlocked = true;

   if(size) page->height = strtoi_str(size, null, 0);

   page->link.construct(page);
   page->link.link(&bip->infogr[BIPC_MAIL]);

   bip->mailreceived++;

   if(!(flags & MAILF_NoPrint))
   {
      ACS_Delay(20);

      p->log("> Mail received from <\Cj%S\C->.", send);

      if(ACS_Random(1, 10000) == 1)
      {
         bip->mailtrulyreceived++;
         ACS_LocalAmbientSound("player/YOUVEGOTMAIL", 127);
      }
      else
         ACS_LocalAmbientSound("player/cbi/mail", 127);
   }
}

//
// Lith_FindBIPPage
//
bippage_t *Lith_FindBIPPage(bip_t *bip, __str name)
{
   if(!name)
      return null;

   ForCategoryAndPage()
      if(page->name == name)
         return page;

   return null;
}

//
// Lith_UnlockBIPPage
//
bippage_t *Lith_UnlockBIPPage(bip_t *bip, __str name, int pclass)
{
   bippage_t *page = bip->find(name);

   if(!page && pclass)
      ifauto(__str, discrim, Lith_PlayerDiscriminator(pclass))
   {
      page = bip->find(StrParam("%S%S", name, discrim));
   }

   if(page && !page->unlocked)
      UnlockPage(bip, page, pclass);

   return page;
}

//
// Lith_DeallocateBIP
//
[[__call("ScriptS")]]
void Lith_DeallocateBIP(bip_t *bip)
{
   ForCategory()
      bip->infogr[categ].free(free);
   bip->init = false;
}

//
// CheckMatch
//
[[__call("ScriptS")]]
static bool CheckMatch(struct page_info *pinf, __str query)
{
   return strcasestr_str(pinf->shname, query) ||
          strcasestr_str(pinf->flname, query) ||
          strcasestr_str(pinf->body,   query);
}

//
// Lith_CBITab_BIP
//
void Lith_CBITab_BIP(gui_state_t *g, player_t *p)
{
   bip_t *bip = &p->bip;
   int avail, max;

   if(bip->curcategory == BIPC_MAIN)
   {
      int n = 0;

      HudMessageF("CBIFONT", "\CTINFO CATEGORIES");
      HudMessagePlain(g->hid--, 40.1, 70.1, TICSECOND);

      bip->lastcategory = BIPC_MAIN;

      static __str const lines[] = {
         "Text search all categories.",
         "Weapons and weapon upgrades.",
         "Enemies and bosses.",
         "Your attributes, abilities and history.",
         "Body upgrades.",
         "Places of interest around the galaxy.",
         "Important companies, historic and current.",
         "Received mail."
      };

      for(int i = 0; i < countof(lines); i++)
      {
         HudMessage(lines[i]);
         HudMessageAlpha(g->hid--, 105.1, 85.1 + n + i * 10, TICSECOND, 0.7);
      }

      if(Lith_GUI_Button(g, "Search", 45, 85 + n, Pre(btnbipmain)))
         bip->curcategory = BIPC_SEARCH;
      n += 10;
#define LITH_X(name, capt) \
      if(Lith_GUI_Button_Id(g, BIPC_##name, capt, 45, 85 + n, Pre(btnbipmain))) \
      { \
         bip->curcategory = BIPC_##name; \
         bip->curpage     = null; \
      } \
      n += 10;
#include "lith_bip.h"

      avail = bip->pageavail;
      max   = bip->pagemax;
   }
   else if(bip->curcategory == BIPC_SEARCH)
   {
      gui_txtbox_state_t *st = Lith_GUI_TextBox(g, &CBIState(g)->bipsearch, 23, 65, p->num, p->txtbuf);
      p->clearTextBuf();

      bip->lastcategory = BIPC_MAIN;

      Lith_GUI_TextBox_OnTextEntered(st)
      {
         // That's a lot of numbers...
         crc64_t const extranames[] = {
            0x5F38B6C56F0A6D84L,
            0x90215131A36573D7L,
            0xC54EC0A7C6836A5BL,
            0xB315B81438717BA6L,
            0x9FD558A2C8C8D163L,
         };

         __str query = StrParam("%.*S", txt_len, txt_buf);
         crc64_t crc = Lith_CRC64_str(txt_buf, txt_len);

         bip->resnum = bip->rescur = 0;

         for(int i = 0; i < countof(extranames); i++)
         {
            if(crc == extranames[i])
            {
               list_t *link = bip->infogr[BIPC_EXTRA].next;
               for(int j = 0; j < i; j++)
                  link = link->next;
               bip->result[bip->resnum++] = link->object;
            }
         }

         ForCategoryAndPage()
         {
            if(bip->resnum >= countof(bip->result))
               break;

            if(!page->unlocked || page->category == BIPC_EXTRA)
               continue;

            struct page_info pinf = GetPageInfo(page);

            if(CheckMatch(&pinf, query))
               bip->result[bip->resnum++] = page;
         }

         if(bip->resnum == 0)
            ACS_LocalAmbientSound("player/cbi/findfail", 127);
      }

      if(bip->resnum)
      {
         for(int i = 0; i < bip->rescur; i++)
         {
            bippage_t *page = bip->result[i];
            struct page_info pinf = GetPageInfo(page);

            if(Lith_GUI_Button_Id(g, i, pinf.flname, 70, 95 + (i * 10), Pre(btnbipmain)))
            {
               bip->lastcategory = bip->curcategory;
               bip->curcategory = page->category;
               SetCurPage(g, bip, page, pinf.body);
            }
         }

         if((ACS_Timer() % ACS_Random(10, 20)) == 0 &&
            bip->rescur != bip->resnum)
         {
            if(++bip->rescur == bip->resnum)
               ACS_LocalAmbientSound("player/cbi/finddone", 127);
            else
               ACS_LocalAmbientSound("player/cbi/find", 127);
         }
      }
      else
      {
         HudMessageF("CBIFONT", "\CmNo results");
         HudMessagePlain(g->hid--, 70, 95, TICSECOND);
      }
   }
   else
   {
      list_t *list = &bip->infogr[bip->curcategory];
      size_t n = list->size;
      size_t i = 0;

      Lith_GUI_ScrollBegin(g, &CBIState(g)->bipscr, 15, 50, guipre.btnlist.w, 170, guipre.btnlist.h * n);

      if(bip->curcategory != BIPC_EXTRA)
         forlistIt(bippage_t *page, *list, i++)
      {
         int y = guipre.btnlist.h * i;

         if(Lith_GUI_ScrollOcclude(g, &CBIState(g)->bipscr, y, guipre.btnlist.h))
            continue;

         struct page_info pinf = GetPageInfo(page);
         __str name = StrParam("%S%S", bip->curpage == page ? "\Ci" : "", pinf.shname);

         if(Lith_GUI_Button_Id(g, i, name, 0, y, !page->unlocked || bip->curpage == page, Pre(btnlist)))
            SetCurPage(g, bip, page, pinf.body);
      }

      Lith_GUI_ScrollEnd(g, &CBIState(g)->bipscr);

      if(bip->curpage)
      {
         bippage_t *page = bip->curpage;
         struct page_info pinf = GetPageInfo(page);

         gui_typeon_state_t const *typeon = Lith_GUI_TypeOnUpdate(g, &CBIState(g)->biptypeon);

         int oy = 0;

         if(page->height)
         {
            Lith_GUI_ScrollBegin(g, &CBIState(g)->bipinfoscr, 100, 40, 200, 180, page->height, 184);
            oy = g->oy - 40;
         }
         else
            ACS_SetHudClipRect(111, 40, 200, 180, 184);

         if(page->image)
            DrawSpriteAlpha(page->image, g->hid--, 296.2, 180.2, TICSECOND, 0.4);

         DrawSpriteAlpha("lgfx/UI/Background.png", g->hid--, 0.1, 0.1, TICSECOND, 0.5);

         HudMessageF("CBIFONT", "\Ci%S", pinf.flname);
         HudMessagePlain(g->hid--, 200.4, 45.1 + oy, TICSECOND);

         #define DrawText(txt, pos, ...) \
            HudMessageF("CBIFONT", "%.*S%S", pos, txt, pos == typeon->len ? Ticker("\n|", "") : "|"), \
            HudMessageParams(0, g->hid--, __VA_ARGS__ + oy, TICSECOND)

         // render an outline if the page has an image
         if(page->image)
         {
            __str s = Lith_RemoveTextColors(typeon->txt, typeon->pos);
            int len = ACS_StrLen(s);

            DrawText(s, len, CR_BLACK, 112.1, 61.1); DrawText(s, len, CR_BLACK, 110.1, 61.1);
            DrawText(s, len, CR_BLACK, 112.1, 59.1); DrawText(s, len, CR_BLACK, 110.1, 59.1);

            DrawText(s, len, CR_BLACK, 111.1, 59.1);
            DrawText(s, len, CR_BLACK, 111.1, 61.1);

            DrawText(s, len, CR_BLACK, 112.1, 60.1);
            DrawText(s, len, CR_BLACK, 110.1, 60.1);
         }

         DrawText(typeon->txt, typeon->pos, CR_WHITE, 111.1, 60.1);

         if(page->height) Lith_GUI_ScrollEnd(g, &CBIState(g)->bipinfoscr);
         else             ACS_SetHudClipRect(0, 0, 0, 0);
      }

      avail = bip->categoryavail[bip->curcategory];
      max   = bip->categorymax[bip->curcategory];
   }

   if(bip->curcategory != BIPC_MAIN)
   {
      if(Lith_GUI_Button(g, "<BACK", 20, 38, false, Pre(btnbipback)))
         bip->curcategory = bip->lastcategory;
   }
   else
   {
      DrawSpriteAlpha("lgfx/UI/bip.png", g->hid--, 20.1, 40.1, TICSECOND, 0.6);
      HudMessageF("CBIFONT", "BIOTIC INFORMATION PANEL ver2.5");
      HudMessagePlain(g->hid--, 35.1, 40.1, TICSECOND);
   }

   if(max)
   {
      HudMessageF("CBIFONT", "%i/%i AVAILABLE", avail, max);
      HudMessagePlain(g->hid--, 300.2, 30.1, TICSECOND);
   }
}

// EOF

