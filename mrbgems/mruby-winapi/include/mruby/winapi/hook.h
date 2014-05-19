#ifndef HOOK_H
#define HOOK_H

#include <mruby.h>
#include <mruby/winapi.h>

struct RClass* winapi_hook_class(mrb_state*);
mrb_value winapi_hook_class_value(mrb_state* mrb);

void mrb_winapi_hook_add_handle_type(mrb_state* mrb);

#endif
