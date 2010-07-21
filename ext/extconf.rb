require 'mkmf'
if !have_header('ktoblzcheck.h')
  STDERR.puts "Couldn't find header file ktoblzcheck.h. Install library files and try again\n"
  STDERR.puts "Hint: visit http://ktoblzcheck.sourceforge.net to obtain a source distribution tar-ball or check the package archives of your distribution (e.g. for Debian `apt-get install libktoblzcheck1-dev`)"
  exit 1
end
if !have_library('ktoblzcheck')
  STDERR.puts "Couldn't find link library libktoblzcheck. Install library files and try again\n"
  STDERR.puts "Hint: visit http://ktoblzcheck.sourceforge.net to obtain a source distribution tar-ball or check the package archives of your distribution (e.g. for Debian `apt-get install libktoblzcheck1-dev`)"
  exit 1
end
dir_config('ktoblzcheck')
create_makefile('ktoblzcheck')
