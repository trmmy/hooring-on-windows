#include <mruby.h>
#include <mruby/hash.h>

#include <mruby/winapi.h>

mrb_value
mrb_rect_hash_set(mrb_state* mrb, mrb_value hash, RECT* rect)
{
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "left")),   mrb_fixnum_value(rect->left));
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "top")),    mrb_fixnum_value(rect->top));
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "right")),  mrb_fixnum_value(rect->right));
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "bottom")), mrb_fixnum_value(rect->bottom));

  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "width")),  mrb_fixnum_value(rect->right - rect->left));
  mrb_hash_set(mrb, hash, mrb_symbol_value(mrb_intern_cstr(mrb, "height")), mrb_fixnum_value(rect->bottom - rect->top));

  return hash;
}

mrb_value
mrb_rect_hash_new(mrb_state* mrb, RECT* rect)
{
  return mrb_rect_hash_set(mrb, mrb_hash_new(mrb), rect);
}


int
mrb_hash_rect_update(mrb_state* mrb, mrb_value hash, RECT* rect)
{
  if(!mrb_hash_p(hash)) {
    return 0;
  }
  if(rect == NULL) {
    return -1;
  }

  {
    mrb_value left = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "left"));
    if(!mrb_nil_p(left)) {
      rect->left = mrb_fixnum(left);
    }
  }
  {
    mrb_value top = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "top"));
    if(!mrb_nil_p(top)) {
      rect->top = mrb_fixnum(top);
    }
  }
  {
    mrb_value right = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "right"));
    if(!mrb_nil_p(right)) {
      rect->right = mrb_fixnum(right);
    }
  }
  {
    mrb_value bottom = mrb_hash_get(mrb, hash, mrb_cstr_symbol(mrb, "bottom"));
    if(!mrb_nil_p(bottom)) {
      rect->bottom = mrb_fixnum(bottom);
    }
  }
  return 0;
}
