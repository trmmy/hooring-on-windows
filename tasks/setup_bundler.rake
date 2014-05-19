version = ENV['BUNDLER_VERSION'] || '1.12.3'

task :bundler do
  begin
    gem 'bundler', "=#{version}"
  rescue LoadError
    STDOUT.puts 'gem install bundler'
    when_writing do
      require 'rubygems/gem_runner'
      Gem::GemRunner.new.run %W(install bundler -v #{version} --no-rdoc --no-ri) # exit here for some reason?
      gem 'bundler', "=#{version}"
    end
  end
  require 'bundler'
end
