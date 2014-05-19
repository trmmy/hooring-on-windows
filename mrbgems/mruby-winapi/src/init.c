#include <mruby.h>
#include <mruby/class.h>

void winapi_init(mrb_state*);
void winapi_application_init(mrb_state*);
void winapi_window_init(mrb_state*);
void winapi_window_class_init(mrb_state*);
void winapi_message_init(mrb_state*);
void winapi_hook_init(mrb_state*);
void winapi_hook_handler_init(mrb_state*);

void
mrb_mruby_winapi_gem_init(mrb_state* mrb)
{
  winapi_init(mrb);
  winapi_application_init(mrb);
  winapi_window_init(mrb);
  winapi_window_class_init(mrb);
  winapi_message_init(mrb);
  winapi_hook_init(mrb);
  winapi_hook_handler_init(mrb);
}

void
mrb_mruby_winapi_gem_final(mrb_state* mrb)
{
}
