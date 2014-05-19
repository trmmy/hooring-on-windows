class WinAPI::Hook
  class << self
    def handle(type)
      define_method type do |&block|
        if block
          handler(type) << Handlet.new(&block)
        end
        handler(type)
      end

      define_method "#{type}=" do |handler|
        handler(type).clear
        handler(type) << handler
      end
    end
  end

  def handler(type)
    @handlers ||= { }
    @handlers[type.to_sym] ||= Handler.create(type.to_sym)
  end

  def deactivate(type=nil)
    if type
      handler(type).deactivate
    else
      @handlers.each(&:deactivate)
    end
  end

  def activate(type=nil)
    if type
      handler(type).activate
    else
      @handlers.each(&:activate)
    end
  end

  def receive(type, message)
    handler(type).each do |handlet|
      handlet.receive(message)
    end
  end
end
