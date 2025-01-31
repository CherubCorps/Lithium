// ╭──────────────────────────────────────────────────────────────────────────╮
// │                                                                          │
// │             Distributed under the CC0 public domain license.             │
// │   By Alison G. Watson. Attribution is encouraged, though not required.   │
// │                See licenses/cc0.txt for more information.                │
// │                                                                          │
// ├──────────────────────────────────────────────────────────────────────────┤
// │                                                                          │
// │ Common type definitions and attribute macros.                            │
// │                                                                          │
// ╰──────────────────────────────────────────────────────────────────────────╯

#ifndef m_types_h
#define m_types_h

#pragma GDCC FIXED_LITERAL ON

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdfix.h>
#include <stdint.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))
#define swap(t, a, b) statement({ t _tmp = a; a = b; b = _tmp; })

#define nil  ((void *)0)
#define snil ((__str_ent *)0)
#define lnil ((__label *)0)

#define script       [[__call("ScriptI")]]
#define script_str   [[__call("ScriptS")]]
#define stkcall      [[__call("StkCall")]]
#define sync         [[__call("SScriptI")]]
#define sync_str     [[__call("SScriptS")]]
#define optargs(x)   [[__optional_args(x)]]
#define noinit       [[__no_init]]
#define anonymous    [[__anonymous]]
#define ext(x)       [[__extern(x)]]
#define alloc_aut(x) [[__alloc_Aut(x)]]
#define dynam_aut    [[__alloc_Aut(0x80000000)]]
#define type(x)      [[__script(x)]]
#define addr(x)      [[__address(x)]]

/* makes it easier to find function typedefs -zoe */
#define funcdef typedef

typedef int32_t i32;
typedef int64_t i64;
typedef int96_t i96;

typedef uint32_t u32;
typedef uint64_t u64;
typedef uint96_t u96;

#define SCR_MAX         INT64_MAX
#define FMT_SCR         "li"
#define faststrtoscr    faststrtoi64
#define lerpscr         lerpli
#define lerpscr_init    lerpli_init
#define interp_data_scr interp_data_li
#define scorediv_t      ldiv_t
#define minscr          minli
#define maxscr          maxli
#define clampscr        clampli

typedef i64 score_t;

typedef i32 mem_tag_t;
typedef i32 mem_size_t;
typedef i32 mem_byte_t;

typedef __label   *lbl;
typedef __str_ent *str;

typedef char __str_ars const *astr;
typedef char           const *cstr;

typedef      _Accum k32;
typedef long _Accum k64;

typedef float  f32;
typedef double f64;

struct polar {k32 ang, dst;};
struct i32v2 {i32 x, y;};
struct k32v2 {k32 x, y;};
struct k64v2 {k64 x, y;};
struct i32v3 {i32 x, y, z;};
struct k32v3 {k32 x, y, z;};
struct k64v3 {k64 x, y, z;};
struct ptr2  {mem_size_t l, h;};

union ik32 {i32 i; k32 k;};
union ik64 {i64 i; k64 k;};

__addrdef extern __mod_arr lmvar;

#endif
