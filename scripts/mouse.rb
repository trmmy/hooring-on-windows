module Hook
  def self.call(params={ })
    return true if params[:event] != :keyhook

    case params[:scan_code]
    when ::Keyboard::Scan[:CONVERT]
      m = ::Mouse.new
      m.add_event(params[:key_up] ? ::Mouse::RIGHT_UP : ::Mouse::RIGHT_DOWN)
      ::Mouse.input m
      false
    when ::Keyboard::Scan[:NONCONVERT]
      m = ::Mouse.new
      m.add_event(params[:key_up] ? ::Mouse::LEFT_UP : ::Mouse::LEFT_DOWN)
      ::Mouse.input m
      false
    else
      true
    end
  end
end
