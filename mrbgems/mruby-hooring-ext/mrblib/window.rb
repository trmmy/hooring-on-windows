module Kernel
  def Window(*args, &block)
    WinAPI::Window.find(*args, &block)
  end

  def Windows(*args, &block)
    WinAPI::Window.find_all(*args, &block)
  end
end
