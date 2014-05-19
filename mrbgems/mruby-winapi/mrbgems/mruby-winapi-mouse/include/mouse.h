#ifndef MOUSE_H
#define MOUSE_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include <mruby/winapi.h>

// ----------------------

/* #define mouse_p(k) TRUE */
/* #define MOUSE(k) (mouse_p(k) ? (KEYBDINPUT*)mrb_data_get_ptr(mrb, k, &mrb_mouse_type) : ) */
#define MOUSE(k) ((struct mrb_mouse*)mrb_data_get_ptr(mrb, k, &mrb_mouse_type))

// data

// memory
struct mrb_mouse {
    MOUSEINPUT mi;
};


// malloc
struct mrb_mouse*
mrb_mouse_malloc(mrb_state *mrb);

// free
void
mrb_mouse_free(mrb_state *mrb, void *ptr);

// ----------------------
// type definition
static struct mrb_data_type mrb_mouse_type = { "Mouse", mrb_mouse_free };

// ----------------------
// construction
mrb_value
mrb_mouse_new(mrb_state* mrb, struct mrb_mouse* mouse);

mrb_value
mrb_mouse_new_alloc(mrb_state* mrb, mrb_value self);

#endif // MOUSE_H
