#!/usr/bin/env ruby
## ---------------------------------------------------------------------------|
##
## Distributed under the CC0 public domain license.
## By Alison Sanderson. Attribution is encouraged, though not required.
## See licenses/cc0.txt for more information.
##
## ---------------------------------------------------------------------------|
##
## Formatted text → LANGUAGE processor.
##
## ---------------------------------------------------------------------------|

def read_lines fname
   s = open(fname, "rt").read
   s.chomp!
   s.lines
end

def escape text
   text.gsub!(/((?<m>\\)(?!c))|(?<m>")/, "\\\\\\k<m>")
   text.gsub!(?\n, "\\n")
   text
end

def split_arg text, sp
   text.split(sp, 2).each{|s| s.strip!}
end

def single_line txt, set
   txt.replace %<"#{txt}" = "#{escape set}";\n>
end

def comment arg
   arg.replace "/*#{arg}*/\n"
end

def buf_lines type, buf
   return "" unless buf.last
   buf.pop if buf.last.chomp.empty?
   case type
   when :just
      t = ""
      buf.each.with_index do |s, i|
         s.chomp!
         if i < buf.size-1 then t << %<   "#{escape s}\\n"\n>
         else                   t << %<   "#{escape s}";\n> end
      end
      t << ?\n
      t
   when :conc
      buf = [*buf, ?\n].each_cons(2).map do |s, n|
            if s == ?\n then s.replace "\n\n"
         elsif n == ?\n then s.chomp!
         else                s.chomp!; s << ?\s end
         s
      end

      t = ""
      buf.each.with_index do |s, i|
         if i < buf.size-1 then           t << %<   "#{escape s}"\n>
         else                   s.chomp!; t << %<   "#{escape s}";\n> end
      end
      t << ?\n
      t
   end
end

def parse_file lnc, lns, pfx
   wr  = nil
   buf = nil
   out = ""
   for ln in lns
      ln = ln.gsub("\\#", pfx)
      if ln =~ /^##(.+)$/
         if wr then out << buf_lines(wr, buf); wr = nil end
         out << comment($~[1].chomp)
      elsif ln =~ /^==(.+)\|(.*)$/
         if wr then out << buf_lines(wr, buf); wr = nil end
         out << single_line($~[1].strip, $~[2].strip)
      elsif ln =~ /^%%(.+)$/
         if wr then out << buf_lines(wr, buf) end
         wr, buf = :just, []
         out << %<"#{$~[1].strip}" =\n>
      elsif ln =~ /^@@(.+)$/
         if wr then out << buf_lines(wr, buf) end
         wr, buf = :conc, []
         out << %<"#{$~[1].strip}" =\n>
      elsif ln =~ /^\+\+(.+)\|(.+)$/
         if wr then out << buf_lines(wr, buf); wr = nil end
         set = $~[2].strip
         out << %<"#{$~[1].strip}" =\n>
         out << comment(" #{set}")
         out << buf_lines(:just, read_lines("#{lnc}/#{set}"))
      else
         if wr then buf << ln
         elsif ln.chomp.strip == "" then out << ?\n end
      end
   end
   if wr then out << buf_lines(wr, buf) end
   out << ?\n
   out
end

def proc_file lnc, lng, lns, nam, pfx
   txt = <<_end_
/* ---------------------------------------------------------------------------|
 *
 * This file was generated by compilefs.
 * Edit only if you aren't going to recompile.
 *
 * ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison Sanderson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
 */

[#{lng}]

_end_

   for ln in lns
      ln = ln.gsub("\\#", pfx)
      if ln =~ /^\s*comment (.+)$/
         txt << comment(" #{$~[1].strip}")
      elsif ln =~ /^\s*put data (.+)->(.+)$/
         txt << single_line($~[2].strip, $~[1].strip)
      elsif ln =~ /^\s*put file (.+)->(.+)$/
         fnam = $~[1].strip
         txt << %<"#{$~[2].strip}" =\n>
         txt << comment(" #{fnam}")
         txt << buf_lines(:just, read_lines("#{lnc}/#{fnam}"))
      elsif ln =~ /^\s*parse file (.+)$/
         txt << parse_file(lnc, read_lines("#{lnc}/#{$~[1].strip}"), pfx)
      end
   end

   txt << comment(" EOF")

   IO.write nam, txt
end

def run_file lns
   lns = lns.each
   dir = ?.
   lng = []
   pfx = ""

   loop do
      ln = lns.next

      if ln =~ /^\s*language (.+) -> (.+)$/
         lng << {lnc: $~[1].strip, lng: $~[2].strip}
      elsif ln =~ /^\s*in directory (.+)$/
         dir.replace $~[1].strip
      elsif ln =~ /^\s*prefix (.+)$/
         pfx.replace $~[1].strip
      elsif ln =~ /^\s*in file (.+)$/
         arg = $~[1].strip
         txt = []

         while lns.peek.strip != "done"
            txt << lns.next
         end
         lns.next

         for l in lng
            nam = "#{dir}/#{arg.sub ?*, l[:lnc]}"
            proc_file l[:lnc], l[:lng], txt, nam, pfx
         end
      end
   end
end

for arg in ARGV
   run_file read_lines(arg)
end

## EOF
