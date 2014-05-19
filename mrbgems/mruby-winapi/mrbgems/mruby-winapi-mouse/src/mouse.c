#include "mouse.h"

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>

#include <mruby/winapi/window.h>

mrb_value
mrb_mouse_get_capture(mrb_state* mrb, mrb_value self)
{
  HWND wnd = GetCapture();
  return winapi_window_new_alloc(mrb, wnd);
}

mrb_value
mrb_mouse_set_capture(mrb_state* mrb, mrb_value self)
{
  winapi_window* window;
  {
    mrb_value wnd;
    mrb_get_args(mrb, "o", &wnd);
    window = winapi_window_obj(mrb, wnd);
  }
  SetCapture(window->handle);
  return self;
}


/* UINT GetDoubleClickTime(VOID); */

struct mrb_mouse*
mrb_mouse_malloc(mrb_state *mrb)
{
  struct mrb_mouse *mouse;
  mouse = (struct mrb_mouse *)mrb_malloc(mrb, sizeof(*mouse));
  ZeroMemory(&(mouse->mi), sizeof(mouse->mi));
  return mouse;
}

void
mrb_mouse_free(mrb_state *mrb, void *ptr)
{
  mrb_free(mrb, ptr);
}

// ----------------------
// construction
mrb_value
mrb_mouse_new(mrb_state* mrb, struct mrb_mouse* mouse)
{
  return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_get(mrb, "Mouse"), &mrb_mouse_type, mouse));
}

mrb_value
mrb_mouse_new_alloc(mrb_state* mrb, mrb_value self)
{
  return mrb_mouse_new(mrb, mrb_mouse_malloc(mrb));
}

mrb_value
mrb_mouse_set_x(mrb_state* mrb, mrb_value self)
{
  mrb_value x;
  mrb_get_args(mrb, "i", &x);
  MOUSE(self)->mi.dx = mrb_fixnum(x);
  return self;
    
}

mrb_value
mrb_mouse_set_y(mrb_state* mrb, mrb_value self)
{
  mrb_value y;
  mrb_get_args(mrb, "i", &y);
  MOUSE(self)->mi.dy = mrb_fixnum(y);
  return self;
}

mrb_value
mrb_mouse_add_event(mrb_state* mrb, mrb_value self)
{
  mrb_value* events;
  int size;
  mrb_get_args(mrb, "*", &events, &size);
  {
    int i;
    for(i=0; i<size; ++i) {
      MOUSE(self)->mi.dwFlags |= mrb_fixnum(events[i]);
    }
  }
  return self;
}

mrb_value
mrb_mouse_remove_event(mrb_state* mrb, mrb_value self)
{
  mrb_value* events;
  int size;
  int i;

  mrb_get_args(mrb, "*", &events, &size);

  for(i=0; i<size; ++i) {
    MOUSE(self)->mi.dwFlags &= ~mrb_fixnum(events[i]);
  }
    
  return self;
}

mrb_value
mrb_mouse_move(mrb_state* mrb, mrb_value self)
{
  mrb_value p;
  mrb_get_args(mrb, "H", &p);

  {
    mrb_value ret;
    ret = mrb_hash_get(mrb, p, mrb_cstr_symbol(mrb, "x"));
    if(!mrb_nil_p(ret))
      MOUSE(self)->mi.dx = mrb_fixnum(ret);
  }
  {
    mrb_value ret;
    ret = mrb_hash_get(mrb, p, mrb_cstr_symbol(mrb, "y"));
    if(!mrb_nil_p(ret))
      MOUSE(self)->mi.dy = mrb_fixnum(ret);
  }      

  MOUSE(self)->mi.dwFlags |= MOUSEEVENTF_MOVE;

  return self;
}

mrb_value
mrb_mouse_send(mrb_state* mrb, mrb_value self)
{
  mrb_value* mouse;
  int size;
  mrb_get_args(mrb, "*", &mouse, &size);
  {
    INPUT inputs[size];
    {    
      int i;
      for(i=0; i<size; ++i) {
        INPUT input;
        MOUSEINPUT mi;

        input.type = INPUT_MOUSE;
        ZeroMemory(&mi, sizeof(mi));
        mi.dx = MOUSE(mouse[i])->mi.dx;
        mi.dy = MOUSE(mouse[i])->mi.dy;
        mi.mouseData = MOUSE(mouse[i])->mi.mouseData;
        mi.dwFlags = MOUSE(mouse[i])->mi.dwFlags;
        input.mi = mi;
        inputs[i] = input;
        /* fprintf(stdout, "mouse send: %x\n", mi.dwFlags); */
      }
    }
    SendInput(size,inputs,sizeof(INPUT));
  }
  return mrb_nil_value();
}



