#include <mruby.h>

#include <mruby/winapi.h>

mrb_value mrb_rect_hash_new(mrb_state*, RECT*);
mrb_value mrb_rect_hash_set(mrb_state*, mrb_value, RECT*);
int mrb_hash_rect_update(mrb_state*, mrb_value, RECT*);
