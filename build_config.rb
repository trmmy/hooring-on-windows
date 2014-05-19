ENV['MGEMFILE'] ||= "#{HOW_ROOT}/MGemfile"

MRuby::Build.new do |conf|
  toolchain ENV['TOOLCHAIN'] ||= 'gcc'

  conf.instance_eval do
    unless methods.include? :__gem__ # here conf does not have `method_defined?`
      alias __gem__ gem
    end

    def gem(*args)
      arg = args.first
      case arg
      when Hash
        # load sub mrbgems if found
        #
        # `gem :foo => :bar` loads one/of/mgem_path/mruby-foo/mrbgems/mruby-foo-bar
        __gem__(find_subgem(*arg.first) || arg)
      when String
        __gem__(find_mgem(arg) || arg)
      else
        __gem__(arg)
      end
    end
  end

  conf.instance_eval File.read(ENV['MGEMFILE'])
end
