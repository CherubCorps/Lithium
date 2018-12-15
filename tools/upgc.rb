#!/usr/bin/env ruby
## Copyright © 2018 Alison Sanderson, all rights reserved.
## UpgC: Upgrade info text compiler.

ifp, ofh, ofc, off = ARGV[0], ARGV[1], ARGV[2], ARGV[3]
ifp, ofh, ofc, off = open(ifp, "rt"), open(ofh, "wt"), open(ofc, "wt"), open(off, "wt")

ofh.puts <<-END
// zsc output: pk7/lzscript/Headers/lith_upgradenames.h

// This file was generated by upgc.
// Edit only if you aren't going to recompile.

enum // UpgradeName
{
END

ofc.puts <<-END
// This file was generated by wepc.
// Edit only if you aren't going to recompile.
#include "lith_upgrades_common.h"

StrEntON

// Extern Objects ------------------------------------------------------------|

upgradeinfo_t const upgrinfobase[UPGR_BASE_MAX] = {
END

off.puts <<-END
// This file was generated by upgc.
// Edit only if you aren't going to recompile.
#include "lith_upgradefunc_start.h"

END

cat = nil

for ln in ifp
   ln.chomp!
   next if ln.empty?

   pre = ln[0]
   res = ln[1..-1]
   case pre
   when ':' then cat = "UC_" + res
   when '+' then
      res = res.split

      nam = res.shift
      inf = res.shift
      inf = if inf == "-" then "null"
            else               "\"#{inf}\"" end

      scr = if /[0-9]/ =~ res[0][0] then res.shift
            else                         "0" end

      pcl = res.shift
      flg = []

      for c, i in res.shift.chars.each_with_index
            if c == "-"           then next
         elsif c == "A" && i == 0 then flg << c
         elsif c == "D" && i == 1 then flg << c
         elsif c == "U" && i == 2 then flg << c
         elsif c == "R" && i == 3 then flg << c
         elsif c == "E" && i == 4 then flg << c
         else raise 'you done fucked up' end
      end

      prf = if res[0] && /[0-9]/ =~ res[0][0] then res.shift
            else                                   "0" end
      mul = if res[0] && /-?\./  =~ res[0]    then res.shift
            else                                   "0" end
      grp = if res[0] && /[A-Z]/ =~ res[0][0] then "UG_#{res.shift}"
            else                                   "0" end
      req = if res[0] && "("     == res[0][0] then res.shift[1...-1].split('|').collect{|a| "UR_#{a}"}.join("|")
            else                                   "0" end

      ofh.puts "   UPGR_#{nam},"

      ofc.puts <<-END
   {{"#{nam}", #{inf}, #{scr}}, #{pcl}, #{cat}, #{prf}, #{grp}, #{req}, #{mul}, UPGR_#{nam}},
END

      unless flg.empty?
         off.puts <<-END
Case(#{nam})
   #{flg.map{|a| "#{a}(#{nam})\n"}.join("   ")}
END
      end
   end
end

ofh.puts <<-END
   UPGR_BASE_MAX
};

// EOF
END

ofc.puts <<-END
};

// EOF
END

off.puts <<-END
#include "lith_upgradefunc_end.h"

// EOF
END

## EOF
