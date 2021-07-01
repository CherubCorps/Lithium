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

require_relative "corinth.rb"

@cfg = {}
begin
   require_relative ".build-cfg.rb"
rescue LoadError
end

def hash s
   s.each_codepoint.reduce do |res, ch| (res * 101 + ch) & 0x7FFFFFFF end
end

def  bin(s) "$" +   BIN + "/" + s + ".ir" end
def  hdr(s) "$" +   HDR + "/" + s         end
def  src(s) "$" +   SRC + "/" + s         end
def  txt(s) "$" +   TXT + "/" + s         end
def  zsc(s) "$" +   ZSC + "/" + s         end
def tool(s) "$" + TOOLS + "/" + s         end

def each_fake_dep ctx, ents, name
   deps = []

   for ent in ents
      deps.push yield ent
   end

   ctx.fp << <<ninja
build #{name}: phony #{deps.join " "}
default #{name}
ninja
end

LIB_PATH = if s = @cfg[:lib_path] then "--lib-path='#{s}' " else "" end
ALL_ARG  = if s = @cfg[:all_arg]  then s + " " else "" end
MK_ARG   = if s = @cfg[:mk_arg]   then s + " " else "" end + ALL_ARG + LIB_PATH
CC_ARG   = if s = @cfg[:cc_arg]   then s + " " else "" end + ALL_ARG + LIB_PATH
LD_ARG   = if s = @cfg[:ld_arg]   then s + " " else "" end + ALL_ARG

BIN, DIR, HASH, HDR, SRC, TARGET, TOOLS, TXT, TYPE, ZSC = ("a".."z").entries

