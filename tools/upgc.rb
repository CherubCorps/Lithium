#!/usr/bin/env -S ruby --enable=frozen-string-literal -w
## ---------------------------------------------------------------------------|
##
## Distributed under the CC0 public domain license.
## By Alison G. Watson. Attribution is encouraged, though not required.
## See licenses/cc0.txt for more information.
##
## ---------------------------------------------------------------------------|
##
## Upgrade info text compiler.
##
## ---------------------------------------------------------------------------|

require_relative "corinth.rb"
require "yaml"

INPUT = ARGV.shift
OUT_H = ARGV.shift
OUT_C = ARGV.shift
OUT_F = ARGV.shift

def zstr s
   s.gsub "\0", "\\\\0"
end

common_main do
   hsh      = YAML.load(IO.read(INPUT), symbolize_names: true)
   upgrades = []

   for category, cupgs in hsh
      catname = category.to_s
      for name, hupg in cupgs
         name = name.to_s
         reqs = (hupg[:req] || "").split
         for req in reqs
            req.prepend "dst_bit(UR_"
            req.concat  ")"
         end
         if reqs.empty?
            reqs = ["0"]
         end
         pg = hupg[:pg] || name
         if pg == "N/A"
            pg = "nil"
         else
            pg = '"' + pg + '"'
         end
         mul = hupg[:mul] || 0
         flg = "Case(" + name + ")\n"
         for c, i in (hupg[:f] || "-----").chars.each_with_index
            if c == "-"
               next
            elsif (c == "A" && i == 0) ||
                  (c == "D" && i == 1) ||
                  (c == "U" && i == 2) ||
                  (c == "R" && i == 3) ||
                  (c == "E" && i == 4)
               flg.concat "   #{c}(#{name})\n"
            else
               raise "functions failed to parse for #{name}"
            end
         end

         upg = {}
         upg[:nam] = name
         upg[:inf] = pg
         upg[:scr] = hupg[:scr] || 0
         upg[:pcl] = hupg[:cl]  || "gA"
         upg[:cat] = catname
         upg[:prf] = hupg[:pr]  || 0
         upg[:grp] = hupg[:grp] || 0
         upg[:req] = reqs.join "|"
         upg[:mul] = mul / 100.0
         upg[:flg] = flg
         upgrades.push upg
      end
   end

   open(OUT_H, "wt").puts <<_end_h_; open(OUT_C, "wt").puts <<_end_c_; open(OUT_F, "wt").puts <<_end_f_
#{generated_header "upgc"}

#if defined(upgrade_x)
#{
res = String.new
for upg in upgrades do res.concat "upgrade_x(#{upg[:nam]})\n" end
res
}
#undef upgrade_x
#elif !defined(upgc_header)
#define upgc_header

enum ZscName(UpgradeName) {
   #define upgrade_x(name) UPGR_##name,
   #include "#{File.basename OUT_H}"
   UPGR_MAX
};

#endif
_end_h_
#{generated_header "upgc"}

#include "u_common.h"

/* Extern Objects ---------------------------------------------------------- */

struct upgradeinfo upgrinfo[] = {
#{
res = String.new
for upg in upgrades do res.concat %(   {{"#{upg[:nam]}", #{upg[:inf]}, #{upg[:scr]}}, #{upg[:pcl]}, #{upg[:cat]}, #{upg[:prf]}, #{upg[:grp]}, #{upg[:req]}, #{upg[:mul]}, UPGR_#{upg[:nam]}},\n) end
res
}
};

/* EOF */
_end_c_
#{generated_header "upgc"}

#include "u_func_beg.h"

#{
res = String.new
for upg in upgrades do res.concat upg[:flg] end
res
}
#include "u_func_end.h"

/* EOF */
_end_f_
end

## EOF
