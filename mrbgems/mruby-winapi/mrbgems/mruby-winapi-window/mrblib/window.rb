class WinAPI::Window
  def id
    @id ||= "#{class_name}/#{title}".to_sym
  end

  class << self
    alias __find__ find

    def find_all(arg=nil, &block)
      return __find__(&block) if block

      arg = { title: arg } unless arg.is_a? Hash

      __find__ { |w|
        arg.all? { |k, v|
          v === w.__send__(k)
        }
      }
    end

    def find(arg=nil, &block)
      find_all(arg, &block).first
    end
  end
end
