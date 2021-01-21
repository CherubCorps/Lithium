#!/usr/bin/env ruby
# frozen_string_literal: true
LICENSE = <<_end_
 * ---------------------------------------------------------------------------|
 *
 * Distributed under the CC0 public domain license.
 * By Alison G. Watson. Attribution is encouraged, though not required.
 * See licenses/cc0.txt for more information.
 *
 * ---------------------------------------------------------------------------|
_end_
##
## Utility libraries for tools.
##
## ---------------------------------------------------------------------------|

require_relative "corinth/io.rb"
require_relative "corinth/token.rb"
require_relative "corinth/tokenstream.rb"
require_relative "corinth/parsestate.rb"

def common_main
   begin
      yield
   rescue => exc
      puts "Error:\n#{exc.backtrace.join "\n"}\n#{exc.message}"
      exit false
   end
end

def generated_header progname, terminate = true
   <<_end_
/* ---------------------------------------------------------------------------|
 *
 * This file was generated by #{progname}.
 * Edit only if you aren't going to recompile.
 *
#{LICENSE}#{if terminate then " */" else "" end}
_end_
end

## EOF
