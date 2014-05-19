class WinAPI::Hook::Handlet
  def initialize(&block)
    @operations = { }
    instance_exec(self, &block) if block
  end

  def handle(message, &operation)
    @operations[message] = operation
  end

  def handles?(message)
    @operations[message]
  end

  def receive(message, *objs)
    @operations[message].call(*objs)
  end
end
