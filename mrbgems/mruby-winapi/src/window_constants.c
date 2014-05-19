#include <mruby.h>
#include <mruby/class.h>

#include <mruby/winapi.h>
#include <mruby/winapi/window.h>

void
winapi_window_constants_init(mrb_state* mrb)
{
  struct RClass* window;
  window = winapi_window_class(mrb);

  {
    struct RClass* display;
    display = mrb_define_module_under(mrb, winapi_window_class(mrb), "Display");
    MRB_SET_INSTANCE_TT(window, MRB_TT_DATA);
    
#define DEF_WIN_DISPLAY_CONST(name) mrb_define_const(mrb, display, #name, mrb_fixnum_value(SW_##name))
    DEF_WIN_DISPLAY_CONST(SHOW);
    DEF_WIN_DISPLAY_CONST(HIDE);
    DEF_WIN_DISPLAY_CONST(RESTORE);
    DEF_WIN_DISPLAY_CONST(MAXIMIZE);
    DEF_WIN_DISPLAY_CONST(MINIMIZE);
#undef DEF_WIN_DISPLAY_CONST
    mrb_include_module(mrb, window, display);
  }

  {
    struct RClass* style;
    style = mrb_define_module_under(mrb, winapi_window_class(mrb), "Style");
    MRB_SET_INSTANCE_TT(style, MRB_TT_DATA);
  
#define DEF_WIN_STYLE_CONST(name) mrb_define_const(mrb, style, #name, mrb_fixnum_value(WS_##name))
    DEF_WIN_STYLE_CONST(OVERLAPPED);
    DEF_WIN_STYLE_CONST(OVERLAPPEDWINDOW);
    DEF_WIN_STYLE_CONST(CHILD);
    DEF_WIN_STYLE_CONST(POPUP);
    DEF_WIN_STYLE_CONST(POPUPWINDOW);
    DEF_WIN_STYLE_CONST(CLIPCHILDREN);
    DEF_WIN_STYLE_CONST(CLIPSIBLINGS);
    DEF_WIN_STYLE_CONST(CAPTION);
    DEF_WIN_STYLE_CONST(SYSMENU);
    DEF_WIN_STYLE_CONST(MINIMIZEBOX);
    DEF_WIN_STYLE_CONST(MAXIMIZEBOX);
    DEF_WIN_STYLE_CONST(BORDER);
    DEF_WIN_STYLE_CONST(DLGFRAME);
    DEF_WIN_STYLE_CONST(THICKFRAME);
    DEF_WIN_STYLE_CONST(HSCROLL);
    DEF_WIN_STYLE_CONST(VSCROLL);
    DEF_WIN_STYLE_CONST(VISIBLE);
    DEF_WIN_STYLE_CONST(DISABLED);
    DEF_WIN_STYLE_CONST(MAXIMIZE);
    DEF_WIN_STYLE_CONST(MINIMIZE);
    DEF_WIN_STYLE_CONST(TABSTOP);
    DEF_WIN_STYLE_CONST(GROUP);
#undef DEF_WIN_STYLE_CONST
    mrb_include_module(mrb, window, style);
  }
  
}
