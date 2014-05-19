#
# ride on the mruby build system
#

ENV['HOW_MRUBY_DIR'] or fail
ENV['HOW_CONFIG'] or fail

# # -- build_config.rb:1
# ENV['MGEMFILE'] = "#{HOW_ROOT}/MGemfile

#
# mruby/Rakefile load
#
File.exists?("#{ENV['HOW_MRUBY_DIR']}/Rakefile") or fail # in case of non default `HOW_MRUBY_DIR`

# predefine build directory
ENV['MRUBY_BUILD_DIR'] = ENV['HOW_BUILD_DIR'] # mruby -- tasks/mruby_build.rake:66

# preset configuration file path
ENV['MRUBY_CONFIG'] = ENV['HOW_CONFIG'] # mruby -- Rakefile:13

if ENV['MGEMFILE']
  file "#{ENV['HOW_BUILD_DIR']}/host/mrbgems/gem_init.c" => ENV['MGEMFILE']
end

class String
  prepend Module.new {
    def relative_path_from(dir)
      super
    rescue ArgumentError => e
      raise e unless e.to_s.start_with? 'different prefix:'
      File.expand_path(self)
    end
  }
end

load "#{ENV['HOW_MRUBY_DIR']}/Rakefile"


task :build => :all

