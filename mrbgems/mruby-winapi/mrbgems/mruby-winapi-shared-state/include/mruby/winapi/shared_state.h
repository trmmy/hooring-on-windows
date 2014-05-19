#include <mruby.h>

#include <mruby/winapi.h>

mrb_state* mrb_open_shared(HANDLE);
HANDLE get_process_handle(char*);

/* #define mrb_open mrb_open_shared */