/* typedef struct tagMOUSEINPUT { */
/*   LONG dx; */
/*   LONG dy; */
/*   DWORD mouseData; */
/*   DWORD dwFlags; */
/*   DWORD time; */
/*   ULONG_PTR dwExtraInfo; */
/* } MOUSEINPUT,*PMOUSEINPUT,*LPMOUSEINPUT; */

/* #define INPUT_MOUSE 0 */

void
mrb_mruby_winapi_mouse_gem_init(mrb_state* mrb)
{

  struct RClass* mc;
    
  mc = mrb_define_class(mrb, "Mouse", mrb->object_class);
  MRB_SET_INSTANCE_TT(mc, MRB_TT_DATA);
  mrb_define_class_method(mrb, mc, "new", mrb_mouse_new_alloc, ARGS_NONE());
  mrb_define_class_method(mrb, mc, "send", mrb_mouse_send, ARGS_REST());

  mrb_define_const(mrb, mc, "MOVE", mrb_fixnum_value(MOUSEEVENTF_MOVE));
  mrb_define_const(mrb, mc, "LEFT_DOWN", mrb_fixnum_value(MOUSEEVENTF_LEFTDOWN));
  mrb_define_const(mrb, mc, "LEFT_UP", mrb_fixnum_value(MOUSEEVENTF_LEFTUP));
  mrb_define_const(mrb, mc, "RIGHT_DOWN", mrb_fixnum_value(MOUSEEVENTF_RIGHTDOWN));
  mrb_define_const(mrb, mc, "RIGHT_UP", mrb_fixnum_value(MOUSEEVENTF_RIGHTUP));
  mrb_define_const(mrb, mc, "MIDDLE_DOWN", mrb_fixnum_value(MOUSEEVENTF_MIDDLEDOWN));
  mrb_define_const(mrb, mc, "MIDDLE_UP", mrb_fixnum_value(MOUSEEVENTF_MIDDLEUP));
  mrb_define_const(mrb, mc, "X_DOWN", mrb_fixnum_value(MOUSEEVENTF_XDOWN));
  mrb_define_const(mrb, mc, "X_UP", mrb_fixnum_value(MOUSEEVENTF_XUP));
  mrb_define_const(mrb, mc, "WHEEL", mrb_fixnum_value(MOUSEEVENTF_WHEEL));
/*     mrb_define_const(mrb, mc, "VIRTUAL_DESK", mrb_fixnum_value(MOUSEEVENTF_VIRTUALDESK)); */
  mrb_define_const(mrb, mc, "ABSOLUTE", mrb_fixnum_value(MOUSEEVENTF_ABSOLUTE));
    
  mrb_define_method(mrb, mc, "move", mrb_mouse_move, ARGS_REQ(1));
  mrb_define_method(mrb, mc, "x=", mrb_mouse_set_x, ARGS_REQ(1));
  mrb_define_method(mrb, mc, "y=", mrb_mouse_set_y, ARGS_REQ(1));
  mrb_define_method(mrb, mc, "add_event", mrb_mouse_add_event, ARGS_REST());
  mrb_define_method(mrb, mc, "remove_event", mrb_mouse_remove_event, ARGS_REST());
    
/* DragDetect */
/* GetCapture */
  mrb_define_class_method(mrb, mc, "capture" , mrb_mouse_get_capture, ARGS_NONE());
/* GetDoubleClickTime */
/* GetMouseMovePointsEx */
/* mouse_event */
/* ReleaseCapture */
/* SetCapture */
  mrb_define_class_method(mrb, mc, "capture=" , mrb_mouse_set_capture, ARGS_REQ(1));
/* SetDoubleClickTime */
/* SwapMouseButton */
/* TrackMouseEvent */
/* _TrackMouseEvent */
}
void
mrb_mruby_winapi_mouse_gem_final(mrb_state* mrb)
{
}

