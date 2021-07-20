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
## Generates a Ninja build file for the project.
##
## ---------------------------------------------------------------------------|

require "ostruct"

require_relative "corinth.rb"

BIN, DIR, HASH, HDR, SRC, TOOLS, TXT, TYPE, ZSC = ("a".."z").entries

INC_PATH = "--include $#{HDR} "
TARGET   = "--target-engine=ZDoom " +
           "--target-format=ACSE " +
           "--func-minimum ScriptI 17100 "

class SrcsEnt
   attr_reader :o, :c

   def initialize o, c
      @o = o
      @c = c
   end

   def self.from s
      new bin(s), src(s)
   end

   def hash
      (hash_impl(self.o) * hash_impl(self.c)) & 0x7FFFFFFF
   end
end

def  bin(s) "$" +   BIN + "/" + s + ".ir" end
def  hdr(s) "$" +   HDR + "/" + s         end
def  src(s) "$" +   SRC + "/" + s         end
def  txt(s) "$" +   TXT + "/" + s         end
def  zsc(s) "$" +   ZSC + "/" + s         end
def tool(s) "$" + TOOLS + "/" + s         end

def check_uniq name, ary
   res = ary.select do |e| ary.count(e) > 1 end
   if res.length > 0
      raise "#{name} array has duplicate entries! duplicates: #{res}"
   end
end

def hash_impl s
   s.each_codepoint.reduce do |res, ch| (res * 101 + ch) & 0x7FFFFFFF end
end

def req_file fname
   begin
      @ctx.rgen.push tool fname
      require_relative fname
   rescue LoadError => exc
      if block_given?
         yield exc
      else
         raise exc
      end
   end
end

