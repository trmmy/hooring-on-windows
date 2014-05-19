#include <mruby.h>

#include <mruby/winapi.h>

/* #define winapi_handler(s, w) ((struct winapi_wnd*)mrb_data_get_ptr(s, w, &winapi_window_type)) */
/* #define winapi_handler(s, w) (winapi_window(s, w)->hwnd) */
/* #define WND(w) winapi_window_handle(winapi, w) */

typedef mrb_value(*hook_obj_wrapper)(mrb_state*, int, WPARAM, LPARAM);
typedef LRESULT CALLBACK(*HFunc)(int, WPARAM, LPARAM);

struct winapi_handler {
  HHOOK handle;
  int   type;
  HFunc func;
};

LRESULT CALLBACK hook_proc(mrb_state*, char*, hook_obj_wrapper, int, WPARAM, LPARAM);

struct winapi_handler* winapi_handler_obj(mrb_state*, mrb_value);
struct RClass* winapi_handler_class(mrb_state*);
mrb_value winapi_handler_class_value(mrb_state*);
mrb_value winapi_handler_new(mrb_state*, struct winapi_handler*);
mrb_value winapi_handler_new_alloc(mrb_state*, int, HFunc);



