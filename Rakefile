HOW_ROOT = __dir__

ENV['MGEM_PATH'] ||= "#{HOW_ROOT}/mrbgems"

ENV['HOW_TOOLS_DIR'] ||= "#{HOW_ROOT}/tools"
ENV['HOW_MRUBY_DIR'] ||= "#{HOW_ROOT}/mruby"
ENV['HOW_BUILD_DIR'] ||= "#{HOW_ROOT}/build"
ENV['HOW_CONFIG'] ||= "#{HOW_ROOT}/build_config.rb"

ENV['TOOLCHAIN'] = 'gcc'

task :default => :install

desc 'build this application, then install it to `install_dir` if `install_dir` was given'
task :install do
  if ENV['install_dir']
    mkdir_p ENV['install_dir']
    cp Dir["#{ENV['HOW_BUILD_DIR']}/host/bin/hooring.*"], ENV['install_dir']
  end
end

load "#{HOW_ROOT}/tasks/setup.rake"
task :install => :setup

load "#{HOW_ROOT}/tasks/hooring_build.rake"
task :install => :build

desc 'update sources' # for non git users' convenience
task :update => :git do
  cd HOW_ROOT do
    `git fetch origin master`
    `git checkout origin/master`
    `git submodule init`
    `git submodule update`
  end
end
