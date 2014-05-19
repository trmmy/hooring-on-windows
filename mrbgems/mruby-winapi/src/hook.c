#include <mruby.h>
#include <mruby/class.h>

#include <mruby/winapi.h>
#include <mruby/winapi/handler.h>

struct RClass*
winapi_hook_class(mrb_state* mrb)
{
  return
    mrb_class_get_under(
      mrb,
      winapi_class(mrb),
      "Hook");
}

mrb_value
winapi_hook_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_hook_class(mrb));
}

void
winapi_hook_add_handle_type(mrb_state* mrb, char* type)
{
  mrb_funcall(
    mrb, winapi_hook_class_value(mrb), "handle",
    1,
    mrb_cstr_symbol(mrb, type));
}

void
winapi_hook_init(mrb_state* mrb)
{
  struct RClass* hook =
    mrb_define_class_under(
      mrb,
      winapi_class(mrb),
      "Hook",
      mrb->object_class);
  MRB_SET_INSTANCE_TT(hook, MRB_TT_DATA);
  
}
