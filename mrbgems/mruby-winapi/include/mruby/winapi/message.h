#include <mruby.h>

#include <mruby/winapi.h>

/*
  typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
  } MSG;
*/

typedef MSG winapi_message;

winapi_message* winapi_message_obj(mrb_state*, mrb_value);
struct RClass* winapi_message_class(mrb_state*);
mrb_value winapi_message_class_value(mrb_state*);
mrb_value winapi_message_new(mrb_state*, winapi_message*);

UINT winapi_message_get_registered(mrb_state*, const char*);
