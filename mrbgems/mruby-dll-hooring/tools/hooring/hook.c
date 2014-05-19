#include <mruby.h>
#include <mruby/hash.h>

#include <mruby/winapi.h>

#define PROC(type) LRESULT CALLBACK type(int nCode, WPARAM wp, LPARAM lp)

extern mrb_state* mrb;

PROC(CallWndProc);
PROC(CallWndRetProc);
PROC(CBTProc);
PROC(DebugProc);
PROC(ForegroundIdleProc);
PROC(GetMsgProc);
PROC(JournalPlaybackProc);
PROC(JournalRecordProc);
PROC(KeyboardProc);
PROC(LowLevelKeyboardProc);
PROC(MouseProc);
PROC(LowLevelMouseProc);
PROC(MessageProc);
PROC(ShellProc);
PROC(SysMsgProc);

#ifndef  LLKHF_INJECTED
#define LLKHF_INJECTED 0x00000010
#endif
#ifndef  LLMHF_INJECTED
#define LLMHF_INJECTED 0x00000001
#endif


#define unhookHandle(handle) do { if(h##handle!=NULL) { UnhookWindowsHookEx(h##handle); h##handle = NULL; } } while(FALSE)

HHOOK hCallWnd;
HHOOK hCallWndRet;
HHOOK hCbt;
HHOOK hDebug;
HHOOK hForegroundIdle;
HHOOK hGetMessage;
HHOOK hJournalPlayBack;
HHOOK hJournalRecord;
HHOOK hKeyboard;
HHOOK hKeyBoardLL;
HHOOK hMouse;
HHOOK hMouseLL;
HHOOK hMessageFilter;
HHOOK hShell;
HHOOK hSystemMessageFilter;

void forward(char*, WPARAM, LPARAM);

void
hook()
{
  HINSTANCE hInst = GetModuleHandle("hooring");
  hCallWnd = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hInst, 0);
  hCallWndRet = SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndRetProc, hInst, 0);
  /* hCbt = SetWindowsHookEx(WH_CBT, CBTProc, hInst, 0); */
  /* hDebug = SetWindowsHookEx(WH_DEBUG, DebugProc, hInst, 0); */
  /* hForegroundIdle = SetWindowsHookEx(WH_FOREGROUNDIDLE, ForegroundIdleProc, hInst, 0); */
  hGetMessage = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInst, 0);
  /*     hJournalPlayBack = SetWindowsHookEx(WH_JOURNALPLAYBACK, JournalPlaybackProc, hInst, 0); */
  /*     hJournalRecord = SetWindowsHookEx(WH_JOURNALRECORD, JournalRecordProc, hInst, 0); */
  /* hKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInst, 0); */
  hKeyBoardLL = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
  /* hMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, hInst, 0); */
  hMouseLL = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);
  /* hMessageFilter = SetWindowsHookEx(WH_MSGFILTER, MessageProc, hInst, 0); */
  hShell = SetWindowsHookEx(WH_SHELL, ShellProc, hInst, 0);
  /* hSystemMessageFilter = SetWindowsHookEx(WH_SYSMSGFILTER, SysMsgProc, hInst, 0); */
}

void
unhook()
{
  unhookHandle(CallWnd);
  unhookHandle(CallWndRet);
  unhookHandle(Cbt);
  unhookHandle(Debug);
  unhookHandle(ForegroundIdle);
  unhookHandle(GetMessage);
  unhookHandle(JournalPlayBack);
  unhookHandle(JournalRecord);
  unhookHandle(Keyboard);
  unhookHandle(KeyBoardLL);
  unhookHandle(Mouse);
  unhookHandle(MouseLL);
  unhookHandle(MessageFilter);
  unhookHandle(Shell);
  unhookHandle(SystemMessageFilter);
}

