MRuby::Gem::Specification.new('mruby-dll-hooring') do |spec|
  spec.license = 'MIT'
  spec.authors = 'TERAJIMA Motoyuki(@trmmy)'
end

MRuby.each_target do |target|
  bin = 'hooring'
  @bins << bin

  # mruby -- Rakefile:53-55
  current_dir = __dir__.relative_path_from(Dir.pwd) # gem.dir -> __dir__
  relative_from_root = __dir__.relative_path_from(HOW_ROOT) # gem.dir -> __dir__, MRUBY_ROOT -> HOW_ROOT
  current_build_dir = "#{build_dir}/#{relative_from_root}"

  # mruby -- Rakefile:58-59
  exec = exefile("#{build_dir}/bin/#{bin}")
  objs = Dir.glob("#{current_dir}/tools/#{bin}/*.{c,cpp,cxx,cc}").map { |f| objfile(f.pathmap("#{current_build_dir}/tools/#{bin}/%n")) }

  file exec.ext('dll') => Dir.glob("#{current_dir}/tools/#{bin}/*.def")

  file("#{current_dir}/tools/mruby/mruby.c") { |t|
    mkdir_p File.dirname(t.name)
    cp "#{MRUBY_ROOT}/mrbgems/mruby-bin-mruby/tools/mruby/mruby.c", t.name
  }.invoke

  file("#{current_dir}/tools/mirb/mirb.c") { |t|
    mkdir_p File.dirname(t.name)
    cp "#{MRUBY_ROOT}/mrbgems/mruby-bin-mirb/tools/mirb/mirb.c", t.name
    sed_is t.name, "int last_char;", "/* int last_char; */"
  }.invoke

  # mruby -- Rakefile:61-66
  file exec.ext('dll') => objs + [libfile("#{build_dir}/lib/libmruby")] do |t| # exec -> exec.ext('dll')
    gem_flags = gems.map { |g| g.linker.flags }
    gem_flags_before_libraries = gems.map { |g| g.linker.flags_before_libraries }
    gem_flags_after_libraries = gems.map { |g| g.linker.flags_after_libraries }
    gem_libraries = gems.map { |g| g.linker.libraries }
    gem_library_paths = gems.map { |g| g.linker.library_paths }

    gem_flags << '-shared'
    gem_flags << '-Wl,--enable-stdcall-fixup,--add-stdcall-alias,--kill-at'
    gem_flags << Dir.glob("#{current_dir}/tools/#{bin}/*.def").join(' ')

    # mruby -- Rakefile:67-68
    linker.run t.name, t.prerequisites, gem_libraries, gem_library_paths, gem_flags, gem_flags_before_libraries, gem_flags_after_libraries
  end
end
