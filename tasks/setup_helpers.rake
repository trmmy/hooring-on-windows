ENV['HOW_TOOLS_DIR'] or fail

task :open_uri do
  require 'open-uri'

  unless ENV['SSL_CERT_FILE']
    ENV['SSL_CERT_FILE'] = File.join(tools_dir, 'cacert.pem')
  end

  unless File.exists?(ENV['SSL_CERT_FILE'])
    STDOUT.puts "download https://curl.haxx.se/ca/cacert.pem > #{ENV['SSL_CERT_FILE']}"
    STDOUT.puts '______________________________________________________________________________________________________________'
    STDOUT.puts '|  INTENTIONAL WARNINGS ABOUT OPENSSL::SSL::VERIFY_PEER                                                      |'
    begin
      require 'openssl'
      original_verify_peer = OpenSSL::SSL::VERIFY_PEER
      OpenSSL::SSL::VERIFY_PEER = OpenSSL::SSL::VERIFY_NONE
      mkdir_p tools_dir
      when_writing do
        File.binwrite(
          ENV['SSL_CERT_FILE'],
          open('https://curl.haxx.se/ca/cacert.pem').read)
      end
    ensure
      OpenSSL::SSL::VERIFY_PEER = original_verify_peer
      STDOUT.puts '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    end
  end
end


#
# directories
#
def cache_dir
  @cache_dir ||= ENV['HOW_TOOLS_CACHE_DIR'] || File.join(tools_dir, 'cache')
end

def tools_dir
  @tools_dir ||= ENV['HOW_TOOLS_DIR']
end


#
# paths
#
def push_path(*paths)
  ENV['PATH'] = [ENV['PATH'], *(paths.flatten.map { |path| path.gsub('/', '\\') })].join(';')
end

def unshift_path(*paths)
  ENV['PATH'] = [*(paths.flatten.map { |path| path.gsub('/', '\\') }), ENV['PATH']].join(';')
end

def find_exe(exe)
  @path ||= ENV['PATH'].split(';').map(&:strip)
  @path.map { |path|
    File.join(path, exe)
  }.find { |exe_path|
    File.exists?(exe_path)
  }
end

def find_mgem(gem)
  @mgem_paths ||= ENV['MGEM_PATH'].split(';').map(&:strip)
  @mgem_paths.map { |path|
    "#{path}/#{gem}"
  }.find { |gem_path|
    Dir.exists?(gem_path)
  }
end

def find_subgem(gem, subgem)
  find_mgem("mruby-#{gem}/mrbgems/mruby-#{gem}-#{subgem}")
end


#
# FileUtils extension
#
module FileUtils
  # sed -i -e "s/pattern/replacement/g"
  def sed_is(file, pattern, replacement)
    File.write(
      file,
      File.read(file).gsub!(pattern, replacement))
  end

  def download(url, out=File.basename(url))
    Rake::Task['open_uri'].invoke
    STDOUT.puts "download #{url} > #{out}"
    when_writing do
      begin
        verbose false do
          mkdir_p File.dirname(out)
          File.binwrite(out, open(url).read) # tentative
          # mv "#{out}.part", out
        end
      rescue => e
        STDERR.puts "failed downloading #{url}"
        rm_rf out
        raise e
      end
    end
  end

  def inflate(ar, out)
    when_writing do # install 7z if not exist
      unless ENV['PATH'].split(';').find { |dir| File.exists?(File.join(dir, '7z.exe')) }
        mkdir_p cache_dir unless Dir.exists?(cache_dir)
        cd cache_dir do
          unless File.exists?('7z1600-x64.msi')
            download 'http://7-zip.org/a/7z1600-x64.msi', '7z1600-x64.msi'
          end
        end
        cd tools_dir do
          unless Dir.exists?('7z1600-x64')
            cd cache_dir do
              `msiexec.exe /A 7z1600-x64.msi targetdir="#{File.join(tools_dir, '7z1600-x64').gsub('/', '\\')}" /QN /LOG log`
            end
          end
          unshift_path File.join(tools_dir, '7z1600-x64/Files/7-Zip')
        end
      end
    end

    STDOUT.puts "inflate #{ar} > #{out}"
    when_writing do
      begin
        verbose false do
          mkdir_p out unless Dir.exists?(out)
          `7z.exe x #{ar.gsub('/', '\\')} -y -o#{out.gsub('/', '\\')}`
        end
      rescue => e
        STDERR.puts "failed inflating #{out}"
        rm_rf out
        raise e
      end
    end
  end
end
