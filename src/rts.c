#include <stdio.h>
#include <memory.h>
#include "unistd.h"
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

extern void __nlex_root(struct sresult *);
extern void __nlex_feed(char const *p);
extern int __nlex_distance();


struct sresult {
  char const *start;
  int length;
  char const *tag;
  char errc;
  unsigned char metadata; // bit 0: stopword, bit 1: sentence_delimiter
  char const *pos;
  int allocd;
};

/* kaleidoscope rts */
double putchard(double d) {
  char c = (char) d;
  int x = fprintf(stderr, "%c", c);
  return (double) x;
}

double eputchard(double d) {
  char c = (char) d;
  int x = fprintf(stderr, "%c", c);
  return (double) x;
}

double printd(double d) {
  return (double) printf("%lf", d);
}

double eprintd(double d) {
  return (double) fprintf(stderr, "%lf", d);
}

int vdprintf(int fd, char* fmt, va_list ap) {
	FILE* fp = fdopen(fd, "w");
	int res = vfprintf(fp, fmt, ap);
	fclose(fp);
	return res;
}

double dprintdf(double fd, double ptr, ...) {
  va_list ap;
  va_start(ap, ptr);

  intptr_t ip = ptr;
  double res = (double) vdprintf((int) fd, (char*) ip, ap);
  va_end(ap);
  return res;
}

double mallocd(double s) {
  size_t sze = (size_t) s;
  void *mem = malloc(sze);
  intptr_t ip = (intptr_t) mem;
  return (double) ip;
}

double memsetd(double p, double s, double n) {
  intptr_t ip = p;
  ip = (intptr_t) memset((void*) ip, (int) s, (size_t) n);
  return (double) ip;
}

double freed(double ptr) {
  intptr_t p = ptr;
  free((void*) p);
  return 0.0;
}

double cderef(double ptr) {
  intptr_t p = ptr;
  return * ((char*) p);
}

double cderefset(double ptr, double value) {
  intptr_t p = ptr;
  memmove((char*) p, (void*) &value, sizeof(char));
  return value;
}

double dderef(double ptr) {
  intptr_t p = ptr;
  return * ((double*) p);
}

double dderefset(double ptr, double value) {
  intptr_t p = ptr;
  memmove((double*) p, (void*) &value, sizeof(double));
  return value;
}

double trap() { 
  __builtin_trap();
  return 0;
}

static int metadata = 0;

double repl_testmetadata(double bit) {
  int ibit = bit;
  return (double) !!(metadata&(1<<ibit));
}

extern int __nlex_utf8_length(char c);
/* debugger code */
static int m_stack_idx = 0;
void __nlex_produce_debug(int action, const char* position, const char* data, char const* name) {
  char *act;
  switch(action) {
    case 0: act = "shift"; break;
    case 1: act = "fail"; break;
    case 2: act = "succeed"; break;
    case 3: act = "jump"; break;
    case 4: act = "backtrack"; break;
    case 5:
      act = "recurse";
      ++m_stack_idx;
    break;
    case 6:
      act = "finish";
      --m_stack_idx;
    break;
    default: act = "?"; break;
  }
  printf("[DEBUG] [REC %d] [%.*s](%#x) %s in %p from regex [%s] :: %p\n", m_stack_idx, 1+__nlex_utf8_length(*position), position, *position, act, position, name, data);
}

/* lib code */

int main() {
  struct sresult res = {0};
  size_t size = 1024000;
  char *s = (char*) malloc(size);
  printf("res at %p, s at %p\n", &res, s);
  while (1) {
    int last = -1;
    size_t els = 0;
    if (isatty(STDIN_FILENO)) {
        printf("> ");
        els = getline(&s, &size, stdin);
    } else {
        els = read(STDIN_FILENO, s, 1024000);
        if (els < 0) {
            fprintf(stderr, "Error from read(2)");
            break;
        }
        if (els == 0) break;
    }
    s[els - 1] = 0;
    printf("processing - '%s'\n", s);
    __nlex_feed(s);
    while (1) {
      __nlex_root(&res);

      printf("%smatch {'%.*s' - %s - %d %s} is%sa stopword\n",
             (res.errc ? "no " : ""), res.length, res.start, res.pos,
             res.length, res.tag, (res.metadata & 1 ? " " : " not "));
      metadata = res.metadata;
      int dist = __nlex_distance();
      if (res.errc || res.length == 0)
        break;
      last = dist;
    }
  }
  free(s);
}

