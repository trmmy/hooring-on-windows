use_devkit = true # hard code for now

#
# oneclick rubyinstaller's devkit
#
if use_devkit
  task :tools => 'devkit:install'

  devkit = 'DevKit-mingw64-64-4.7.2-20130224-1432-sfx' # depend on ruby version
  devkit_dir = "#{tools_dir}/#{devkit}"

  namespace :devkit do
    unshift_path File.join(devkit_dir, 'bin')
    unshift_path File.join(devkit_dir, 'mingw/bin')

    task :install => devkit_dir

    directory devkit_dir

    file devkit_dir => File.join(cache_dir, "#{devkit}.exe") do |t|
      inflate t.prerequisites.first, t.name
    end

    file File.join(cache_dir, "#{devkit}.exe") => cache_dir do |t|
      download "http://dl.bintray.com/oneclick/rubyinstaller/#{File.basename(t.name)}", t.name
    end
  end
end

#
# gnu win32 packages
#
namespace :gnu do
  pkgs = { }

  unless use_devkit
    # packages mruby requires
    {
      bison: '2.4.1',
      libintl: '0.14.4',
      libiconv: '1.9.2-1',
      regex: '2.7',
      m4: '1.4.14-1',
    }.each do |pkg, ver|
      pkgs[pkg] = ver # unless find_exe(pkg.to_s.ext('exe')) # in case of dll ?
    end
  end

  # mruby-onig-regexp requires
  {
    gzip: '1.3.12',
    tar: '1.13-1',
    libintl: '0.14.4', # tar requires
    libiconv: '1.9.2-1', # tar requires
  }.each do |pkg, ver|
    pkgs[pkg] = ver # unless find_exe(pkg.to_s.ext('exe')) # in case of dll ?
  end

  gnu_dir = "#{tools_dir}/gnu"

  push_path File.join(gnu_dir, 'bin')

  pkgs.each do |pkg, ver|
    cached = "#{cache_dir}/#{pkg}-#{ver}-bin.zip"

    installed_mark = File.join(gnu_dir, 'manifest', File.basename(cached)).ext('mft')

    task :install => "gnu:#{pkg}"

    task pkg => installed_mark

    namespace pkg do
      file cached => File.dirname(cached) do |t|
        download "http://downloads.sourceforge.net/gnuwin32/#{File.basename(t.name)}", t.name
      end

      if File.exist?(installed_mark)
        # # ensure all entires installed
        # task pkg do
        #   File.open(installed_mark) { |f|
        #     f.each_line do |line|
        #       ff = "gnu/#{line.chomp.split(' ').first}"
        #       task "gnu:#{pkg}" do
        #       end
        #     end
        #   })
        # end
      else
        file installed_mark => cached do |t|
          inflate t.prerequisites.first, gnu_dir
        end
      end
    end
  end

  task :install => File.join(gnu_dir, 'bin/libintl-2.dll')

  file File.join(gnu_dir, 'bin/libintl-2.dll') do |t| # tar requires libintl as `libintl-2.dll`, but libintl 0.14.4 provides `libintl3.dll`
    cd File.dirname(t.name) do
      cp 'libintl3.dll', File.basename(t.name)
    end
  end
end

task :tools => 'gnu:install'
