ver   = ENV['GIT_VERSION'] || '2.8.2'

task :git => 'git:install'

git = "PortableGit-#{ver}-64-bit"

install_dir = File.join(tools_dir, git)

namespace :git do
  directory install_dir

  push_path File.join(install_dir, 'bin')

  task :install => install_dir

  file install_dir => File.join(cache_dir, "#{git}.7z.exe") do |t|
    inflate t.prerequisites.first, t.name
  end

  file Rake::Task[install_dir].prerequisites.first do |t|
    download "https://github.com/git-for-windows/git/releases/download/v#{ver}.windows.1/#{git}.7z.exe", t.name
  end
end
