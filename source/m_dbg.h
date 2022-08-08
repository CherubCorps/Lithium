// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Debugging functions.                                                     │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#ifndef NDEBUG
#define Dbg_Trace(n) (ACS_BeginPrint(), ACS_PrintChar('\C'), ACS_PrintChar(ACS_Timer() & 3 ? 'i' : 'g'), PrintChrLi(_f), ACS_PrintChar(':'), ACS_PrintChar(' '), _p(n), EndLogEx(_pri_bold|_pri_nolog))

#define Dbg_Log(level, ...) \
   (dbglevel(level) ? \
    (ACS_BeginPrint(), \
     PrintChrLi(#level), \
     ACS_PrintChar(':'), \
     ACS_PrintChar(' '), \
     (__VA_ARGS__), \
     EndLogEx(_pri_critical|_pri_nonotify)) : \
    (void)0)
#define PrintErrLevel _pri_bold
#else
#define Dbg_Stat(...)
#define Dbg_Note(...)
#define Dbg_Log(...)
#define PrintErrLevel (_pri_bold | _pri_nonotify)
#endif

#ifndef NDEBUG
enum {
   log_dev,   /* general debug info */
   log_devh,  /* prints live stats to the HUD (position, angles, etc) */
   log_boss,  /* debug info for the boss system */
   log_dmon,  /* debug info for the monster tracker */
   log_dmonV, /* verbose debug info for the monster tracker */
   log_dlg,   /* debug info for the dialogue/terminal compiler */
   log_bip,   /* debug info for the BIP */
   log_dpl,   /* dynamic stack usage */
   log_save,  /* save data */
};

enum {
   dbgf_ammo,
   dbgf_bip,
   dbgf_gui,
   dbgf_items,
   dbgf_score,
   dbgf_upgr,
};
#endif

#ifndef NDEBUG
void Dbg_PrintMem(void const *data, mem_size_t size);
#endif

#ifndef NDEBUG
#define dbglevel(level) get_bit(cv.debug_level, level)
#define dbgflags(flags) get_bit(cv.debug_flags, flags)
#define dbglevel_any()  !!cv.debug_level
#define dbgflags_any()  !!cv.debug_flags
#else
#define dbglevel(level) false
#define dbgflags(flags) false
#define dbglevel_any()  false
#define dbgflags_any()  false
#endif
