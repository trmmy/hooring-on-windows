#include <mruby.h>
#include <mruby/winapi.h>

int
mirb_main(int argc, char **argv);

char _getchar();

#define getchar _getchar

void cprintf(char* msg, ...);

#define printf(msg, ...) cprintf(msg) /* TODO */

void cwrite(char* msg, int len);

#define fwrite(msg, len, size, out) cwrite(msg, len)

void cputc(char c);

#define putc(c, out) cputc(c)

void cputs(char* msg);

#define fputs(msg, out) cputs(msg)

#ifndef ENABLE_READLINE
  int last_char;
#endif

mrb_state* the_mrb_state();

#define main(argc, argv) mirb_main(argc, argv)
#define mrb_open() the_mrb_state()
#define mrb_close(mrb)

#include "../mirb/mirb.c"

HANDLE hin;
HANDLE hout;
HANDLE herr;
DWORD f;

char _getchar() {
  char c;
  ReadConsole(hin, &c, 1, &f, NULL);
  return c;
}

void cprintf(char* msg, ...) {
  WriteConsole(hout, msg, strlen(msg), &f, NULL);
}

void cwrite(char* msg, int len) {
  WriteConsole(hout, msg, len, &f, NULL);
}

void cputc(char c) {
  char cs[] = {c, (char)0};
  WriteConsole(hout, cs, sizeof(cs), &f, NULL);
}

void cputs(char* msg) {
  WriteConsole(hout, msg, strlen(msg), &f, NULL);
}

// ignore Ctrl-c
#include <signal.h>
void SigInt(int i)
{
  putc('\n', stdout);
  fputs("> *", stdout);
#ifndef ENABLE_READLINE
  last_char = 0;
#endif
  signal(SIGINT, SigInt);
}

extern mrb_state* mrb;
mrb_state* the_mrb_state()
{
  return mrb;
}

int
mirb(int argc, char **argv)
{

  signal(SIGINT, SigInt);

  hin =     GetStdHandle(STD_INPUT_HANDLE);
  hout =    GetStdHandle(STD_OUTPUT_HANDLE);
  herr =    GetStdHandle(STD_ERROR_HANDLE);

  return mirb_main(argc, argv);
}
