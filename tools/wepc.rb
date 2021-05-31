#!/usr/bin/env ruby
# frozen_string_literal: true
## ---------------------------------------------------------------------------|
##
## Distributed under the CC0 public domain license.
## By Alison G. Watson. Attribution is encouraged, though not required.
## See licenses/cc0.txt for more information.
##
## ---------------------------------------------------------------------------|
##
## Weapon info text compiler.
##
## ---------------------------------------------------------------------------|

require_relative "corinth.rb"

common_main do
   WEPNAMES = %W"wepnam_fist wepnam_chainsaw wepnam_pistol wepnam_shotgun
                 wepnam_supershotgun wepnam_chaingun wepnam_rocketlauncher
                 wepnam_plasmarifle wepnam_bfg9000"

   tks = tokenize ARGV.shift

   pcl  = "pcl_any"
   weps = []
   wepn = {}

   loop do
      tok = tks.next.expect_in_top [:colon, :modulo, :plus, :semico, :eof]

      case tok.type
      when :colon
         tok = tks.next.expect_after tok, :identi
         pcl = "pcl_" + tok.text
      when :plus
         res = []
         tok = tks.next.expect_after tok, :identi
         nam = tok.text
         tok = tks.next.expect_after tok, :number
         slt = tok.text
         tok = tks.next.expect_after tok, :identi
         res.push 'N("' + tok.text + '")'
         tok = tks.next.expect_after tok, [:identi, :string]
         if tok.type == :identi
            res.push tok.text
         else
            res.push 'P("' + tok.text + '")'
         end
         tok = tks.next.expect_after tok, :identi
         res.push "AT_" + tok.text
         tks.peek_or :identi do |orig|
            typ = orig.text
            tok = tks.next.expect_after orig, :string
            res.push typ + '("' + tok.text + '")'
         end
         weps.push({pcl: pcl, nam: nam, slt: slt, res: res})
      when :semico
         res = []
         while tks.peek.type == :identi
            tok = tks.next
            res.push "weapon_" + tok.text
         end
         wepn[pcl] = res
      when :eof
         break
      end
   end

   open(ARGV.shift, "wt").puts <<_end_h_; open(ARGV.shift, "wt").puts <<_end_c_
#{generated_header "wepc"}

#ifndef wepc_header
#define wepc_header

enum ZscName(WeaponNum) {
   weapon_min = 1,
   weapon_unknown = 0,

#{
res = String.new
weps.each do |wep| res.concat "   weapon_#{wep[:nam]},\n" end
res
}
   weapon_max
};

enum ZscName(WeaponName) {
   wepnam_fist,
   wepnam_chainsaw,
   wepnam_pistol,
   wepnam_shotgun,
   wepnam_supershotgun,
   wepnam_chaingun,
   wepnam_rocketlauncher,
   wepnam_plasmarifle,
   wepnam_bfg9000,

   wepnam_max,
};

enum ZscName(RifleMode) {
   rifle_firemode_auto,
   rifle_firemode_grenade,
   rifle_firemode_burst,
   rifle_firemode_max
};

#endif
_end_h_
#{generated_header "wepc"}

#include "common.h"
#include "p_player.h"

/* Extern Objects ---------------------------------------------------------- */

#define Placeholder1 s"MMMMHMHMMMHMMM"
#define Placeholder2 s"YOUSONOFABITCH"
#define A(a) sOBJ a "Ammo"
#define O(a) sOBJ a
#define P(a) s"weapons/" a "/pickup"
#define N(a) .classname = sOBJ a, .name = Spf a
struct weaponinfo const weaponinfo[weapon_max] = {
   {0, pcl_any, snil, Placeholder1},
#{
res = String.new
weps.each do |wep|
   res.concat "   {#{wep[:slt]}, #{wep[:pcl]}, #{wep[:res].join ","}},\n"
end
res
}
};

/* Extern Functions -------------------------------------------------------- */

cstr P_Wep_GetPickup(i32 n) {
   switch(n) {
#{
res = String.new
for wep in weps
   res.concat "   case weapon_#{wep[:nam]}: return \"#{wep[:nam]}\";\n"
end
res
}
   }
   return "unknown";
}

i32 P_Wep_FromName(i32 name) {
   switch(pl.pclass) {
#{
res = String.new
for pcl, wpns in wepn
   res.concat "   case #{pcl}:\n      switch(name) {\n"
   wpns.each_with_index do |wep, i|
      res.concat "      case #{WEPNAMES[i]}: return #{wep};\n"
   end
   res.concat "      }\n"
end
res
}
   }

   return weapon_unknown;
}

/* EOF */
_end_c_
end

## EOF
