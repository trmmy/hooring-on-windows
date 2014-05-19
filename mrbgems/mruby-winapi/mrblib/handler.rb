class WinAPI::Hook::Handler
  include Enumerable

  def self.create(type)
    self.new
  end

  def each(*args, &block)
    @handlets.each(*args, &block)
  end

  def <<(handlet)
    handlets << handlet
  end

  def clean
    handlets.clean
  end

  def receive(message, *objs)
    handlets.each do |handlet|
      handlet[message].call(*objs)
    end
  end

  private
  def handlets
    @handlets ||= []
  end
end
