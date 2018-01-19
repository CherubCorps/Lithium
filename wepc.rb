#!/usr/bin/env ruby
## Copyright © 2017 Graham Sanderson, all rights reserved.
## wepc: Weapon info text compiler.
## vim: columns=110

def outHeader fp, weps
   fp.puts <<-END
// zsc output: pk7/lzscript/Headers/lith_weapons.h

// This file was generaed by wepc.
// Edit only if you aren't going to recompile.

enum // Lith_WeaponNum
{
   weapon_min = 1,
   weapon_unknown = 0,

#{
   ret = ""
   for wep in weps
      ret += "   #{wep.nam},\n"
   end
   ret
}
   weapon_max_lith,
   weapon_nonlith_start = weapon_max_lith - 1,

#{
   ret = ""
   for cl in ["plut", "tnt", "doom2", "aliens", "jpcp", "btsx"]
      for wp in ["fist", "chainsaw", "pistol", "shotgun", "ssg", "chaingun", "launcher", "plasma", "bfg"]
         ret += "   weapon_fd_#{cl}_#{wp},\n"
      end
   end
   ret
}
   weapon_max
};

enum // Lith_WeaponName
{
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

enum // Lith_RifleMode
{
   rifle_firemode_auto,
   rifle_firemode_grenade,
   rifle_firemode_burst,
   rifle_firemode_max
};

// EOF
END
end

def outSource fp, weps, wepn
   fp.puts <<-END
// This file was generated by wepc.
// Edit only if you aren't going to recompile.
#include "lith_player.h"

// Extern Objects ------------------------------------------------------------|

#define Placeholder1 "MMMMHMHMMMHMMM"
#define Placeholder2 "YOUSONOFABITCH"
#define A(a) "Lith_" a "Ammo"
#define M(a) "Lith_" a "ShotsFired"
#define P(a) "weapons/" a "/pickup"
#define N(a) .classname = "Lith_" a, .name = a
#define F(...) .flags = __VA_ARGS__
#define FN(a) .classname = "FD" a, .name = a
#define FDClass(cname) \\
   {1, pcl_fdoomer, FN(cname "Fist"),           "YOUSONOFABITCH", AT_None}, \\
   {1, pcl_fdoomer, FN(cname "Chainsaw"),       P("cfist"),       AT_None}, \\
   {2, pcl_fdoomer, FN(cname "Pistol"),         P("pistol"),      AT_Ammo, "FD" cname "PistolAmmo"}, \\
   {3, pcl_fdoomer, FN(cname "Shotgun"),        P("shotgun"),     AT_Ammo, "FD" cname "Shells"}, \\
   {3, pcl_fdoomer, FN(cname "SuperShotgun"),   P("ssg"),         AT_Ammo, "FD" cname "Shells"}, \\
   {4, pcl_fdoomer, FN(cname "Chaingun"),       P("rifle"),       AT_Ammo, "FD" cname "Bullets"}, \\
   {5, pcl_fdoomer, FN(cname "RocketLauncher"), P("rocket"),      AT_Ammo, "FD" cname "Rocket"}, \\
   {6, pcl_fdoomer, FN(cname "PlasmaRifle"),    P("plasma"),      AT_Ammo, "FD" cname "Cell"}, \\
   {7, pcl_fdoomer, FN(cname "BFG9000"),        P("cannon"),      AT_Ammo, "FD" cname "BFGCharge"},
weaponinfo_t const weaponinfo[weapon_max] = {
   {0, pcl_any, null, "MMMMHMHMMMHMMM"},

#{
   ret = ""
   for wep in weps
      ret += "   {#{wep.slt}, #{wep.pcl}, #{wep.res.join ", "}},\n"
   end
   ret
}
   FDClass("Plut")
   FDClass("TNT")
   FDClass("Doom2")
   FDClass("Aliens")
   FDClass("JPCP")
   FDClass("BTSX")
};
#undef FDClass

// Extern Functions ----------------------------------------------------------|

int Lith_WeaponFromName(player_t *p, int name)
{
   switch(p->pclass)
   {
#{
   ret = ""
   for pcl, wpns in wepn
      wepnames = ["wepnam_fist", "wepnam_chainsaw", "wepnam_pistol", "wepnam_shotgun",
                  "wepnam_supershotgun", "wepnam_chaingun", "wepnam_rocketlauncher",
                  "wepnam_plasmarifle", "wepnam_bfg9000"]
      ret += "   case #{pcl}:\n      switch(name) {\n"
      for wep, i in wpns.each_with_index
         ret += "      case #{wepnames[i]}: return #{wep};\n"
      end
      ret += "      }\n"
   end
   ret
}
   case pcl_fdoomer:
      #define FDClass(cname, ctype) \\
         if(p->pcstr == "FD" cname "Player") \\
            switch(name) { \\
            case wepnam_fist:           return weapon_fd_##ctype##_fist; \\
            case wepnam_chainsaw:       return weapon_fd_##ctype##_chainsaw; \\
            case wepnam_pistol:         return weapon_fd_##ctype##_pistol; \\
            case wepnam_shotgun:        return weapon_fd_##ctype##_shotgun; \\
            case wepnam_supershotgun:   return weapon_fd_##ctype##_ssg; \\
            case wepnam_chaingun:       return weapon_fd_##ctype##_chaingun; \\
            case wepnam_rocketlauncher: return weapon_fd_##ctype##_launcher; \\
            case wepnam_plasmarifle:    return weapon_fd_##ctype##_plasma; \\
            case wepnam_bfg9000:        return weapon_fd_##ctype##_bfg; \\
            }
      FDClass("Plut",   plut)
      FDClass("TNT",    tnt)
      FDClass("Doom2",  doom2)
      FDClass("Aliens", aliens)
      FDClass("JPCP",   jpcp)
      FDClass("BTSX",   btsx)
      #undef FDClass
   }
}

// EOF
END
end

class LithWep
   attr_reader :pcl
   attr_reader :nam
   attr_reader :slt
   attr_reader :res

   def initialize pcl, wf, spl
      @pcl = pcl
      @nam = "weapon_#{spl.shift}"
      @slt = spl.shift
      @res = [
         "N(\"#{spl.shift}\")",
         spl.shift,
         "AT_#{spl.shift}",
         *spl
      ]
      @res.push "F(#{wf})" unless wf.empty?
   end
end

def procFile ifp, ofh, ofc
   pcl  = "pcl_any"
   wf   = ""
   weps = []
   wepn = {}

   for ln in ifp
      ln.chomp!
      next if ln.empty?

      pre = ln[0]
      res = ln[1..-1]
      case pre
      when ':' then pcl = "pcl_" + res
      when '%' then wf = res
      when '{' then weps.push LithWep.new(pcl, wf, res.split)
      when ';' then wepn[pcl] = res.split.map {|s| "weapon_#{s}"}
      end
   end

   outHeader ofh, weps
   outSource ofc, weps, wepn
end

for arg in ARGV
   ifn, ofh, ofc = arg.split(',')
   procFile open(ifn, "rt"), open(ofh, "wt"), open(ofc, "wt")
end

## EOF
