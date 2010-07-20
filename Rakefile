# encoding: UTF-8
begin
  require 'jeweler'
  Jeweler::Tasks.new do |gem|
    gem.name = "rbktoblzcheck"
    gem.summary = "rbktoblzcheck is an interface for libktoblzcheck, a library to check German account numbers and
bank codes. See http://ktoblzcheck.sourceforge.net for details."
    gem.email = "kim.rudolph@web.de"
    gem.homepage = "http://github.com/krudolph/rbktoblzcheck"
    gem.authors = ["Sascha Loetz", "Kim Rudolph"]
    gem.require_paths = ["lib", "ext"]
    gem.extra_rdoc_files = `git ls-files *.rdoc`.split("\n")
    gem.files = `git ls-files`.split("\n")
    gem.extensions = ["ext/extconf.rb"]
    gem.files.include %w(lib/jeweler/templates/.document lib/jeweler/templates/.gitignore)
  end
rescue LoadError
  puts "Jeweler not available. Install it with: gem install jeweler"
end
