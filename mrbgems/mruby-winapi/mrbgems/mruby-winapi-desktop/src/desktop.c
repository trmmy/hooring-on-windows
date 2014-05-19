#include "desktop.h"

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/gc.h>

#include <mruby/winapi/window.h>

#define DESK(d) (((struct mrb_desktop*)mrb_data_get_ptr(mrb, d, &mrb_desktop_type))->hdesk)

BOOL CALLBACK EnumDesktopProc(LPTSTR, LPARAM);

struct mrb_desktop*
mrb_desktop_malloc(mrb_state *mrb, HDESK hdesk)
{
    struct mrb_desktop *desktop;
    desktop = (struct mrb_desktop *)mrb_malloc(mrb, sizeof(*desktop));
    desktop->hdesk = hdesk;
    return desktop;
}

void
mrb_desktop_free(mrb_state *mrb, void *ptr)
{
    mrb_free(mrb, ptr);
/*     free(ptr); */
}

// ----------------------
// construction
mrb_value
mrb_desktop_new(mrb_state* mrb, struct mrb_desktop* desktop)
{
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_get(mrb, "Desktop"), &mrb_desktop_type, desktop));
}

mrb_value
mrb_desktop_new_alloc(mrb_state* mrb, HDESK hdesk)
{
    return mrb_desktop_new(mrb, mrb_desktop_malloc(mrb, hdesk));
}

mrb_value
mrb_desktop_thread(mrb_state* mrb, mrb_value self)
{
    HDESK desk = GetThreadDesktop(GetCurrentThreadId());
    if(desk==NULL)
        mrb_raise(mrb, E_RUNTIME_ERROR, "can't get desktop handle");

    return mrb_desktop_new_alloc(mrb, desk);
}

mrb_value
mrb_desktop_windows(mrb_state* mrb, mrb_value self)
{
    mrb_value ary;
    ary = mrb_ary_new(mrb);
    {
      void* p[] = { NULL, mrb, NULL, &ary };
      {
        mrb_value block;
        mrb_get_args(mrb, "|&", &block);
        p[0] = &block;
      }
      {
        mrb_value w;
        w = winapi_window_new_alloc(mrb, NULL);
        p[2] = &w;
      }
      EnumDesktopWindows(DESK(self), EnumWindowsProc, (LPARAM)p);
    }
    return ary;
}
//

// ------------------------
void
mrb_mruby_winapi_desktop_gem_init(mrb_state* mrb)
{
    struct RClass* dc;
    dc = mrb_define_class(mrb, "Desktop", mrb->object_class);
    MRB_SET_INSTANCE_TT(dc, MRB_TT_DATA);
    mrb_define_class_method(mrb, dc, "current", mrb_desktop_thread, ARGS_NONE());
    
    mrb_define_method(mrb, dc, "windows", mrb_desktop_windows, ARGS_OPT(1));
}

void
mrb_mruby_winapi_desktop_gem_final(mrb_state* mrb)
{
}
