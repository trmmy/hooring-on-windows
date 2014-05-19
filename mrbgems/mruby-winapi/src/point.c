#include <mruby.h>
#include <mruby/hash.h>

#include <mruby/winapi.h>

mrb_value
mrb_point_hash_set(mrb_state* mrb, mrb_value hash, POINT* point)
{
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "x")), mrb_fixnum_value(point->x));
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "y")), mrb_fixnum_value(point->y));
  return hash;
}

mrb_value
mrb_point_hash_new(mrb_state* mrb, POINT* point)
{
  return mrb_point_hash_set(mrb, mrb_hash_new(mrb), point);
}

int
mrb_hash_point_update(mrb_state* mrb, mrb_value hash, POINT* point)
{
  if(!mrb_hash_p(hash)) {
    return 0;
  }
  if(point == NULL) {
    return -1;
  }

  {
    mrb_value x = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "x"));
    if(!mrb_nil_p(x)) {
      point->x = mrb_fixnum(x);
    }
  }
  {
    mrb_value y = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "y"));
    if(!mrb_nil_p(y)) {
      point->y = mrb_fixnum(y);
    }
  }
  return 0;
}
