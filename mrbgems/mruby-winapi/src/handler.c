#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>

#include <mruby/winapi/hook.h>
#include <mruby/winapi/handler.h>

/*
  NEXT
  SKIP
  
*/
/* gm = WinAPI::Handler.new */
/* gm.handle(WinAPI::Message::CAPTURECHANGED) do |msg| */
/* end */
/* typedef void (*Interceptor)(mrb_value*, WPARAM, LPARAM); */

void
winapi_handler_free(mrb_state* mrb, void* p)
{
  struct winapi_handler* handler = (struct winapi_handler*)p;
  if(handler->handle != NULL) {
    int res = UnhookWindowsHookEx(handler->handle);
    /* if(res==0) { */
    /*   should raise error ? */
    /* } */
  }
  mrb_free(mrb, p);
}

static struct mrb_data_type winapi_handler_type = { "winapi_handler", mrb_free };

LRESULT CALLBACK hook_proc(
  mrb_state* mrb, char* handler, hook_obj_wrapper wrapper, int nCode, WPARAM wp, LPARAM lp)
{
  mrb_value mrb_handler =
    mrb_funcall(
      mrb,
      winapi_hook_class_value(mrb),
      handler,
      0);
  mrb_value res =
    mrb_funcall(
      mrb,
      mrb_handler,
      "receive",
      1,
      (*wrapper)(mrb, nCode, wp, lp));

  if(mrb_nil_p(res)) {
    return
      CallNextHookEx(
        winapi_handler_obj(mrb, mrb_handler)->handle,
        nCode, wp, lp);
  }
  
  return mrb_fixnum(res);
}

struct winapi_handler*
winapi_handler_obj(mrb_state* mrb, mrb_value handler)
{
  return (struct winapi_handler*)mrb_data_get_ptr(mrb, handler, &winapi_handler_type);
}

struct RClass*
winapi_handler_class(mrb_state* mrb)
{
  return mrb_class_get_under(mrb, winapi_hook_class(mrb), "Handler");
}

mrb_value
winapi_handler_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_handler_class(mrb));
}

struct winapi_handler*
winapi_handler_malloc(mrb_state *mrb)
{
  return (struct winapi_handler*)mrb_malloc(mrb, sizeof(struct winapi_handler));
}

mrb_value
winapi_handler_new(mrb_state* mrb, struct winapi_handler* handler)
{
  return
    mrb_obj_value(
      Data_Wrap_Struct(
        mrb, winapi_handler_class(mrb), &winapi_handler_type, handler));
}

mrb_value
winapi_handler_new_alloc(mrb_state* mrb, int type, HFunc func)
{
  struct winapi_handler* handler = winapi_handler_malloc(mrb);
  handler->handle = NULL;
  handler->type = type;
  handler->func = func;
  return winapi_handler_new(mrb, handler);
}

mrb_value
winapi_hook_active_p(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(winapi_handler_obj(mrb, self)->handle != NULL);
}

mrb_value
winapi_hook_activate(mrb_state* mrb, mrb_value self)
{
  struct winapi_handler* handler = winapi_handler_obj(mrb, self);
  
  handler->handle =
    SetWindowsHookEx(
      handler->type,
      handler->func,
      GetModuleHandle(NULL), 0);
  
  return winapi_hook_active_p(mrb, self);
}

mrb_value
winapi_hook_deactivate(mrb_state* mrb, mrb_value self)
{
  struct winapi_handler* handler =
    ((struct winapi_handler*)mrb_data_get_ptr(mrb, self, &winapi_handler_type));

  if(handler->handle == NULL) {
    return winapi_hook_active_p(mrb, self);
  }

  {
    int res = UnhookWindowsHookEx(handler->handle);
    if(res == 0) {
      return winapi_hook_active_p(mrb, self);
    }
  }

  handler->handle = NULL;
  return winapi_hook_active_p(mrb, self);
}

/*
mrb_value
winapi_handler_class_create(mrb_state* mrb, mrb_value class)
{
  mrb_value type;
  mrb_get_args(mrb, "s", &type);
}
*/

void
winapi_hook_handler_init(mrb_state* mrb)
{
  struct RClass* handler =
    mrb_define_class_under(
      mrb, 
      winapi_class(mrb),
      "Handler",
      mrb->object_class);
  MRB_SET_INSTANCE_TT(handler, MRB_TT_DATA);

  mrb_define_method(mrb, handler, "active?",    winapi_hook_active_p,   ARGS_NONE());
  mrb_define_method(mrb, handler, "activate",   winapi_hook_activate,   ARGS_NONE());
  mrb_define_method(mrb, handler, "deactivate", winapi_hook_deactivate, ARGS_NONE());

  /* mrb_define_class_method(mrb, handler, "create", winapi_handler_class_create,   ARGS_REQ(1)); */
  
}
