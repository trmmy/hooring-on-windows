#include "cursor.h"
#include <mruby/hash.h>
#include <mruby/array.h>
#include <mruby/string.h>

#include <mruby/winapi/point.h>
#include <mruby/winapi/rect.h>

static mrb_value
mrb_cursor_clip(mrb_state *mrb, mrb_value self)
{
  /*
    rect := { :top => top, :left => left, :bottom => bottom, :right => right }
  */
  RECT rect;
  GetWindowRect(GetDesktopWindow(), &rect);
  {
    mrb_value r;
    mrb_get_args(mrb, "|H", &r);

    if(!mrb_hash_p(r)) {
      return self; /* must raise error */
    }

    mrb_hash_rect_update(mrb, r, &rect);
  }
  ClipCursor(&rect);

  return self;
}

static mrb_value
mrb_cursor_get_pos(mrb_state *mrb, mrb_value self)
{
  POINT point;
  GetCursorPos(&point);
  return mrb_point_hash_new(mrb, &point);
}

static mrb_value
mrb_cursor_set_pos(mrb_state *mrb, mrb_value self)
{
  mrb_value p;
  mrb_get_args(mrb, "H", &p);

  if(!mrb_hash_p(p)) {
    p = mrb_hash_new(mrb); /* must raise error */
  }

  {
    POINT point;
    GetCursorPos(&point);
    mrb_hash_point_update(mrb, p, &point);
    SetCursorPos(point.x, point.y);
  }
  return p;
}

static mrb_value
mrb_cursor_show(mrb_state *mrb, mrb_value self)
{
    mrb_bool b;
    mrb_get_args(mrb, "b", &b);
    /* ShowCursor(b); */
    ShowCursor(b?TRUE:FALSE);
    return mrb_bool_value(b);
}

// ---------------
void
mrb_mruby_winapi_cursor_gem_init(mrb_state* mrb)
{
    struct RClass* cc;
  
    cc = mrb_define_class(mrb, "Cursor", mrb->object_class);
    MRB_SET_INSTANCE_TT(cc, MRB_TT_DATA);
  
/* ClipCursor */
    mrb_define_class_method(mrb, cc, "clip" , mrb_cursor_clip, ARGS_OPT(1));
/* CopyCursor */
/* CreateCursor */
/* DestroyCursor */
/* GetClipCursor */
/* GetCursor */
/* GetCursorInfo */
/* GetCursorPos */
  mrb_define_class_method(mrb, cc, "pos" , mrb_cursor_get_pos, ARGS_NONE());
/* LoadCursor */
/* LoadCursorFromFile */
/* SetCursor */
/* SetCursorPos */
  mrb_define_class_method(mrb, cc, "move" , mrb_cursor_set_pos, ARGS_OPT(1));
/* SetSystemCursor */
/* ShowCursor */
  mrb_define_class_method(mrb, cc, "show" , mrb_cursor_show, ARGS_REQ(1));

 /* CreateCaret(HWND hWnd,HBITMAP hBitmap,int nWidth,int nHeight); */
 /* GetCaretBlinkTime(VOID); */
 /* SetCaretBlinkTime(UINT uMSeconds); */
 /* DestroyCaret(VOID); */
 /* HideCaret(HWND hWnd); */
 /* ShowCaret(HWND hWnd); */
 /* SetCaretPos(int X,int Y); */
 /* GetCaretPos(LPPOINT lpPoint); */
 /* ClientToScreen(HWND hWnd,LPPOINT lpPoint); */
 /* ScreenToClient(HWND hWnd,LPPOINT lpPoint); */
  
}

void
mrb_mruby_winapi_cursor_gem_final(mrb_state* mrb)
{
}
