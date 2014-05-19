#include "key.h"

#include <mruby/data.h>
#include <mruby/class.h>

// malloc
struct mrb_key*
mrb_key_malloc(mrb_state *mrb)
{
    return mrb_struct_malloc(mrb_key);
}

// free
void
mrb_key_free(mrb_state *mrb, void *ptr)
{
    mrb_free(mrb, ptr);
}

mrb_value
mrb_key_new(mrb_state* mrb, struct mrb_key* key)
{
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_get(mrb, "Key"), &mrb_key_type, key));
}

mrb_value
mrb_key_new_alloc(mrb_state* mrb, mrb_value self)
{
    return mrb_key_new(mrb, mrb_key_malloc(mrb));
}

mrb_value
mrb_key_get_code(mrb_state* mrb, mrb_value self)
{
    return mrb_fixnum_value(mrb_key_code(mrb, self)->code & 0xFF);
}

mrb_value
mrb_key_set_code(mrb_state* mrb, mrb_value self)
{
    mrb_value code;
    mrb_get_args(mrb, "i", &code);
    mrb_key_code(mrb, self)->code
        = (mrb_key_code(mrb, self)->code & ~0xFF) | mrb_fixnum(code);
    return self;
}

mrb_value
mrb_key_test_extended(mrb_state* mrb, mrb_value self)
{
    return mrb_bool_value((mrb_key_code(mrb, self)->code >> 8) & 0x04);
}

mrb_value
mrb_key_set_extended(mrb_state* mrb, mrb_value self)
{
    mrb_key_code(mrb, self)->code |= 0x400;
    return self;
}

mrb_value
mrb_key_shift_required(mrb_state* mrb, mrb_value self)
{
    return mrb_bool_value((mrb_key_code(mrb, self)->code >> 8) & 0x02);
}

mrb_value
mrb_key_require_shift(mrb_state* mrb, mrb_value self)
{
    mrb_key_code(mrb, self)->code |= 0x200;
    return self;
}

mrb_value
mrb_key_test_scan_code(mrb_state* mrb, mrb_value self)
{
    return mrb_bool_value((mrb_key_code(mrb, self)->code >> 8) & 0x01);
}

mrb_value
mrb_key_set_scan_code(mrb_state* mrb, mrb_value self)
{
    mrb_key_code(mrb, self)->code |= 0x100;
    return self;
}

void
mrb_mruby_winapi_key_gem_init(mrb_state* mrb)
{
    struct RClass* key;
    key = mrb_define_class(mrb, "Key", mrb->object_class);
    MRB_SET_INSTANCE_TT(key, MRB_TT_DATA);
    mrb_define_class_method(mrb, key, "new", mrb_key_new_alloc, ARGS_NONE());
    
    mrb_define_method(mrb, key, "code", mrb_key_get_code, ARGS_NONE());
    mrb_define_method(mrb, key, "code=", mrb_key_set_code, ARGS_REQ(1));
    mrb_define_method(mrb, key, "is_extended?", mrb_key_test_extended, ARGS_NONE());
    mrb_define_method(mrb, key, "is_extended", mrb_key_set_extended, ARGS_NONE());
    mrb_define_method(mrb, key, "with_shift?", mrb_key_shift_required, ARGS_NONE());
    mrb_define_method(mrb, key, "with_shift", mrb_key_require_shift, ARGS_NONE());
    mrb_define_method(mrb, key, "is_scan?", mrb_key_test_scan_code, ARGS_NONE());
    mrb_define_method(mrb, key, "is_scan", mrb_key_set_scan_code, ARGS_NONE());
}

void
mrb_mruby_winapi_key_gem_final(mrb_state* mrb)
{
}


