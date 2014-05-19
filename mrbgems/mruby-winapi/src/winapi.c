#include <mruby.h>

struct RClass*
winapi_class(mrb_state* mrb)
{
  return mrb_module_get(mrb, "WinAPI");
}

mrb_value
winapi_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_class(mrb));
}

void
winapi_init(mrb_state* mrb)
{
  mrb_define_module(mrb, "WinAPI");
}

