load "#{__dir__}/setup_helpers.rake"

directory tools_dir

directory cache_dir

#
# setup git
#
unless find_exe('git.exe')
  ENV['GIT_VERSION'] ||= '2.8.2'
  load "#{__dir__}/setup_git.rake"
  task :setup => :git
else
  task :git
end

#
# setup build tools(devkit, gnu packages, ...)
#
load "#{__dir__}/setup_tools.rake"
task :setup => :tools

#
# setup bundler if required(not work for now)
#
if File.exists?("#{HOW_ROOT}/Gemfile")
  ENV['BUNDLER_VERSION'] ||= '1.12.3'

  load "#{__dir__}/setup_bundler.rake"
  task :setup => :bundler

  unless ENV['update'] == 'false' || ENV['update_gems'] == 'false'
    task :setup do
      cd HOW_ROOT do
        `bundle install --path vendor/bundle`
      end
    end
  end
end

