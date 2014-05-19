class Key
  def self.[](key)
    @keys ||= {
      :shift       => Key.scan( :LSHIFT ),

      :asterisk    => Key.shift( :COLON ),
      :exclamation => Key.shift( :ONE ),
      :doublequote => Key.shift( :TWO ),
      :hash        => Key.shift( :THREE ),
      :dollar      => Key.shift( :FOUR ),
      :percent     => Key.shift( :FIVE ),
      :ampersand   => Key.shift( :SIX ),
      :apostrophe  => Key.shift( :SEVEN ),
      :leftparen   => Key.shift( :EIGHT ),
      :rightparen  => Key.shift( :NINE ),
      :equal       => Key.shift( :MINUS ),
      :tilde       => Key.shift( :CARET ),
      :bar         => Key.shift( :YEN ),
      :grave       => Key.shift( :AT ),
      :plus        => Key.shift( :SEMICOLON ),
      :underscore  => Key.shift( :BACKSLASH ),
      :question    => Key.shift( :SLASH ),
      :lt          => Key.shift( :COMMA ),
      :gt          => Key.shift( :PERIOD ),

      :at          => Key.scan( :AT ),
      :colon       => Key.scan( :COLON ),
      :caret       => Key.scan( :CARET ),

    }
    @keys[key] || key
  end

  def to_sym
    @sym
  end
  
  def sym=(arg)
    @sym = arg
  end

  def self.mouse(params = { })
  end
  
  def self.code(params = { })
    k = Key.new
    k.with_shift if params[:shift]
    k.sym = params[:scan] || params[:key]
    if params[:scan]
      k.code = ::Keyboard::Scan[params[:scan]]
      k.is_scan
    else
      k.code = ::Keyboard::Virtual[params[:key]]
    end
    k
  end

  def self.shift(scan)
    self.code :shift => true, :scan => scan
  end

  def self.scan(scan, params = { })
    params[:scan] = scan
    self.code params
  end

  def self.virtual(virtual, params = { })
    params[:virtual] = virtual
    self.code params
  end

  def [](key)
    case key
    when :shift
      with_shift?
    when :scan
      #is_scan? ? code : nil
      to_sym
    when :key
      is_scan? ? nil : code
    when :extended
      is_extended?
    else
      nil
    end
  end

  def to_s
    "#{with_shift?}, #{is_extended?}, #{is_scan?}, #{code}"
  rescue => a
    p a
  end

end
