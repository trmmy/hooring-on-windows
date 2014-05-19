#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/string.h>

#include <mruby/winapi.h>
#include <mruby/winapi/message.h>

static struct mrb_data_type winapi_message_type = { "winapi_message", mrb_free };

winapi_message*
winapi_message_obj(mrb_state* mrb, mrb_value message)
{
  return (winapi_message*)mrb_data_get_ptr(mrb, message, &winapi_message_type);
}

struct RClass*
winapi_message_class(mrb_state* mrb)
{
  return mrb_class_get_under(mrb, winapi_class(mrb), "Message");
}

mrb_value
winapi_message_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_message_class(mrb));
}

mrb_value
winapi_message_new(mrb_state* mrb, winapi_message* message)
{
  return
    mrb_obj_value(
      Data_Wrap_Struct(
        mrb, winapi_message_class(mrb), &winapi_message_type, message));
}

mrb_value
winapi_message_class_register(mrb_state* mrb, mrb_value class)
{
  mrb_value message;
  mrb_get_args(mrb, "S", &message);
  {
    UINT mid =
      RegisterWindowMessage(
        RSTRING_PTR(message));
    if(mid == 0){
      mrb_raisef(mrb, E_RUNTIME_ERROR, "Could not register message '%s'", RSTRING_PTR(message));
    }
    return mrb_fixnum_value(mid);
  }
}

UINT
winapi_message_get_registered(mrb_state* mrb, const char* message)
{
  return
    mrb_fixnum(
      mrb_funcall(
        mrb,
        winapi_message_class_value(mrb),
        "[]",
        1, mrb_str_new_cstr(mrb, message)));
}

mrb_value
winapi_message_translate(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(TranslateMessage(winapi_message_obj(mrb, self)));
}

mrb_value
winapi_message_dispatch(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(DispatchMessage(winapi_message_obj(mrb, self)));
}

void
winapi_message_init(mrb_state* mrb)
{
  struct RClass* message =
    mrb_define_class_under(
      mrb, 
      winapi_class(mrb),
      "Message",
      mrb->object_class);
  MRB_SET_INSTANCE_TT(message, MRB_TT_DATA);
  
  mrb_define_class_method(mrb, message, "register", winapi_message_class_register, ARGS_REQ(1));
  
  mrb_define_method(mrb, message, "translate", winapi_message_translate, ARGS_NONE());
  mrb_define_method(mrb, message, "dispatch", winapi_message_dispatch, ARGS_NONE());

  {
    struct RClass* constants =
      mrb_define_module_under(
        mrb, 
        winapi_message_class(mrb),
        "Constants");
    MRB_SET_INSTANCE_TT(constants, MRB_TT_DATA);
  }
  
}