PROC(CallWndProc){//
  CWPSTRUCT *msg = (CWPSTRUCT*)lp;
  switch (msg->message) {
    /* case WM_IME_STARTCOMPOSITION: */
    /*     forward("start_composition", wp, lp); */
    /*     break; */
    /* case WM_IME_ENDCOMPOSITION: */
    /*     forward("end_composition", wp, lp); */
    /*     break; */
  case WM_SETFOCUS:
    if (msg->hwnd == GetForegroundWindow()) {
      forward("set_forcus", wp, lp);
    }
    break;
  case WM_SETCURSOR:
    forward("set_cursor", wp, lp);
    break;
  case WM_NCLBUTTONDOWN:
    forward("nc_lbutton_down", wp, lp);
    break;
  case WM_NCLBUTTONUP:
    forward("nc_lbutton_up", wp, lp);
    break;
  case WM_MOVING:
    forward("moving", wp, lp);
    break;
  case WM_ENTERSIZEMOVE:
    forward("enter_size_move", wp, lp);
    break;
  case WM_EXITSIZEMOVE:
    forward("exit_size_move", wp, lp);
    break;
  case WM_NCACTIVATE:
    if (msg->wParam && msg->hwnd == GetForegroundWindow()) {
      forward("nc_activate", wp, lp);
    }
    break;
  }
  return CallNextHookEx(hCallWnd, nCode, wp, lp);
}
PROC(CallWndRetProc){//
  const CWPRETSTRUCT *msg = (CWPRETSTRUCT *)lp;
  switch (msg->message) {
  case WM_SETTEXT:
    if (msg->hwnd == GetForegroundWindow()) {
      forward("set_text", wp, lp);
    }
    break;
  case WM_SETCURSOR:
    forward("set_cursor", wp, lp);
    break;
  case WM_NCLBUTTONDOWN:
    forward("nc_lbutton_down", wp, lp);
    break;
  case WM_NCLBUTTONUP:
    forward("nc_lbutton_up", wp, lp);
    break;
  }
  return CallNextHookEx(hCallWndRet, nCode, wp, lp);
}
PROC(CBTProc){//
  return CallNextHookEx(hCbt, nCode, wp, lp);
}
PROC(DebugProc){//
  return CallNextHookEx(hDebug, nCode, wp, lp);
}
PROC(ForegroundIdleProc){//
  return CallNextHookEx(hForegroundIdle, nCode, wp, lp);
}
PROC(GetMsgProc){//
  if (nCode < 0) {
    return CallNextHookEx(hGetMessage, nCode, wp, lp);
  }

  {
    LPMSG msg = (LPMSG)lp;
    switch (msg->message) {
    case WM_IME_STARTCOMPOSITION:
      forward("start_composition", wp, lp);
      break;
    case WM_IME_ENDCOMPOSITION:
      forward("end_composition", wp, lp);
      break;
    case WM_NCLBUTTONDOWN:
      forward("nc_lbutton_down", wp, lp);
      break;
    case WM_LBUTTONDOWN:
      forward("lbutton_down", wp, lp);
      break;
    case WM_NCLBUTTONUP:
      forward("nc_lbutton_up", wp, lp);
      break;
    }
  }
  return CallNextHookEx(hGetMessage, nCode, wp, lp);
}

PROC(JournalPlaybackProc){
  return CallNextHookEx(hJournalPlayBack, nCode, wp, lp);
}
PROC(JournalRecordProc){
  return CallNextHookEx(hJournalRecord, nCode, wp, lp);
}
PROC(KeyboardProc){
  return CallNextHookEx(hKeyboard, nCode, wp, lp);
}

BOOL in_proc_ll_key = FALSE;

PROC(LowLevelKeyboardProc){
  if(nCode<0) {
    return CallNextHookEx(hKeyBoardLL, nCode, wp, lp);
  }

  if(in_proc_ll_key) {
    return CallNextHookEx(hKeyBoardLL, nCode, wp, lp);
  }

  {
    BOOL r;
    in_proc_ll_key = TRUE;
    /* forward("kll", wp, lp); */
    {
      int i;
      i = mrb_gc_arena_save(mrb);
      {
        mrb_value arg;
        arg = mrb_hash_new(mrb);
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "event"), mrb_cstr_symbol(mrb, "keyhook"));
        {
          KBDLLHOOKSTRUCT* pKey =  (KBDLLHOOKSTRUCT*)lp;
          mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "virtual_key"), mrb_fixnum_value(pKey->vkCode));
          mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "scan_code"), mrb_fixnum_value(pKey->scanCode));
          if(pKey->flags & LLKHF_EXTENDED){
            mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "extended"), mrb_true_value());
          }
          if(pKey->flags & LLKHF_INJECTED){
            mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "injected"), mrb_true_value());
          }
          if(pKey->flags & LLKHF_UP){
            mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "key_up"), mrb_true_value());
          }
          if(pKey->flags & LLKHF_ALTDOWN){
            mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "alt_down"), mrb_true_value());
          }
          /* pKey->flags is represented by LLKHF, send input requires KEYEVENTF, so this code is a bug */
          /* mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "flags"), mrb_fixnum_value(pKey->flags)); */
        }

        {
          mrb_value ret;
          ret = mrb_funcall(
            mrb,
            mrb_obj_value(mrb_module_get(mrb, "Hook")),
            "call",
            1,
            arg);
          r = mrb_bool(ret);
        }
      }
      mrb_gc_arena_restore(mrb, i);
    }
    in_proc_ll_key = FALSE;
    if(r) {
      return CallNextHookEx(hKeyBoardLL, nCode, wp, lp);
    } else {
      /* fprintf(stdout, "hore\n"); */
      return 1;
    }
  }
}
PROC(MouseProc){
  return CallNextHookEx(hMouse, nCode, wp, lp);
}

BOOL in_proc_ll_mouse = FALSE;

