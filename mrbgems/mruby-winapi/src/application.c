#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

#include <mruby/winapi.h>
#include <mruby/winapi/application.h>

#include <mruby/hash.h>
#include <mruby/winapi/window.h>

static struct mrb_data_type winapi_application_type = { "winapi_application", mrb_free };

struct winapi_application*
winapi_application_malloc(mrb_state *mrb)
{
  return (struct winapi_application*)mrb_malloc(mrb, sizeof(struct winapi_application));
}

struct winapi_application*
winapi_application_obj(mrb_state* mrb, mrb_value application)
{
  return (struct winapi_application*)mrb_data_get_ptr(mrb, application, &winapi_application_type);
}

struct RClass*
winapi_application_class(mrb_state* mrb)
{
  return mrb_class_get_under(mrb, winapi_class(mrb), "Application");
}

mrb_value
winapi_application_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_application_class(mrb));
}

mrb_value
winapi_application_new(mrb_state* mrb, struct winapi_application* application)
{
  return
    mrb_obj_value(
      Data_Wrap_Struct(
        mrb, winapi_application_class(mrb), &winapi_application_type, application));
}

mrb_value
winapi_application_new_hinstance(mrb_state* mrb, HINSTANCE instance)
{
  struct winapi_application* application = winapi_application_malloc(mrb);
  application->handle = instance;
  return winapi_application_new(mrb, application);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

mrb_value
winapi_application_start(mrb_state* mrb, mrb_value self)
{
  {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = winapi_application_obj(mrb, self)->handle;
    wc.hIcon = NULL;
    wc.hIconSm = wc.hIcon;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "HooringOnWindowClass";

    if(RegisterClassEx(&wc)==0){
      return mrb_fixnum_value(1);
    }
  }

  {

    /*
    HWND CreateWindow(
      LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName,
      DWORD dwStyle,
      int x,
      int y,
      int nWidth,
      int nHeight,
      HWND hwndParent,
      HMENU hmenu,
      HANDLE hinst,
      LPVOID lpvParam );
    */
    HWND hWnd;
    hWnd = CreateWindow("HooringOnWindowClass",
                        "HOW",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        winapi_application_obj(mrb, self)->handle,
                        NULL
                        );
    
    if(hWnd == NULL){
      return mrb_fixnum_value(1);
    }

    mrb_value main_wnd = winapi_window_new_alloc(mrb, hWnd);
    mrb_value windows = mrb_funcall(mrb, self, "window", 0);
    mrb_hash_set(mrb, windows, mrb_cstr_symbol(mrb, "main"), main_wnd);
    mrb_funcall(mrb, self, "do_start", 0);
  }

  return mrb_fixnum_value(0);
}

void
winapi_application_init(mrb_state* mrb)
{
  struct RClass* application =
    mrb_define_class_under(
      mrb, 
      winapi_class(mrb),
      "Application",
      mrb->object_class);
  MRB_SET_INSTANCE_TT(application, MRB_TT_DATA);

  mrb_define_method(mrb, application, "start",    winapi_application_start,   ARGS_NONE());
  
}
