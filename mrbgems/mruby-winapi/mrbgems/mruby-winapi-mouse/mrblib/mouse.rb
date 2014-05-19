class Mouse
  def self.input(*args)
    self.send(*args)
  end
end
