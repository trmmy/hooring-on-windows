#ifndef WINDOW_H
#define WINDOW_H

#include <mruby.h>

#include <mruby/winapi.h>

typedef struct winapi_window {
  HWND handle;
} winapi_window;

struct RClass* winapi_window_class(mrb_state*);
mrb_value winapi_window_class_value(mrb_state*);
winapi_window* winapi_window_obj(mrb_state*, mrb_value);
winapi_window* winapi_window_malloc(mrb_state*, HWND);

mrb_value winapi_window_new(mrb_state*, winapi_window*);
mrb_value winapi_window_new_alloc(mrb_state*, HWND);

/* ----------------- */
#define winapi_window_handle(mrb, self) (winapi_window_obj(mrb, self)->handle)

BOOL CALLBACK EnumWindowsProc( HWND hWnd, LPARAM lParam );


#endif /* WINDOW_H */
