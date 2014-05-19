#include <mruby/winapi.h>
#include <stdlib.h>
#include <stdio.h>
int gui(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd);
int mruby(int argc, char **argv);

#ifdef USE_MIRB
int mirb(int argc, char **argv);
void force_mrb_open();
void force_mrb_close();
#endif

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst /* according to MSDN, be NULL in Win32 Application */, LPSTR pCmdLine, int showCmd)
{
  if(__argc == 1) {
    return gui(hInst, hPrevInst, pCmdLine, showCmd);
  } else if(strcmp(__argv[1], "-s") == 0) { /* in case of 'hooring -s <setting file>' */
    return gui(hInst, hPrevInst, pCmdLine, showCmd);
  } else if(strcmp(__argv[1], "--send") == 0) {
    return gui(hInst, hPrevInst, pCmdLine, showCmd);
  } else if(strcmp(__argv[1], "--exec") == 0) {
    return gui(hInst, hPrevInst, pCmdLine, showCmd);
#ifdef USE_MIRB
  } else if(strcmp(__argv[1], "-i") == 0) {
    int ret;
    force_mrb_open();
    ret = mirb(__argc, __argv);
    force_mrb_close();
    return ret;
#endif
  } else {
    return mruby(__argc, __argv);
  }
}

