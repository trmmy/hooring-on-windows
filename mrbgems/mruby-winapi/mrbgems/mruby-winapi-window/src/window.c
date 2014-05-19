#include <mruby.h>
#include <mruby/class.h>

#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/string.h>

#include <mruby/winapi.h>
#include <mruby/winapi/window.h>
#include <mruby/winapi/point.h>
#include <mruby/winapi/rect.h>

mrb_value
winapi_window_get_obj(mrb_state* mrb, HWND hwnd)
{
  return winapi_window_new_alloc(mrb, hwnd);
}

mrb_value
winapi_window_is_visible(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(IsWindowVisible(winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_is_enebled(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(IsWindowEnabled(winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_is_minimized(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(IsIconic(winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_is_maximized(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(IsZoomed(winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_is_window(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(IsWindow(winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_owner(mrb_state* mrb, mrb_value self)
{
  HWND h = GetWindow(winapi_window_handle(mrb, self), GW_OWNER);
  if(h==NULL)
    return mrb_nil_value();
  else
    return winapi_window_new_alloc(mrb, h);
}

mrb_value
winapi_window_is_owner(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(GetWindow(winapi_window_handle(mrb, self), GW_OWNER)==NULL);
}

mrb_value
winapi_window_get_parent(mrb_state* mrb, mrb_value self)
{
  return winapi_window_new_alloc(mrb, GetAncestor(winapi_window_handle(mrb, self), GA_PARENT));
}

mrb_value
winapi_window_get_ancestor(mrb_state* mrb, mrb_value self)
{
  return winapi_window_new_alloc(mrb, GetAncestor(winapi_window_handle(mrb, self), GA_ROOT));
}

mrb_value
winapi_window_get_root(mrb_state* mrb, mrb_value self)
{
  return winapi_window_new_alloc(mrb, GetAncestor(winapi_window_handle(mrb, self), GA_ROOTOWNER));
}

mrb_value
winapi_window_get_title(mrb_state* mrb, mrb_value self)
{
  TCHAR szTitle[1024];
  GetWindowText( winapi_window_handle(mrb, self), szTitle, sizeof(szTitle) );
  return mrb_str_new_cstr(mrb, szTitle);
}

mrb_value
winapi_window_set_title(mrb_state* mrb, mrb_value self)
{
  mrb_value str;
  mrb_get_args(mrb, "S", &str);
  SetWindowText( winapi_window_handle(mrb, self), RSTRING_PTR(str));
  return self;
}

mrb_value
winapi_window_get_class_name(mrb_state* mrb, mrb_value self)
{
  TCHAR szClass[1024];
  GetClassName( winapi_window_handle(mrb, self), szClass, sizeof(szClass) );
  return mrb_str_new_cstr(mrb, szClass);
}


mrb_value
winapi_window_activate(mrb_state* mrb, mrb_value self)
{
  SetActiveWindow(winapi_window_handle(mrb, self));
  return self;
}

mrb_value
winapi_window_set_foreground(mrb_state* mrb, mrb_value self)
{
  SetForegroundWindow(winapi_window_handle(mrb, self));
  return self;
}

// layer

mrb_value
winapi_window_set_alpha(mrb_state* mrb, mrb_value self)
{
  {
    LONG exStyle = GetWindowLong( winapi_window_handle(mrb, self), GWL_EXSTYLE );
    SetWindowLong( winapi_window_handle(mrb, self), GWL_EXSTYLE, exStyle | WS_EX_LAYERED );
  }
  {
    mrb_value a;
    mrb_get_args(mrb, "i", &a);
    {
      byte alpha;
      alpha = mrb_fixnum(a) > 100 ? 100 : mrb_fixnum(a);
      SetLayeredWindowAttributes(winapi_window_handle(mrb, self), (COLORREF)0, (byte)(alpha * 255 / 100), LWA_ALPHA);
    }
  }
  return self;
}

mrb_value
winapi_window_get_alpha(mrb_state* mrb, mrb_value self)
{
  LONG exStyle = GetWindowLong( winapi_window_handle(mrb, self), GWL_EXSTYLE );
  if(! (exStyle & WS_EX_LAYERED)) {
    return mrb_nil_value();
  }
  {
    byte alpha;
    GetLayeredWindowAttributes(winapi_window_handle(mrb, self), NULL, &alpha, NULL);
    return mrb_fixnum_value((byte)(alpha * 100 / 255));
  }
}

mrb_value
winapi_window_set_transparency_color(mrb_state* mrb, mrb_value self)
{
  {
    LONG exStyle = GetWindowLong( winapi_window_handle(mrb, self), GWL_EXSTYLE );
    SetWindowLong( winapi_window_handle(mrb, self), GWL_EXSTYLE, exStyle | WS_EX_LAYERED );
  }
  {
    mrb_value a;
    mrb_get_args(mrb, "i", &a);
    {
      byte alpha = mrb_fixnum(a) > 100 ? 100 : mrb_fixnum(a);
      SetLayeredWindowAttributes(winapi_window_handle(mrb, self), (COLORREF)0, (byte)(alpha * 255 / 100), LWA_ALPHA);
    }
  }
  return self;
}

mrb_value
winapi_window_get_transparency_color(mrb_state* mrb, mrb_value self) {
  return mrb_nil_value();
}

// enum

BOOL CALLBACK EnumWindowsProc( HWND hWnd, LPARAM lParam )
{
  mrb_value *proc =(mrb_value*)((void**)lParam)[0];
  mrb_state* mrb = (mrb_state*)((void**)lParam)[1];
  mrb_value *w  = (mrb_value*)((void**)lParam)[2];
  mrb_value *ary  = (mrb_value*)((void**)lParam)[3];
  winapi_window_handle(mrb, *w) = hWnd;
  if(mrb_nil_p(*proc) || mrb_bool(mrb_yield(mrb, *proc, *w)))
  {
    int i = mrb_gc_arena_save(mrb);
    mrb_ary_push(mrb, *ary, winapi_window_new_alloc(mrb, hWnd));
    mrb_gc_arena_restore(mrb, i);
  }
  return TRUE;
}


mrb_value
winapi_window_find(mrb_state* mrb, mrb_value self)
{
  mrb_value ary;
  ary = mrb_ary_new(mrb);
  {
    mrb_value block;
    mrb_value w;
    void* p[4];

    {
      mrb_get_args(mrb, "|&", &block);
      p[0] = &block;
    }
    {
      p[1] = mrb;
    }
    {
      w = winapi_window_new_alloc(mrb, NULL);
      p[2] = &w;
    }
    {
      p[3] = &ary;
    }
    EnumWindows(EnumWindowsProc, (LPARAM)p);
  }
  return ary;
}

mrb_value
winapi_window_find_children(mrb_state* mrb, mrb_value self)
{
  mrb_value ary;
  ary = mrb_ary_new(mrb);
  {
    mrb_value block;
    mrb_value w;
    void* p[4];
    {
      mrb_get_args(mrb, "|&", &block);
      p[0] = &block;
    }
    {
      p[1] = mrb;
    }
    {
      w = winapi_window_new_alloc(mrb, NULL);
      p[2] = &w;
    }
    {
      p[3] = &ary;
    }
    EnumChildWindows(winapi_window_handle(mrb, self), EnumWindowsProc, (LPARAM)p);
  }
  return ary;
}

mrb_value
winapi_window_sibling_previous(mrb_state* mrb, mrb_value self)
{
  return winapi_window_new_alloc(mrb, GetNextWindow( winapi_window_handle(mrb, self), GW_HWNDPREV ));
}

mrb_value
winapi_window_sibling_next(mrb_state* mrb, mrb_value self)
{
  return winapi_window_new_alloc(mrb, GetNextWindow( winapi_window_handle(mrb, self), GW_HWNDNEXT ));
}

mrb_value
winapi_window_is_child(mrb_state* mrb, mrb_value self)
{
  mrb_value parent;
  mrb_get_args(mrb, "o", &parent);
  return mrb_bool_value(IsChild(winapi_window_handle(mrb, parent), winapi_window_handle(mrb, self)));
}

mrb_value
winapi_window_set_parent(mrb_state* mrb, mrb_value self)
{
  mrb_value parent;
  mrb_get_args(mrb, "o", &parent);
  if(mrb_nil_p(parent))
    SetParent(winapi_window_handle(mrb, self), NULL);
  else
    SetParent(winapi_window_handle(mrb, self), winapi_window_handle(mrb, parent));
  return self;
}

mrb_value
winapi_window_iconize(mrb_state* mrb, mrb_value self)
{
  {
    NOTIFYICONDATA nIcon;
    nIcon.cbSize = sizeof(NOTIFYICONDATA) ;
    nIcon.uID = 1;
    nIcon.hWnd = winapi_window_handle(mrb, self);
    nIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP ;
    nIcon.hIcon = NULL;
    nIcon.uCallbackMessage = (UINT)0;
    {
      mrb_value str;
      mrb_get_args(mrb, "|S", &str);
      if(mrb_nil_p(str)) 
        lstrcpy(nIcon.szTip,RSTRING_PTR(winapi_window_get_title(mrb, self)));
      else
        lstrcpy(nIcon.szTip,RSTRING_PTR(str));
    }
    Shell_NotifyIcon( NIM_ADD, &nIcon);
  }
  return self;
}

mrb_value
winapi_window_tool_on(mrb_state* mrb, mrb_value self)
{
  long style = GetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE);
  SetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE, style | WS_EX_TOOLWINDOW);
  return self;
}

mrb_value
winapi_window_tool_off(mrb_state* mrb, mrb_value self)
{
  long style = GetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE);
  SetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE, style & ~WS_EX_TOOLWINDOW);
  return self;
}

mrb_value
winapi_window_add_style(mrb_state* mrb, mrb_value self)
{
  mrb_value s;
  long style;
  mrb_get_args(mrb, "i", &s);
  style = GetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE);
  SetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE, style | mrb_fixnum(s));

  return self;
}

mrb_value
winapi_window_remove_style(mrb_state* mrb, mrb_value self)
{
  mrb_value s;
  long style;
  mrb_get_args(mrb, "i", &s);
  style = GetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE);
  SetWindowLongPtr(winapi_window_handle(mrb, self), GWL_EXSTYLE, style & ~mrb_fixnum(s));

  return self;
}

mrb_value
winapi_window_get_client_rect(mrb_state* mrb, mrb_value self)
{
  RECT r;
  GetClientRect(winapi_window_handle(mrb, self), &r);
  return mrb_rect_hash_new(mrb, &r);
}

mrb_value
winapi_window_get_window_rect(mrb_state* mrb, mrb_value self)
{
  RECT r;
  GetWindowRect(winapi_window_handle(mrb, self), &r);
  return mrb_rect_hash_new(mrb, &r);
}


/* mrb_value */
/* winapi_window_flash(mrb_state* mrb, mrb_value self) */
/* { */
/* FLASHWINFO info; */
/* info.cbSize = sizeof(info); */
/* info.hwnd = hWnd; */
/* info.dwFlags = FLASHW_ALL; */
/* info.uCount = 5; */
/* info.dwTimeout = 0; */
/* FlashWindowEx( &info ); */

/* FLASHW_CAPTION	ウィンドウのキャプションを点滅させる */
/* FLASHW_TRAY	タスクバーのボタンを点滅させる */
/* FLASHW_ALL	FLASHW_CAPTION | FLASHW_TRAYと同じ */
/* FLASHW_STOP	点滅を停止させて、元の状態に戻す */
/* FLASHW_TIMER	FLASHW_STOPで停止させるまで、点滅を続ける */
/* FLASHW_TIMERNOFG	ウィンドウがフォアグラウンドになるまで、点滅を続ける */
/* } */

mrb_value
winapi_window_from_point(mrb_state* mrb, mrb_value self)
{
    
  POINT point;
  GetCursorPos(&point);
  {    
    mrb_value p;
    mrb_get_args(mrb, "|H", &p);
    mrb_hash_point_update(mrb, p, &point);
  }
  {
    HWND wnd;
    wnd = WindowFromPoint(point);
    if(wnd==NULL)
      return mrb_nil_value();
    else
      return winapi_window_new_alloc(mrb, wnd);
  }
}


mrb_value
winapi_window_get_foreground(mrb_state* mrb, mrb_value self)
{
  HWND hwnd;
  hwnd = GetForegroundWindow();
  if(hwnd==NULL)
    return mrb_nil_value();
  else
    return winapi_window_new_alloc(mrb, hwnd);
}

// ------------------------
void
mrb_mruby_winapi_window_gem_init(mrb_state* mrb)
{
  struct RClass* wc;
  wc = winapi_window_class(mrb);
  /* mrb_define_class_method(mrb, wc, "new", winapi_window_new_alloc, ARGS_NONE()); */
  mrb_define_class_method(mrb, wc, "current", winapi_window_get_foreground, ARGS_NONE());
  mrb_define_class_method(mrb, wc, "at", winapi_window_from_point, ARGS_OPT(1));
  mrb_define_class_method(mrb, wc, "find", winapi_window_find, ARGS_OPT(1));

  // status
  mrb_define_method(mrb, wc, "visible?", winapi_window_is_visible, ARGS_NONE());
  mrb_define_method(mrb, wc, "minimized?", winapi_window_is_minimized, ARGS_NONE());
  mrb_define_method(mrb, wc, "maximized?", winapi_window_is_maximized, ARGS_NONE());
  mrb_define_method(mrb, wc, "window?", winapi_window_is_window, ARGS_NONE());
  mrb_define_method(mrb, wc, "owner?", winapi_window_is_owner, ARGS_NONE());

  //
  mrb_define_method(mrb, wc, "activate", winapi_window_activate, ARGS_NONE());
  mrb_define_method(mrb, wc, "fore", winapi_window_set_foreground, ARGS_NONE());
    
  // layered
  mrb_define_method(mrb, wc, "alpha=", winapi_window_set_alpha, ARGS_REQ(1));
  mrb_define_method(mrb, wc, "alpha", winapi_window_get_alpha, ARGS_REQ(0));

  // window
  mrb_define_method(mrb, wc, "owner", winapi_window_owner, ARGS_NONE());
  mrb_define_method(mrb, wc, "parent", winapi_window_get_parent, ARGS_NONE());
  mrb_define_method(mrb, wc, "root", winapi_window_get_parent, ARGS_NONE());
  mrb_define_method(mrb, wc, "children", winapi_window_find_children, ARGS_OPT(1));
  mrb_define_method(mrb, wc, "next", winapi_window_sibling_next, ARGS_NONE());
  mrb_define_method(mrb, wc, "previous", winapi_window_sibling_previous, ARGS_NONE());
  mrb_define_method(mrb, wc, "ancestor", winapi_window_get_ancestor, ARGS_NONE());
  mrb_define_method(mrb, wc, "descendent_of?", winapi_window_is_child, ARGS_REQ(1));
  mrb_define_method(mrb, wc, "child_of?", winapi_window_is_child, ARGS_REQ(1));
  mrb_define_method(mrb, wc, "size", winapi_window_get_client_rect, ARGS_NONE());
  mrb_define_method(mrb, wc, "position", winapi_window_get_window_rect, ARGS_NONE());
    
  mrb_define_method(mrb, wc, "iconize", winapi_window_iconize, ARGS_OPT(1));
  mrb_define_method(mrb, wc, "title", winapi_window_get_title, ARGS_NONE());
  mrb_define_method(mrb, wc, "title=", winapi_window_set_title, ARGS_REQ(1));
  mrb_define_method(mrb, wc, "class_name", winapi_window_get_class_name, ARGS_NONE());

  mrb_define_method(mrb, wc, "toolon", winapi_window_tool_on, ARGS_NONE());
  mrb_define_method(mrb, wc, "tooloff", winapi_window_tool_off, ARGS_NONE());
    
  mrb_define_method(mrb, wc, "add_style", winapi_window_add_style, ARGS_REQ(1));
  mrb_define_method(mrb, wc, "remove_style", winapi_window_remove_style, ARGS_REQ(1));

  /* MapWindowPoints(HWND hWndFrom,HWND hWndTo,LPPOINT lpPoints,UINT cPoints); */
  /* WindowFromPoint(POINT Point); */
  /* ChildWindowFromPoint(HWND hWndParent,POINT Point); */

}

void
mrb_mruby_winapi_window_gem_final(mrb_state* mrb)
{
}
/*
  WINUSERAPI int WINAPI ShowCursor(WINBOOL bShow);
  WINUSERAPI WINBOOL WINAPI SetCursorPos(int X,int Y);
  WINUSERAPI HCURSOR WINAPI SetCursor(HCURSOR hCursor);
  WINUSERAPI WINBOOL WINAPI GetCursorPos(LPPOINT lpPoint);
  WINUSERAPI WINBOOL WINAPI ClipCursor(CONST RECT *lpRect);
  WINUSERAPI WINBOOL WINAPI GetClipCursor(LPRECT lpRect);
  WINUSERAPI HCURSOR WINAPI GetCursor(VOID);
  WINUSERAPI WINBOOL WINAPI CreateCaret(HWND hWnd,HBITMAP hBitmap,int nWidth,int nHeight);
  WINUSERAPI UINT WINAPI GetCaretBlinkTime(VOID);
  WINUSERAPI WINBOOL WINAPI SetCaretBlinkTime(UINT uMSeconds);
  WINUSERAPI WINBOOL WINAPI DestroyCaret(VOID);
  WINUSERAPI WINBOOL WINAPI HideCaret(HWND hWnd);
  WINUSERAPI WINBOOL WINAPI ShowCaret(HWND hWnd);
  WINUSERAPI WINBOOL WINAPI SetCaretPos(int X,int Y);
  WINUSERAPI WINBOOL WINAPI GetCaretPos(LPPOINT lpPoint);
  WINUSERAPI WINBOOL WINAPI ClientToScreen(HWND hWnd,LPPOINT lpPoint);
  WINUSERAPI WINBOOL WINAPI ScreenToClient(HWND hWnd,LPPOINT lpPoint);
  WINUSERAPI int WINAPI MapWindowPoints(HWND hWndFrom,HWND hWndTo,LPPOINT lpPoints,UINT cPoints);
  WINUSERAPI HWND WINAPI WindowFromPoint(POINT Point);
  WINUSERAPI HWND WINAPI ChildWindowFromPoint(HWND hWndParent,POINT Point);

  #define CWP_ALL 0x0000
  #define CWP_SKIPINVISIBLE 0x0001
  #define CWP_SKIPDISABLED 0x0002
  #define CWP_SKIPTRANSPARENT 0x0004

  WINUSERAPI HWND WINAPI ChildWindowFromPointEx(HWND hwnd,POINT pt,UINT flags);

  WINUSERAPI WINBOOL WINAPI ShowOwnedPopups(HWND hWnd,WINBOOL fShow);
  WINUSERAPI WINBOOL WINAPI OpenIcon(HWND hWnd);
  WINUSERAPI WINBOOL WINAPI CloseWindow(HWND hWnd);
  WINUSERAPI WINBOOL WINAPI MoveWindow(HWND hWnd,int X,int Y,int nWidth,int nHeight,WINBOOL bRepaint);
  WINUSERAPI WINBOOL WINAPI SetWindowPos(HWND hWnd,HWND hWndInsertAfter,int X,int Y,int cx,int cy,UINT uFlags);
  WINUSERAPI WINBOOL WINAPI GetWindowPlacement(HWND hWnd,WINDOWPLACEMENT *lpwndpl);
  WINUSERAPI WINBOOL WINAPI SetWindowPlacement(HWND hWnd,CONST WINDOWPLACEMENT *lpwndpl);

  #define SWP_NOSIZE 0x0001
  #define SWP_NOMOVE 0x0002
  #define SWP_NOZORDER 0x0004
  #define SWP_NOREDRAW 0x0008
  #define SWP_NOACTIVATE 0x0010
  #define SWP_FRAMECHANGED 0x0020
  #define SWP_SHOWWINDOW 0x0040
  #define SWP_HIDEWINDOW 0x0080
  #define SWP_NOCOPYBITS 0x0100
  #define SWP_NOOWNERZORDER 0x0200
  #define SWP_NOSENDCHANGING 0x0400

  #define SWP_DRAWFRAME SWP_FRAMECHANGED
  #define SWP_NOREPOSITION SWP_NOOWNERZORDER
  #define SWP_DEFERERASE 0x2000
  #define SWP_ASYNCWINDOWPOS 0x4000

*/
