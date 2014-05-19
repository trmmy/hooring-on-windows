class WinAPI::Window
  def ==(other)
    id == other.id
  end

  %w(show hide restore maximize minimize).each do |action|
    define_method action do
      self.display = self.class.const_get(action.upcase)
    end
  end
end