BUILD_FILES =
   %(#{tool ".build-cfg.rb" unless @cfg.empty?} #{tool "genbuild.rb"})

Context = Struct.new :fp, :srcs, :deps, :link

UpgcEnt = Struct.new :i, :c, :h, :f
UPGC = [
   UpgcEnt.new("Upgrades.yaml", "p_upgrinfo.c", "u_names.h", "u_func.h"),
]
def proc_upgc ctx
   ctx.fp << <<ninja
rule upgc
 command = #{tool "upgc.rb"} $in $out
 description = UpgC
ninja

   for ent in UPGC
      i = txt ent.i
      o = bin ent.c
      c = src ent.c
      h = hdr ent.h
      f = hdr ent.f

      ctx.fp << "build #{h} #{c} #{f}: upgc #{i} | #{tool "upgc.rb"}\n"

      ctx.srcs.push SrcsEnt.new o, c
      ctx.deps.push h, f
   end
end

WepcEnt = Struct.new :i, :c, :h
WEPC = [
   WepcEnt.new("Weapons.yaml", "p_weaponinfo.c", "p_weaponinfo.h"),
]
def proc_wepc ctx
   ctx.fp << <<ninja
rule wepc
 command = #{tool "wepc.rb"} $in $out
 description = WepC
ninja

   for ent in WEPC
      i = txt ent.i
      o = bin ent.c
      c = src ent.c
      h = hdr ent.h

      ctx.fp << "build #{h} #{c}: wepc #{i} | #{tool "wepc.rb"}\n"

      ctx.srcs.push SrcsEnt.new o, c
      ctx.deps.push h
   end
end

def proc_trie ctx
   ctx.fp << <<ninja
rule trie
 command = gdcc-cpp $in -o - | #{tool "trie.rb"} $out
 description = TRIE
build #{hdr "m_trie.h"} #{src "m_trie.c"}: trie #{txt "Trie.yaml"} | #{tool "trie.rb"}
ninja
end

ZcppEnt = Struct.new :i, :z
ZCPP = [
   ZcppEnt.new("items.h",        "items.zsc"),
   ZcppEnt.new("m_drawing.h",    "m_drawing.zsc"),
   ZcppEnt.new("p_player.h",     "p_player.zsc"),
   ZcppEnt.new("p_weaponinfo.h", "p_weaponinfo.zsc"),
   ZcppEnt.new("u_names.h",      "u_names.zsc"),
   ZcppEnt.new("w_monster.h",    "w_monster.zsc"),
   ZcppEnt.new("w_world.h",      "w_world.zsc"),
]
def proc_zcpp ctx
   ctx.fp << <<ninja
rule zcpp
 command = gdcc-cpp -DZscOn=1 -D'ZscName(x, ...)=Lith_##x' $in -o $out
 description = ZCPP
ninja

   each_fake_dep ctx, ZCPP, "zcpp_" do |ent|
      i = hdr ent.i
      z = zsc ent.z

      ctx.fp << "build #{z}: zcpp #{i}\n"

      z
   end
end

TxtcEnt = Struct.new :i
TXTC = [
   TxtcEnt.new("Text.txt"),
]
def proc_txtc ctx
   ctx.fp << <<ninja
rule txtc
 command = #{tool "txtc.rb"} $in
 description = TxtC
ninja

   each_fake_dep ctx, TXTC, "txtc_" do |ent|
      i = txt ent.i
      o = i + "_"
      ctx.fp << "build #{o}: txtc #{i} | #{tool "txtc.rb"}\n"
      o
   end
end

HsfsEnt = Struct.new :o, :p, :d
HSFS = [
   HsfsEnt.new(    "pk7/language.gfx.txt",     "pk7/", "lgfx"),
   HsfsEnt.new("pk7_ep1/language.gfx.txt", "pk7_ep1/", "lgfx"),
]
def proc_hsfs ctx
   ctx.fp << <<ninja
rule hsfs
 command = #{tool "hashfs.rb"} $out $in $#{DIR}
 description = HashFS
build _fake_: phony
ninja

   each_fake_dep ctx, HSFS, "hsfs_" do |ent|
      ctx.fp << <<ninja
build #{ent.o}: hsfs | _fake_ #{tool "hashfs.rb"}
 #{DIR} = #{ent.p} #{ent.d}
ninja

      ent.o
   end
end

SndcEnt = Struct.new :i
SNDC = [
   SndcEnt.new("Sounds.txt"),
]
def proc_sndc ctx
   ctx.fp << <<ninja
rule sndc
 command = #{tool "sndc.rb"} $in
 description = SndC
ninja

   each_fake_dep ctx, SNDC, "sndc_" do |ent|
      i = txt ent.i
      o = i + "_"
      ctx.fp << "build #{o}: sndc #{i} | #{tool "sndc.rb"}\n"
      o
   end
end

MdlcEnt = Struct.new :i
MDLC = [
   MdlcEnt.new("Models.txt"),
]
def proc_mdlc ctx
   ctx.fp << <<ninja
rule mdlc
 command = #{tool "mdlc.rb"} $in
 description = MdlC
ninja

   each_fake_dep ctx, MDLC, "mdlc_" do |ent|
      i = txt ent.i
      o = i + "_"
      ctx.fp << "build #{o}: mdlc #{i} | #{tool "mdlc.rb"}\n"
      o
   end
end

LibrEnt = Struct.new :l
LIBR = [
   LibrEnt.new("libc"),
]
def proc_libr ctx
   ctx.fp << <<ninja
rule makelib
 command = gdcc-makelib $#{TARGET} #{MK_ARG} -c $#{TYPE} -o $out
 description = MakeLib $out
ninja

   for ent in LIBR
      o = bin ent.l

      ctx.fp << <<ninja
build #{o}: makelib
 #{TYPE} = #{ent.l}
ninja

      ctx.link.push o
   end
end

SrcsEnt = Struct.new :o, :c
class SrcsEnt
   def self.from s
      new bin(s), src(s)
   end

   def hash
      self.o.hash * self.c.hash
   end
end
def proc_srcs ctx
   ctx.srcs.uniq!

   ctx.fp << <<ninja
rule cc
 command = gdcc-cc $#{TARGET} --warn-all --no-warn-parentheses -i$#{HDR} --alloc-Aut 8192 #{CC_ARG} -D'ZscName(x, ...)=__VA_ARGS__' -DFileHash=$#{HASH} -c $in -o $out
 description = CC $out
ninja

   deps = ctx.deps.join " "
   for ent in ctx.srcs
      ctx.fp << <<ninja
build #{ent.o}: cc #{ent.c} | #{deps}
 #{HASH} = #{hash ent.c}
ninja

      ctx.link.push ent.o
   end
end

def proc_link ctx
   ctx.link.uniq!

   ctx.fp << <<ninja
rule ld
 command = gdcc-ld $#{TARGET} --bc-opt #{LD_ARG} --alloc-min Sta "" 70000 --no-bc-zdacs-init-script-named --bc-zdacs-init-script-number 17000 $in -o $out --bc-zdacs-dump-ScriptI #{bin "lithmain_ld.txt"}
 description = LD $out
build pk7/acs/lithmain.bin | #{bin "lithmain_ld.txt"}: ld #{ctx.link.join " "}
default pk7/acs/lithmain.bin
ninja
end

ctx = Context.new

ctx.link = []

ctx.srcs = [
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
   SrcsEnt.from("m_cps.c"),
   SrcsEnt.from("m_drawing.c"),
   SrcsEnt.from("m_file.c"),
   SrcsEnt.from("m_math.c"),
   SrcsEnt.from("m_memory.c"),
   SrcsEnt.from("m_namegen.c"),
   SrcsEnt.from("m_str.c"),
   SrcsEnt.from("m_tokbuf.c"),
   SrcsEnt.from("m_token.c"),
   SrcsEnt.from("m_trie.c"),
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
   SrcsEnt.from("p_upgrinfo.c"),
   SrcsEnt.from("p_weaponinfo.c"),
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
]

ctx.deps = [
   hdr("common.h"),
   hdr("d_compile.h"),
   hdr("d_vm.h"),
   hdr("gui.h"),
   hdr("items.h"),
   hdr("m_base64.h"),
   hdr("m_char.h"),
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
   hdr("p_weaponinfo.h"),
   hdr("p_weapons.h"),
   hdr("u_common.h"),
   hdr("u_data.h"),
   hdr("u_func.h"),
   hdr("u_func_beg.h"),
   hdr("u_func_end.h"),
   hdr("u_names.h"),
   hdr("w_monster.h"),
   hdr("w_world.h"),
]

ctx.fp = open "build.ninja", "wb"

ctx.fp << <<ninja
#{HDR   } = source/include
#{BIN   } = bin
#{SRC   } = source
#{TOOLS } = tools
#{TARGET} = --target-engine=ZDoom --target-format=ACSE --func-minimum ScriptI 17100
#{TXT   } = text
#{ZSC   } = pk7/lzscript/Constants
rule font
 command = #{tool "mkfont.rb"}
 description = MkFont
build font_: font
rule genbuild
 command = #{tool "genbuild.rb"}
 generator = 1
 description = GenBuild
build build.ninja: genbuild | #{BUILD_FILES}
ninja

proc_upgc ctx
proc_wepc ctx
proc_trie ctx
proc_zcpp ctx
proc_txtc ctx
proc_hsfs ctx
proc_sndc ctx
proc_mdlc ctx
proc_libr ctx
proc_srcs ctx
proc_link ctx

## EOF