@ctx = OpenStruct.new({
   link: [],
   srcs: [
      SrcsEnt.from("d_compile.c"),
      SrcsEnt.from("d_cutil.c"),
      SrcsEnt.from("d_stmt.c"),
      SrcsEnt.from("d_stmt_asm.c"),
      SrcsEnt.from("d_vm.c"),
      SrcsEnt.from("debug.c"),
      SrcsEnt.from("g_auto.c"),
      SrcsEnt.from("g_button.c"),
      SrcsEnt.from("g_checkbox.c"),
      SrcsEnt.from("g_preset.c"),
      SrcsEnt.from("g_scrollbar.c"),
      SrcsEnt.from("g_slider.c"),
      SrcsEnt.from("g_textbox.c"),
      SrcsEnt.from("g_window.c"),
      SrcsEnt.from("m_base64.c"),
      SrcsEnt.from("m_char.c"),
      SrcsEnt.from("m_cheat.c"),
      SrcsEnt.from("m_cps.c"),
      SrcsEnt.from("m_drawing.c"),
      SrcsEnt.from("m_file.c"),
      SrcsEnt.from("m_math.c"),
      SrcsEnt.from("m_memory.c"),
      SrcsEnt.from("m_namegen.c"),
      SrcsEnt.from("m_str.c"),
      SrcsEnt.from("m_tokbuf.c"),
      SrcsEnt.from("m_token.c"),
      SrcsEnt.from("p_attrib.c"),
      SrcsEnt.from("p_bip.c"),
      SrcsEnt.from("p_cbi.c"),
      SrcsEnt.from("p_data.c"),
      SrcsEnt.from("p_gui_bip.c"),
      SrcsEnt.from("p_gui_cbi.c"),
      SrcsEnt.from("p_gui_upgrades.c"),
      SrcsEnt.from("p_hud.c"),
      SrcsEnt.from("p_items.c"),
      SrcsEnt.from("p_log.c"),
      SrcsEnt.from("p_m_dbg.c"),
      SrcsEnt.from("p_magic.c"),
      SrcsEnt.from("p_notes.c"),
      SrcsEnt.from("p_obituary.c"),
      SrcsEnt.from("p_payout.c"),
      SrcsEnt.from("p_pickups.c"),
      SrcsEnt.from("p_player.c"),
      SrcsEnt.from("p_powerup.c"),
      SrcsEnt.from("p_ren_debug.c"),
      SrcsEnt.from("p_ren_magic.c"),
      SrcsEnt.from("p_ren_scope.c"),
      SrcsEnt.from("p_ren_step.c"),
      SrcsEnt.from("p_ren_view.c"),
      SrcsEnt.from("p_render.c"),
      SrcsEnt.from("p_save.c"),
      SrcsEnt.from("p_savedata.c"),
      SrcsEnt.from("p_settings.c"),
      SrcsEnt.from("p_shop.c"),
      SrcsEnt.from("p_shopdef.c"),
      SrcsEnt.from("p_statistics.c"),
      SrcsEnt.from("p_upgrades.c"),
      SrcsEnt.from("p_weapons.c"),
      SrcsEnt.from("u_adrenaline.c"),
      SrcsEnt.from("u_autoreload.c"),
      SrcsEnt.from("u_cannon_a.c"),
      SrcsEnt.from("u_cyberlegs.c"),
      SrcsEnt.from("u_defensenuke.c"),
      SrcsEnt.from("u_flashlight.c"),
      SrcsEnt.from("u_goldeneye.c"),
      SrcsEnt.from("u_headsupdisa.c"),
      SrcsEnt.from("u_headsupdisc.c"),
      SrcsEnt.from("u_headsupdisd.c"),
      SrcsEnt.from("u_headsupdisi.c"),
      SrcsEnt.from("u_headsupdism.c"),
      SrcsEnt.from("u_headsupdist.c"),
      SrcsEnt.from("u_headsupdisw.c"),
      SrcsEnt.from("u_instadeath.c"),
      SrcsEnt.from("u_jetbooster.c"),
      SrcsEnt.from("u_lolsords.c"),
      SrcsEnt.from("u_magic.c"),
      SrcsEnt.from("u_reactarmor.c"),
      SrcsEnt.from("u_reflexwetw.c"),
      SrcsEnt.from("u_rifle_a.c"),
      SrcsEnt.from("u_rpg_b.c"),
      SrcsEnt.from("u_seven7s.c"),
      SrcsEnt.from("u_stealthsys.c"),
      SrcsEnt.from("u_subweapons.c"),
      SrcsEnt.from("u_unceunce.c"),
      SrcsEnt.from("u_vitalscan.c"),
      SrcsEnt.from("u_zoom.c"),
      SrcsEnt.from("w_boss.c"),
      SrcsEnt.from("w_cbi.c"),
      SrcsEnt.from("w_cyberspace.c"),
      SrcsEnt.from("w_data.c"),
      SrcsEnt.from("w_dmon.c"),
      SrcsEnt.from("w_finale.c"),
      SrcsEnt.from("w_misc.c"),
      SrcsEnt.from("w_monster.c"),
      SrcsEnt.from("w_payout.c"),
      SrcsEnt.from("w_rain.c"),
      SrcsEnt.from("w_spawn.c"),
      SrcsEnt.from("w_time.c"),
      SrcsEnt.from("w_title.c"),
      SrcsEnt.from("w_world.c"),
   ],
   deps: [
      hdr("common.h"),
      hdr("d_compile.h"),
      hdr("d_vm.h"),
      hdr("gui.h"),
      hdr("items.h"),
      hdr("m_base64.h"),
      hdr("m_char.h"),
      hdr("m_cheat.h"),
      hdr("m_cps.h"),
      hdr("m_drawing.h"),
      hdr("m_file.h"),
      hdr("m_list.h"),
      hdr("m_math.h"),
      hdr("m_memory.h"),
      hdr("m_stab.h"),
      hdr("m_str.h"),
      hdr("m_tokbuf.h"),
      hdr("m_token.h"),
      hdr("m_types.h"),
      hdr("m_vec.h"),
      hdr("m_version.h"),
      hdr("p_attrib.h"),
      hdr("p_bip.h"),
      hdr("p_cbi.h"),
      hdr("p_hud.h"),
      hdr("p_hudid.h"),
      hdr("p_log.h"),
      hdr("p_player.h"),
      hdr("p_savedata.h"),
      hdr("p_shopdef.h"),
      hdr("p_upgrades.h"),
      hdr("p_weapons.h"),
      hdr("u_common.h"),
      hdr("u_data.h"),
      hdr("u_func_beg.h"),
      hdr("u_func_end.h"),
      hdr("w_monster.h"),
      hdr("w_world.h"),
   ],
   fake: [],
   ordr: [],
   rgen: [tool("genbuild.rb")],
   fp: open("build.ninja", "wb"),
})

@cfg = {}
req_file(".build-cfg.rb") do end

LIB_PATH = if s = @cfg[:lib_path] then "--lib-path='#{s}' " else "" end
ALL_ARG  = if s = @cfg[:all_arg]  then s + " " else "" end
MK_ARG   = if s = @cfg[:mk_arg]   then s + " " else "" end + ALL_ARG + LIB_PATH
CC_ARG   = if s = @cfg[:cc_arg]   then s + " " else "" end + ALL_ARG + LIB_PATH
CPP_ARG  = if s = @cfg[:cpp_arg]  then s + " " else "" end + ALL_ARG + INC_PATH
LD_ARG   = if s = @cfg[:ld_arg]   then s + " " else "" end + ALL_ARG

@ctx.fp << <<ninja
#{HDR  } = source/include
#{BIN  } = bin
#{SRC  } = source
#{TOOLS} = tools
#{TXT  } = text
#{ZSC  } = pk7/lzscript/Constants
ninja

# things that generate files to be built or linked
req_file "build/libr.rb"
req_file "build/trie.rb"
req_file "build/upgc.rb"
req_file "build/wepc.rb"

# things that generate stuff outside of the build system
req_file "build/hsfs.rb"
req_file "build/mdlc.rb"
req_file "build/sndc.rb"
req_file "build/txtc.rb"
req_file "build/zcpp.rb"

# the actual build process
req_file "build/fake.rb"
req_file "build/srcs.rb"
req_file "build/link.rb"

# rebuild - must be last
req_file "build/genb.rb"

## EOF
