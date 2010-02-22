#!/usr/bin/env ruby
#
# Test program for rbktoblzcheck Ruby extension:
#
# Reads all bank codes from an ktoblzcheck data file
# and checks them against the same file using the Ruby
# bindings for libktoblzecheck.
#
# Path to data file must be given as command line parameter.
#
# (c) 2004 Sascha Loetz (sloetz ( a t ) web.de)
#
# See LICENSE for the BSD-style license for this code.
#
# This file is part of the rbktoblzcheck package
# available at http://www.vdp-software.com/Rbktoblzcheck.html
#
# $Id: test-ktoblzcheck-all.rb,v 1.1 2004/10/28 19:10:19 vdpsoftware Exp $


require 'ktoblzcheck'

$failed=0
$ok=0

if ARGV[0].nil?
  STDERR.puts "Usage: #{File.basename($0)} /path/to/bankdata.file"
  exit(2)
end

KtoBlzCheck.new(ARGV[0]) do |kbc|
  File.open(ARGV[0],"r") do |df|
    while line=df.gets
      blz,algo,name,location=line.chomp.split("\t");
      res=kbc.find(blz.to_s)
      print "File: #{blz} / #{name} / #{location} => KtoBlzCheck name: #{res[0]} location: #{res[1]} ==> "
      if res[0]==name and res[1]==location
        print "OK\n"
        $ok+=1
      else
        print "FAILED\n"
        $failed+=1
      end
    end # while
  end # File.open
end # new

print "\n\nResult:\nLines checked: #{$ok+$failed}\nOK: #{$ok}\nFailure: #{$failed}\n"

exit(1) if $failed>0
