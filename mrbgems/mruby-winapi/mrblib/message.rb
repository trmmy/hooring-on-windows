class WinAPI::Message
  class << self
    def [](name)
      name = name.to_s
      unless messages.has_key? name
        messages[name] = register(name)
      end
      messages[name]
    end

    def messages
      @messages ||= { }
    end
  end
end
