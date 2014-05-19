#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/hash.h>
#include <mruby/string.h>

#include <mruby/winapi.h>
/* #include <mruby/winapi/shared_state.h> */
#include <mruby/winapi/application.h>
#include <mruby/winapi/window.h>
#include <mruby/winapi/message.h>

#include <stdio.h>
#include <stdlib.h>


void hook();
void unhook();
int mirb(int argc, char **argv);

#define INVOKING    40000
#define REQUIRE     40001
#define TRAY_NOTIFY 30000
mrb_state* mrb;
HWND mainwnd __attribute__((section("shared"), shared )) = 0;
NOTIFYICONDATA nIcon;

void
forward(char* message, WPARAM wp, LPARAM lp) {
  PostMessage(mainwnd, RegisterWindowMessage(message), wp, lp);
}

void force_mrb_open()
{
  mrb = mrb_open();
}

void force_mrb_close()
{
  mrb_close(mrb);
}

BOOL WINAPI DllMain(HINSTANCE handle, DWORD dwReason, LPVOID lpReserved)
{
  /*   switch(dwReason) { */
  /*   case DLL_PROCESS_ATTACH: */
  /*   break; */
  /*   case DLL_THREAD_ATTACH: */
  /*   break; */
  /*   case DLL_THREAD_DETACH: */
  /*   break; */
  /*   case DLL_PROCESS_DETACH: */
  /*   break; */
  /*   } */
  return TRUE;
}


DWORD WINAPI console_thread(LPVOID hWnd) {
  /* Thread.new(&method(:console)) ?? */
  FreeConsole();
  AllocConsole();
  {
    char* argv[] = { "mirb" };
    mirb(1, argv);
  }
  FreeConsole();
  ExitThread(TRUE);
}

typedef void (*Interceptor)(mrb_value*, WPARAM, LPARAM);

void window_activated(mrb_value* arg, WPARAM wp, LPARAM lp)
{
  mrb_hash_set(mrb, *arg, mrb_cstr_symbol(mrb, "window"), winapi_window_new_alloc(mrb, (HWND)wp));
}

int send_event(char* event, WPARAM wp, LPARAM lp, Interceptor interceptor)
{
  mrb_value ret;
  {
    int i;
    i = mrb_gc_arena_save(mrb);
    {
      mrb_value arg;
      arg = mrb_hash_new(mrb);
      mrb_hash_set(mrb, arg, mrb_cstr_symbol(mrb, "event"), mrb_cstr_symbol(mrb, event));
      if(interceptor != NULL) {
        interceptor(&arg, wp, lp);
      }
      ret = mrb_funcall(mrb, mrb_obj_value(mrb_module_get(mrb, "Hook")), "call", 1, arg);
    }
    mrb_gc_arena_restore(mrb, i);
  }
  return mrb_bool(ret);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  HANDLE hConsole = NULL;
  DWORD pConsole;
  
  switch(msg) {
  case WM_CREATE:
    mainwnd = hWnd;
    {
      nIcon.cbSize = sizeof(NOTIFYICONDATA) ;
      nIcon.uID = 26000;
      nIcon.hWnd = hWnd ;
      nIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP ;
      nIcon.hIcon = LoadIcon(GetModuleHandle(NULL),"icon.ico");
      nIcon.uCallbackMessage = TRAY_NOTIFY ;
        
      lstrcpy(nIcon.szTip,"MENU");
        
      Shell_NotifyIcon(NIM_ADD, &nIcon);
    }
    /* hConsole = CreateThread(NULL , 0 , console_thread , hWnd , 0 , &pConsole); */
    hook();
    send_event("created", wp, lp, NULL);
    ShowWindow(hWnd,SW_HIDE);
    PostMessage(hWnd, winapi_message_get_registered(mrb, "activated"), (WPARAM)GetForegroundWindow(), (LPARAM)0);
    break;
  case WM_DESTROY:
    send_event("destroy", wp, lp, NULL);
    unhook();
    /* CloseHandle(hConsole); */
    PostQuitMessage(0);
    break;
  case WM_COPYDATA:
    if(INVOKING==(int)wp) {
      mrb_load_string(mrb, (char*)((COPYDATASTRUCT*)lp)->lpData);
    }
    if(REQUIRE==(int)wp) {
      FILE* fp;
      if((fp=fopen((char*)((COPYDATASTRUCT*)lp)->lpData, "r"))==NULL){
        break;
      }
      mrb_load_file(mrb, fp);
      fclose(fp);
    }
    break;
  case TRAY_NOTIFY:
  {
    const int wmTray=lp;
    switch(wmTray) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
      hConsole = CreateThread(NULL , 0 , console_thread , hWnd , 0 , &pConsole);
      /* if(IsWindowVisible(hWnd)) { */
      /*   ShowWindow(hWnd,SW_HIDE); */
      /* } else { */
      /*   ShowWindowAsync(hWnd,SW_SHOWNORMAL); */
      /*   SetForegroundWindow(hWnd); */
      /* } */
      break;
    case WM_RBUTTONDOWN:
    {
#if 0
      reset, console, window, exit ..?
#endif
      /* POINT pos; */
      /* GetCursorPos(&pos); */
            
      /* TrackPopupMenu( */
      /*     GetSubMenu( */
      /*         LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDP_TRAY)) */
      /*             ,0), */
      /*     TPM_LEFTALIGN|TPM_RIGHTBUTTON, */
      /*     pos.x+2, pos.y+2, */
      /*     0, */
      /*     hWnd, */
      /*     NULL); */
      break;
    }
    }
  }
  }
    
  if(msg == winapi_message_get_registered(mrb, "start_composition")) {
    send_event("ime_on", wp, lp, NULL);
    return DefWindowProc(hWnd, msg, wp, lp);
  }
    
  if(msg == winapi_message_get_registered(mrb, "end_composition")) {
    send_event("ime_off", wp, lp, NULL);
    return DefWindowProc(hWnd, msg, wp, lp);
  }
    
  if(msg == winapi_message_get_registered(mrb, "enter_size_move")) {
    send_event("move_start", wp, lp, NULL);
    return DefWindowProc(hWnd, msg, wp, lp);
  }
    
  if(msg == winapi_message_get_registered(mrb, "exit_size_move")) {
    send_event("move_end", wp, lp, NULL);
    return DefWindowProc(hWnd, msg, wp, lp);
  }
    
  if(msg == winapi_message_get_registered(mrb, "activated")) {
    send_event("window_activated", wp, lp, window_activated);
    return DefWindowProc(hWnd, msg, wp, lp);
  }
    
  return DefWindowProc(hWnd, msg, wp, lp);
}

