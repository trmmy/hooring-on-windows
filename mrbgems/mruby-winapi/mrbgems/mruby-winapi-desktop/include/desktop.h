#ifndef DESKTOP_H
#define DESKTOP_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/winapi.h>

// ----------------------
// data

// memory
struct mrb_desktop {
    HDESK hdesk;
};

// malloc
struct mrb_desktop*
mrb_desktop_malloc(mrb_state *mrb, HDESK hdesk);

// free
void
mrb_desktop_free(mrb_state *mrb, void *ptr);

// ----------------------
// type definition
static struct mrb_data_type mrb_desktop_type = { "Desktop", mrb_desktop_free };

// ----------------------
// construction
mrb_value
mrb_desktop_new(mrb_state* mrb, struct mrb_desktop* desktop);

#endif // DESKTOP_H
