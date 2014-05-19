#define _WIN32_WINNT 0x0600
#include <stdio.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>

#include <mruby/winapi.h>
#include <mruby/winapi/application.h>
#include <mruby/winapi/window.h>
#include <mruby/winapi/message.h>

static struct mrb_data_type winapi_window_type = { "winapi_window", mrb_free };

winapi_window*
winapi_window_malloc(mrb_state *mrb, HWND hwnd)
{
    winapi_window *window;
    window = (winapi_window *)mrb_malloc(mrb, sizeof(*window));
    window->handle = hwnd;
    return window;
}

winapi_window*
winapi_window_obj(mrb_state* mrb, mrb_value self)
{
  return (winapi_window*)mrb_data_get_ptr(mrb, self, &winapi_window_type);
}

struct RClass*
winapi_window_class(mrb_state* mrb)
{
  return
    mrb_class_get_under(
      mrb,
      winapi_class(mrb),
      "Window");

}

mrb_value
winapi_window_class_value(mrb_state* mrb)
{
  return mrb_obj_value(winapi_window_class(mrb));
}

mrb_value
winapi_window_new(mrb_state* mrb, winapi_window* window)
{
  return mrb_obj_value(Data_Wrap_Struct(mrb, winapi_window_class(mrb), &winapi_window_type, window));
}

mrb_value
winapi_window_new_alloc(mrb_state* mrb, HWND hwnd)
{
  return winapi_window_new(mrb, winapi_window_malloc(mrb, hwnd));
}

mrb_value
winapi_window_id(mrb_state* mrb, mrb_value self)
{
  return mrb_fixnum_value((int)winapi_window_handle(mrb, self));
}

mrb_value
winapi_window_destroy(mrb_state* mrb, mrb_value self)
{
  BOOL destroyed = 
    DestroyWindow(
      winapi_window_handle(mrb, self));
  if(destroyed) {
    winapi_window_handle(mrb, self) = NULL;
  }
  return mrb_bool_value(destroyed);
}

mrb_value
winapi_window_move(mrb_state* mrb, mrb_value self)
{
  mrb_value opts;
  mrb_get_args(mrb, "H", &opts);
  if(mrb_nil_p(opts)) {
    return self;
  }
  /* todo */
  
  MoveWindow(
    winapi_window_handle(mrb, self),
    mrb_fixnum(mrb_hash_get(mrb, opts, mrb_cstr_symbol(mrb, "x"))),
    mrb_fixnum(mrb_hash_get(mrb, opts, mrb_cstr_symbol(mrb, "y"))),
    mrb_fixnum(mrb_hash_get(mrb, opts, mrb_cstr_symbol(mrb, "width"))),
    mrb_fixnum(mrb_hash_get(mrb, opts, mrb_cstr_symbol(mrb, "height"))),
    mrb_bool(mrb_hash_get(mrb, opts, mrb_cstr_symbol(mrb, "repaint"))));
  return self;
}

mrb_value
winapi_window_update(mrb_state* mrb, mrb_value self)
{
  UpdateWindow(winapi_window_handle(mrb, self));
  /* todo / returns result of update window ? */
  return mrb_nil_value();
}

mrb_value
winapi_window_display_set(mrb_state* mrb, mrb_value self)
{
  mrb_value display;
  mrb_get_args(mrb, "i", &display);
  if(mrb_nil_p(display)) {
    return self;
  }
  ShowWindow(winapi_window_handle(mrb, self), mrb_fixnum(display));
  return self;
}

static mrb_value
winapi_window_application(mrb_state* mrb, mrb_value self)
{
  return
    winapi_application_new_hinstance(
      mrb,
      (HINSTANCE)GetWindowLongPtr(
        winapi_window_handle(mrb, self),
        GWLP_HINSTANCE));
}

