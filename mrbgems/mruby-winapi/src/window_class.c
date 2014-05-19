#include <mruby.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include <mruby/winapi.h>
#include <mruby/winapi/window.h>

struct RClass*
winapi_window_class_class(mrb_state* mrb)
{
  return
    mrb_class_get_under(
      mrb,
      winapi_class(mrb),
      "WindowClass");
}

mrb_value
winapi_window_class_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_window_class_class(mrb));
}


LRESULT CALLBACK GenericWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  /* winapi_window_class(mrb); */
}
/**
   Window["HooringOnWindowClass"] = Proc.new do
   
   end
   main = Window["HooringOnWindowClass"].new
 */
mrb_value
winapi_window_class_register(mrb_state* mrb, mrb_value self)
{
  WNDCLASSEX wc;
  wc.cbSize = sizeof(wc);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = GenericWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetModuleHandle(NULL);
  wc.hIcon = NULL;
  wc.hIconSm = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "HooringOnWindowClass";

  return mrb_bool_value(RegisterClassEx(&wc));
}

void
winapi_window_class_init(mrb_state* mrb)
{
  struct RClass* window_class;
  window_class = mrb_define_class_under(mrb, winapi_class(mrb), "WindowClass", mrb->object_class);
  MRB_SET_INSTANCE_TT(window_class, MRB_TT_DATA);
  
}
