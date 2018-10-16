#!/usr/bin/env ruby
## Copyright © 2018 Alison Sanderson, all rights reserved.
## HashFS: Compile a directory structure into an enumerated list.

require 'fileutils'

of = open("pk7/language.gfx.txt", "w")
of.puts("// This file was generated by hashfs.
// Edit only if you aren't going to recompile.
[enu default]")

Dir.glob "pk7/lgfx/**/*.png" do |item|
   unless item.include? "/Font/"
      item.slice!(0, 4)
      of.puts "\"LITH:#{/lgfx\/(.+)\.png/.match(item)[1].gsub('/', ':')}\" = \"#{item}\";"
   end
end