static mrb_value
winapi_get_message(mrb_state* mrb, HWND hwnd)
{
  int min;
  int max;
  mrb_value opt;
  mrb_value block;
  mrb_get_args(mrb, "|H&", &opt, &block);
  if(mrb_nil_p(opt)){
    min = 0;
    max = 0;
  } else if(!mrb_hash_p(opt)) {
    /* block = opt; */
    min = 0;
    max = 0;
  } else {
    {
      mrb_value m = mrb_hash_get(mrb, opt, mrb_cstr_symbol(mrb, "min"));
      if(mrb_nil_p(m)) {
        min = 0;
      } else {
        min = mrb_fixnum(m);
      }
    }
    {
      mrb_value m = mrb_hash_get(mrb, opt, mrb_cstr_symbol(mrb, "max"));
      if(mrb_nil_p(m)) {
        max = 0;
      } else {
        max = mrb_fixnum(m);
      }
    }
  }

  if(mrb_nil_p(block)) {
    MSG* msg = (MSG*)mrb_malloc(mrb, sizeof(MSG));
    int res = GetMessage(msg, hwnd, min, max);
    if(res > 0) {
      return winapi_message_new(mrb, msg);
    }

    /* res == 0 (quit) | res == -1 (error) */
    mrb_free(mrb, msg);
    if(res == -1) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "Error occured while getting message");
    }
    return mrb_nil_value();
  }

  while(TRUE) {
    MSG* msg = (MSG*)mrb_malloc(mrb, sizeof(MSG));
    int res = GetMessage(msg, hwnd, min, max);
    if(res > 0) {
      mrb_yield(mrb, block, winapi_message_new(mrb, msg));
      continue;
    }
    /* res == 0 (quit) | res == -1 (error) */
    mrb_free(mrb, msg);
    if(res == -1) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "Error occured while getting message");
    }
    return mrb_nil_value();
  }
}

mrb_value
winapi_window_get_message_all(mrb_state* mrb, mrb_value self)
{
  return winapi_get_message(mrb, NULL);
}

mrb_value
winapi_window_get_message(mrb_state* mrb, mrb_value self)
{
  return winapi_get_message(mrb, winapi_window_handle(mrb, self));
}

mrb_value
winapi_post_message(mrb_state* mrb, HWND handle)
{
  mrb_value message;
  mrb_get_args(mrb, "i", &message);
  if(mrb_nil_p(message)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Message should not nil");
    return mrb_nil_value();
  }
  /* todo */
  /* return mrb_bool_value(PostMessage(handle, (UINT)mrb_fixnum(message), NULL, NULL)); */
  return mrb_nil_value();
}

mrb_value
winapi_window_post_message_all(mrb_state* mrb, mrb_value self)
{
  return winapi_post_message(mrb, HWND_BROADCAST);
}

mrb_value
winapi_window_post_message(mrb_state* mrb, mrb_value self)
{
  return winapi_post_message(mrb, winapi_window_handle(mrb, self));
}
/* send_message */
/* post_message(immediate) */
/* HWND_BROADCAST */
//
void winapi_window_constants_init(mrb_state*);

void
winapi_window_init(mrb_state* mrb)
{
  struct RClass* window;
  window = mrb_define_class_under(mrb, winapi_class(mrb), "Window", mrb->object_class);
  MRB_SET_INSTANCE_TT(window, MRB_TT_DATA);

  mrb_define_method(mrb, window, "id",   winapi_window_id,   ARGS_NONE());
  mrb_define_method(mrb, window, "destroy",   winapi_window_destroy,   ARGS_NONE());
  mrb_define_method(mrb, window, "application",   winapi_window_application,   ARGS_NONE());
  mrb_define_method(mrb, window, "move",   winapi_window_move,   ARGS_REQ(1));
  mrb_define_method(mrb, window, "update", winapi_window_update, ARGS_NONE());
  mrb_define_method(mrb, window, "display=", winapi_window_display_set, ARGS_REQ(1));

  mrb_define_class_method(mrb, window, "post_message", winapi_window_post_message_all, ARGS_REQ(1) | ARGS_BLOCK());
  mrb_define_class_method(mrb, window, "get_message", winapi_window_get_message_all, ARGS_OPT(1) | ARGS_BLOCK());
  mrb_define_method(mrb, window, "post_message", winapi_window_post_message, ARGS_REQ(1) | ARGS_BLOCK());
  mrb_define_method(mrb, window, "get_message", winapi_window_get_message, ARGS_OPT(1) | ARGS_BLOCK());

  winapi_window_constants_init(mrb);
}
