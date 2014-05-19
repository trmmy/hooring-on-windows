MRuby::Gem::Specification.new('mruby-bin-hooring') do |spec|
  spec.license = 'MIT'
  spec.authors = 'TERAJIMA Motoyuki(@trmmy)'

  spec.add_dependency 'mruby-dll-hooring'

  # spec.bins = %w(hooring)
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

  file exec => exec.ext('dll')

  # mruby -- Rakefile:61-68
  file exec => objs + [libfile("#{build_dir}/lib/libmruby")] do |t|
    gem_flags = gems.map { |g| g.linker.flags }
    gem_flags_before_libraries = gems.map { |g| g.linker.flags_before_libraries }
    gem_flags_after_libraries = gems.map { |g| g.linker.flags_after_libraries }
    gem_libraries = gems.map { |g| g.linker.libraries }
    gem_library_paths = gems.map { |g| g.linker.library_paths }
    linker.run t.name, t.prerequisites, gem_libraries, gem_library_paths, gem_flags, gem_flags_before_libraries, gem_flags_after_libraries
  end
end