#include <windows.h>
#include <Tlhelp32.h>

HANDLE
get_process_handle(char* exefile)
{
  HANDLE hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hthSnapshot == INVALID_HANDLE_VALUE) {
    return (HANDLE)-1;
  }

  {
    PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
    BOOL bProcess = Process32First(hthSnapshot, &pe);
    for (; bProcess; bProcess = Process32Next(hthSnapshot, &pe)) {
      if(strcmp(pe.szExeFile, exefile)==0) {
        CloseHandle(hthSnapshot);
        return (HANDLE)pe.th32ProcessID;
      }
    }
  }

  CloseHandle(hthSnapshot);
  return 0;
}

HWND GetWindowHandle(const DWORD TargetID)
{
  HWND hWnd = GetTopWindow(NULL);
  do {
    DWORD ProcessID;
    GetWindowThreadProcessId( hWnd, &ProcessID);
    if(TargetID == ProcessID) {
      return hWnd;
    }
  } while((hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

  return NULL;
}

int send_other_process(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
  if(!(__argc > 2)) {
    return -1;
  }
  {
    HANDLE existing = get_process_handle("hooring.exe");
    if(existing == 0) {
      // some message
      return -1;
    }

    {
      HWND hWnd = GetWindowHandle((DWORD)existing);
      {
        COPYDATASTRUCT cd;

        char buffer[strlen(__argv[2])+1];
        strcpy(buffer, __argv[2]);

        cd.dwData = 0;
        cd.cbData = (DWORD)strlen(__argv[2])+1;
        cd.lpData = buffer;

        {
          WPARAM wp;
          if(strcmp(__argv[1], "--exec") == 0) {
            wp = (WPARAM)REQUIRE;
          } else if(strcmp(__argv[1], "--send")) {
            wp = (WPARAM)INVOKING;
          } else {
            wp = (WPARAM)0;
          }
          SendMessage(hWnd, WM_COPYDATA, wp, (LPARAM)&cd);
        }
      }
    }
  }
  return 0;
}

int gui(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
  if(__argc > 2 && strcmp(__argv[1], "--send") == 0) {
    return send_other_process(hInst, hPrevInst, pCmdLine, showCmd);
  }
  if(__argc > 2 && strcmp(__argv[1], "--exec") == 0) {
    return send_other_process(hInst, hPrevInst, pCmdLine, showCmd);
  }
  
  if(GetLastError()==ERROR_ALREADY_EXISTS){
    MessageBox(NULL, "Already exists", "Info", MB_OK);
    return 0;
  }

  {
    int res;
    HANDLE hMutex=CreateMutex(NULL,TRUE,"MENU");

    mrb = mrb_open();
    if(__argc > 2 && strcmp(__argv[1], "-s") == 0) {
      int i;
      FILE* fp;
      for(i=2; i<__argc; i++) {
        if((fp=fopen(__argv[i], "r"))==NULL){
          continue;
        }
        mrb_load_file(mrb, fp);
        fclose(fp);
      }
    }
    res = mrb_fixnum(
      mrb_funcall(
        mrb,
        winapi_application_new_hinstance(mrb, hInst),
        "start", 0));
    mrb_close(mrb);

    ReleaseMutex(hMutex);
    return res;
  }
}

// gcc requires main function
int
main(int argc, char** argv)
{
  return 0; 
}
