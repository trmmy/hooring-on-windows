#include <mruby.h>
#include <mruby/winapi.h>

mrb_value mrb_point_hash_new(mrb_state*, POINT*);
mrb_value mrb_point_hash_set(mrb_state*, mrb_value, POINT*);
int mrb_hash_point_update(mrb_state*, mrb_value, POINT*);
