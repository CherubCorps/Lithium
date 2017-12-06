#!/usr/bin/env ruby
## Copyright © 2017 Graham Sanderson
## DeCompat: ZScript ↔ DECORATE shared syntax preprocessor.

def writehead out
   out.write "// This file was generated by decompat.\n"
   out.write "// Edit only if you aren't going to recompile.\n\n"
end

def tozsc fp, out
   writehead out

   indefault = false
   instates  = false

   for ln in fp
      ln = ln.chomp.sub(/<Actor>/, "class")
                   .sub(/<Const>/, "const")
                   .sub(/<Var>/,   "")
                   .sub(/enum \/\/ /, "enum ")
      if ln.include? "<Default>"
         indefault = true
         out.write "   default\n   {\n"
      elsif ln.include? "<EndDefault>"
         indefault = false
         out.write "   }\n"
      elsif ln.include? "<States>"
         instates = true
         out.write "   states\n   {\n"
      elsif ln.include? "<EndStates>"
         instates = false
         out.write "   }\n"
      elsif ln.include? "<ZScript>" or ln.include? "<EndZScript>"
         next
      else
         if indefault && !ln.empty?
            out.write "   " + ln + ";\n"
         elsif instates && !ln.empty? && !ln.end_with?(":")
            out.write ln + ";\n"
         else
            out.write ln + "\n"
         end
      end
   end
end

def todec fp, out
   writehead out

   inzsc = false

   for ln in fp
      ln = ln.chomp.sub(/<Actor>/, "actor")
                   .sub(/<Const>/, "const int")
                   .sub(/<Var>/,   "var")

      if ln.include? "<ZScript>"
         inzsc = true
      elsif ln.include? "<EndZScript>"
         inzsc = false
      elsif not inzsc
         if ln.include? "<Default>" or ln.include? "<EndDefault>"
            next
         elsif ln.include? "<States>"
            out.write "   states\n   {\n"
         elsif ln.include? "<EndStates>"
            out.write "   }\n"
         else
            out.write ln + "\n"
         end
      end
   end
end

for arg in ARGV
   fp = open(arg, "rt")
   dec = fp.gets[15..-1].chomp
   zsc = fp.gets[15..-1].chomp
   fp.gets
   tozsc fp, open(zsc, "wt")
   fp.rewind
   3.times {fp.gets}
   todec fp, open(dec, "wt")
end

## EOF
