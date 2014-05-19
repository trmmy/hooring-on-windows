#ifndef KEY_H
#define KEY_H

#include <mruby.h>
#include <mruby/data.h>

#include <mruby/winapi.h>

// ----------------------

// |--------|--------|--------|--------|
//                   |<-- scan code or |
//                   |  virtual key -->|
// is_extended?, with_shift?, is_scan?
#define mrb_key_value(code) (mrb_key_new_alloc(mrb, code))
#define mrb_key_code(mrb, key) ((struct mrb_key*)mrb_data_get_ptr(mrb, key, &mrb_key_type))

struct mrb_key {
    mrb_int code;
};

// malloc
struct mrb_key*
mrb_key_malloc(mrb_state *mrb);

// free
void
mrb_key_free(mrb_state *mrb, void *ptr);

// ----------------------
// type definition
static struct mrb_data_type mrb_key_type = { "Key", mrb_key_free };

mrb_value
mrb_key_new(mrb_state* mrb, struct mrb_key* key);

mrb_value
mrb_key_new_alloc(mrb_state* mrb, mrb_value self);

#endif // WINDOW_H
