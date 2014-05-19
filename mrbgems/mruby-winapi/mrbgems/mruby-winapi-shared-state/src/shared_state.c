#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <mruby.h>

HANDLE hProcess __attribute__((section("shared"), shared )) = 0;

static void*
allocf_shared(mrb_state *mrb, void *p, size_t size, void *ud)
{
  if (size == 0) {
    VirtualFreeEx(
      hProcess,
      p,
      0,
      MEM_RELEASE
      );
    return NULL;
  }
  else {
    return
      VirtualAllocEx(
        hProcess,
        p,
        size,
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
        );
  }
}

HANDLE
get_process_handle(char* exefile)
{
  HANDLE hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hthSnapshot == INVALID_HANDLE_VALUE) {
    return -1;
  }
	
  PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
  BOOL bProcess = Process32First(hthSnapshot, &pe);
  for (; bProcess; bProcess = Process32Next(hthSnapshot, &pe)) {
    if(strcmp(pe.szExeFile, exefile)==0) {
      CloseHandle(hthSnapshot);
      return pe.th32ProcessID;
    }
  }

  CloseHandle(hthSnapshot);
  return 0;
}

mrb_state*
mrb_open_shared(HANDLE process)
{
  hProcess = process;
  return mrb_open_allocf(allocf_shared, NULL);
}

