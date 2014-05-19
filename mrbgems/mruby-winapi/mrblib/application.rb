class WinAPI::Application
  # def self.inherited(subclass)
  #   raise Exception if WinAPI.application
  #   WinAPI.application = subclass.new
  # end

  def window
    @window ||= { }
  end

  def do_start
    window[:main].tap { |w|
      w.move x: 300, y: 200, width: 320, height: 100, repaint: true
      w.update
      w.hide
    }

    Window.get_message do |msg|
      msg.translate
      msg.dispatch
    end
  end
end