PROC(LowLevelMouseProc){
  if(nCode<0) {
    return CallNextHookEx(hMouseLL, nCode, wp, lp);
  }
  {
    BOOL r;
    
    mrb_value arg;
    {
      int i;
      i = mrb_gc_arena_save(mrb);
      arg = mrb_hash_new(mrb);
      if(!in_proc_ll_mouse && wp==WM_MOUSEMOVE) {
        in_proc_ll_mouse = TRUE;
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "event"), mrb_cstr_symbol(mrb, "mouse_move"));
        mrb_funcall(
          mrb,
          mrb_obj_value(mrb_module_get(mrb, "Hook")),
          "call",
          1,
          arg);
        in_proc_ll_mouse = FALSE;
      }        
      mrb_gc_arena_restore(mrb, i);
      return CallNextHookEx(hMouseLL, nCode, wp, lp);

      if(in_proc_ll_mouse) {
        return CallNextHookEx(hMouseLL, nCode, wp, lp);
      }
    
    
      mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "event"), mrb_cstr_symbol(mrb, "mousehook"));

      switch(wp) {
        /* case WM_MOUSEFIRST: */
        /*     mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "first")); */
        /*     break; */
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "left"));
        break;
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "right"));
        break;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "middle"));
        break;
      case WM_XBUTTONDOWN:
      case WM_XBUTTONUP:
      case WM_XBUTTONDBLCLK:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "x"));
        break;
        /* case WM_MOUSELAST: */
        /*     mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "button"), mrb_cstr_symbol(mrb, "last")); */
        /*     break; */
      }
    
      switch(wp) {
      case WM_MOUSEMOVE:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "type"), mrb_cstr_symbol(mrb, "move"));
        break;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
      case WM_XBUTTONDOWN:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "type"), mrb_cstr_symbol(mrb, "down"));
        break;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
      case WM_XBUTTONUP:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "type"), mrb_cstr_symbol(mrb, "up"));
        break;
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "type"), mrb_cstr_symbol(mrb, "double_click"));
        break;
      case WM_MOUSEWHEEL:
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "type"), mrb_cstr_symbol(mrb, "wheel"));
        break;
      }
    
      /* switch(wp) { */
      /* case WM_MOUSEFIRST: */
      /* case WM_MOUSEMOVE: */
      /* case WM_LBUTTONDOWN: */
      /* case WM_LBUTTONUP: */
      /* case WM_LBUTTONDBLCLK: */
      /* case WM_RBUTTONDOWN: */
      /* case WM_RBUTTONUP: */
      /* case WM_RBUTTONDBLCLK: */
      /* case WM_MBUTTONDOWN: */
      /* case WM_MBUTTONUP: */
      /* case WM_MBUTTONDBLCLK: */
      /* case WM_MOUSEWHEEL: */
      /* case WM_XBUTTONDOWN: */
      /* case WM_XBUTTONUP: */
      /* case WM_XBUTTONDBLCLK: */
      /* case WM_MOUSELAST: */
      /* } */

      {
        MSLLHOOKSTRUCT* pMouse =  (MSLLHOOKSTRUCT*)lp;
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "x"), mrb_fixnum_value(pMouse->pt.x));
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "y"), mrb_fixnum_value(pMouse->pt.y));
        mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "mouse_data"), mrb_fixnum_value(pMouse->mouseData));
        if(pMouse->flags & LLMHF_INJECTED){
          mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "injected"), mrb_true_value());
        }
      }
      mrb_gc_arena_restore(mrb, i);
    }

    {
      mrb_value ret = mrb_funcall(
        mrb,
        mrb_obj_value(mrb_module_get(mrb, "Hook")),
        "call",
        1,
        arg);
    
      r = mrb_bool(ret);
    }
    in_proc_ll_mouse = FALSE;
    if(r) {
      return CallNextHookEx(hMouseLL, nCode, wp, lp);
    } else {
      return 1;
    }
  }
}
PROC(MessageProc){//
  return CallNextHookEx(hMessageFilter, nCode, wp, lp);
}
PROC(ShellProc){
  switch(nCode) {
  case HSHELL_WINDOWACTIVATED:
    forward("activated", wp, lp);
  default:
    return CallNextHookEx(hShell, nCode, wp, lp);
  }
}
PROC(SysMsgProc){//
  if(nCode < 0) {
    return CallNextHookEx(hSystemMessageFilter, nCode, wp, lp);
  }
  switch(nCode) {
  case MSGF_DIALOGBOX:
    forward("dialogbox", wp, lp);
    break;
  case MSGF_MENU:
    forward("menu", wp, lp);
    break;
  case MSGF_SCROLLBAR:
    forward("scrollbar", wp, lp);
    break;
  default:
    return CallNextHookEx(hSystemMessageFilter, nCode, wp, lp);
  }
  return CallNextHookEx(hSystemMessageFilter, nCode, wp, lp);
}
