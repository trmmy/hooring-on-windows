#include <mruby.h>
#include <mruby/class.h>

#include <mruby/winapi.h>
#include <mruby/winapi/hook.h>

/*
  Hook.handle message_type
  
  Hook#handle(message, &proc)
  hooks = WinAPI::Hook.new

  gm = WinAPI::Hook::Handler.new
  hooks.get_message = gm
  hooks.get_message << gm

  hooks.get_message do
    handle(WinAPI::Hook::FooBar) do |msg|
    end
  end

  hooks.get_message do |gm|
    gm.handle(WinAPI::Hook::FooBar, NEXT | BREAK | RETURN | PREPEND | UNSHIFT | SHIFT) do |msg|
    end
  end

  hooks.get_message(WinAPI::Hook::FooBar) do |msg|
  end

 */

typedef LPMSG mrb_winapi_hook_get_message_message;

/* BroadcastSystemMessage */
/* DispatchMessage */
/* GetInputState */
/* GetMessage */
/* GetMessageExtraInfo */
/* GetMessagePos */
/* GetMessageTime */
/* GetQueueStatus */
/* InSendMessage */
/* InSendMessageEx */
/* PeekMessage */
/* PostAppMessage */
/* PostMessage */
/* PostQuitMessage */
/* PostThreadMessage */
/* RegisterWindowMessage */
/* ReplyMessage */
/* SendMessage */
/* SendMessageCallback */
/* SendMessageTimeout */
/* SendNotifyMessage */
/* SendAsyncProc */
/* SetMessageExtraInfo */
/* SetMessageQueue */
/* TranslateMessage */
/* WaitMessage */

/* void */
/* forward(char* message, WPARAM wp, LPARAM lp) { */
/*   PostMessage(mainwnd, RegisterWindowMessage(message), wp, lp); */
/* } */

/* hGetMessage = SetWindowsHookEx(WH_GETMESSAGE, proc, GetModuleHandle(NULL), 0); */
/* PROC(GetMsgProc){// */
/*   LPMSG msg = (LPMSG)lp; */
/*   switch (msg->message) { */
/*   case WM_IME_STARTCOMPOSITION: */
/*     forward("start_composition", wp, lp); */
/*     break; */
/*   case WM_IME_ENDCOMPOSITION: */
/*     forward("end_composition", wp, lp); */
/*     break; */
/*   case WM_NCLBUTTONDOWN: */
/*     forward("nc_lbutton_down", wp, lp); */
/*     break; */
/*   case WM_LBUTTONDOWN: */
/*     forward("lbutton_down", wp, lp); */
/*     break; */
/*   case WM_NCLBUTTONUP: */
/*     forward("nc_lbutton_up", wp, lp); */
/*     break; */
/*   } */
/* } */


void
mrb_mruby_winapi_hook_get_message_gem_init(mrb_state* mrb)
{
  struct RClass* get_message =
    mrb_define_class_under(
      mrb, winapi_hook_class(mrb), "GetMessage", mrb->object_class);
  MRB_SET_INSTANCE_TT(get_message, MRB_TT_DATA);
}

void
mrb_mruby_winapi_hook_get_message_gem_final(mrb_state* mrb)
{
}
