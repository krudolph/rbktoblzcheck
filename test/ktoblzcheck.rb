#!/usr/bin/env ruby
#
# Test program for rbktoblzcheck Ruby extension:
#
# Checks some bank codes with both initialisation modes
# for KtoBlzCheck.
#
# (c) 2004 Sascha Loetz (sloetz ( a t ) web.de)
#
# See LICENSE for the BSD-style license for this code.
#
# This file is part of the rbktoblzcheck package
# available at http://www.vdp-software.com/Rbktoblzcheck.html
#
# $Id: test-ktoblzcheck.rb,v 1.1 2004/10/28 19:10:19 vdpsoftware Exp $

require 'ktoblzcheck'

# file with testdata
$df='test-bankdata.txt'
# some bank codes to ck, all listed in the test data file
$find=[20040000,30020947,40050150,50012800,60020300,70010500,80053552,87096124]

# exit code
$ec=0

#
# Use block syntax
#
print "\nUsing block syntax...\n"
KtoBlzCheck.new($df) do |kbc|
  print "\nCurrently #{kbc.num_records} records available in the database\n"
  $find.each do |bc|
    res=kbc.find(bc.to_s)
    if nil==res[0]
      print "Couldn't find bank code #{bc}\n"
      $ec=1
    else
      print "Bank code #{bc} => bank name: #{res[0]} / location (city): #{res[1]}\n"
    end
  end # each
end # do

#
# Use "normal" syntax
#
# IMPORTANT: you must call #close when your finished,
#            otherwise resource leaks may occur.
#
print "\nUsing 'normal' syntax...\n"
$kbc=KtoBlzCheck.new($df)
print "\nCurrently #{$kbc.num_records} records available in the database\n"
$find.each do |bc|
    res=$kbc.find(bc.to_s)
    if nil==res[0]
      print "Couldn't find bank code #{bc}\n"
      $ec=1
    else
      print "Bank code: #{bc} => name: #{res[0]} / location (city): #{res[1]}\n"
    end
end # do

$kbc.close

exit $ec
