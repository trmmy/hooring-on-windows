#ifndef APPLICATION_H
#define APPLICATION_H

#include <mruby.h>
#include <mruby/winapi.h>

struct winapi_application {
  HINSTANCE handle;
};

struct winapi_application* winapi_application_malloc(mrb_state*);
struct winapi_application* winapi_application_obj(mrb_state*, mrb_value);
struct RClass* winapi_application_class(mrb_state*);
mrb_value winapi_application_class_value(mrb_state*);
mrb_value winapi_application_new(mrb_state*,  struct winapi_application*);
mrb_value winapi_application_new_hinstance(mrb_state*, HINSTANCE);

#endif
