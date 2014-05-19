class Symbol
  def each
    yield self
  end

  def upcase
    to_s.upcase.to_sym
  end

  def downcase
    to_s.downcase.to_sym
  end

  def split(*args)
    to_s.split(*args).map(&:to_sym)
  end

  def size
    to_s.size
  end
end
