#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include <mruby/winapi.h>

// ----------------------

/* #define keyboard_p(k) TRUE */
/* #define KEYBOARD(k) (keyboard_p(k) ? (KEYBDINPUT*)mrb_data_get_ptr(mrb, k, &mrb_keyboard_type) : ) */
#define KEYBOARD(k) ((struct mrb_keyboard*)mrb_data_get_ptr(mrb, k, &mrb_keyboard_type))

// data

// memory
struct mrb_keyboard {
    KEYBDINPUT ki;
};


// malloc
struct mrb_keyboard*
mrb_keyboard_malloc(mrb_state *mrb);

// free
void
mrb_keyboard_free(mrb_state *mrb, void *ptr);

// ----------------------
// type definition
static struct mrb_data_type mrb_keyboard_type = { "Keyboard", mrb_keyboard_free };

// ----------------------
// construction
mrb_value
mrb_keyboard_new(mrb_state* mrb, struct mrb_keyboard* keyboard);

mrb_value
mrb_keyboard_new_alloc(mrb_state* mrb, mrb_value self);

#endif // KEYBOARD_H
