#ifndef MRUBY_WINAPI_H
#define MRUBY_WINAPI_H

#ifdef WINVER
#undef WINVER
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <mruby.h>
#include <windows.h>

#define mrb_struct_malloc(strct) ((struct strct *)mrb_malloc(mrb, sizeof(struct strct)))
#define mrb_cstr_symbol(mrb, s) (mrb_symbol_value(mrb_intern_cstr(mrb, s)))

struct RClass* winapi_class(mrb_state*);
mrb_value winapi_class_value(mrb_state*);

#endif /* MRUBY_WINAPI_H */
