#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

struct sresult {
  char const *start;
  int length;
  char const *tag;
  char errc;
  unsigned char metadata; // bit 0: stopword, bit 1: sentence_delimiter
  char const *pos;
  int allocd;
};
extern void __nlex_root(struct sresult *);
extern void __nlex_feed(char const *p);
extern int __nlex_distance();

// extern void libpredict_tag_sequence(void* predictor, struct sresult* tokens, int length);
// extern void* libpredict_predictor_from_file(char const *filename);
// extern void libpredict_free_predictor(void *predictor);

// might be null
// extern char const __nlex_ptag;
// extern char __nlex_has_tagpos;
// extern char __nlex_tagpos_model_file;

// static void* predictor = NULL;

// void __nlex_postag_sentence(struct sresult *v, int len) {
//   libpredict_tag_sequence(predictor, v, len);
// }

// struct sresult bufferA[1024];
// struct sresult bufferB[1024];

// struct WorkingBuffer {
//   struct sresult *buffer;
//   int size;
//   int start;
//   int end;
//   int ready;
// };

// static struct WorkingBuffer back = {bufferA, 1024, 0, 0, 0};

// static struct WorkingBuffer front = {bufferB, 1024, 0, 0, 0};

// static void swap_buffers() {
//   struct WorkingBuffer buf = back;
//   back = front;
//   front = buf;
// }

// static void resetfront() {
//   front.ready = 0;
//   front.start = 0;
//   front.end = 0;
// }

// static void resetback() {
//   back.ready = 0;
//   back.start = 0;
//   back.end = 0;
// }

// static int done = 0;
// static int fst = 1;
// static int dumpout = 0;
// 
// void __nlex_postag_m(struct sresult *res, char const *ptag) {
//   int pos_fst = 0;
//   if (front.ready) {
//     // front is not depleted but back has a full sentence
//     *res = front.buffer[front.start++];
//     if (front.start >= front.end) {
//       dumpout = 0;
//       resetfront();
//       pos_fst = 1;
//     }
//     if (dumpout)
//       return;
//   } else
//     pos_fst = 1;
//   if (done)
//     return;
//   if (back.ready) {
//     swap_buffers();
//     resetback();
//     libpredict_tag_sequence(predictor, front.buffer+front.start, front.end-front.start);
//     if (front.start < front.end)
//       *res = front.buffer[front.start++];
//     else {
//       front.ready = 0;
//     }
//   } else {
//     struct sresult mres = {0};
//     __nlex_root(&mres);
//     if (mres.errc || mres.length == 0) {
//       // something is fucked up, we finished before we got a complete sentence,
//       // or we ran out of tokens in the stream
//       mres.start = ""; // set it to empty
//       mres.length = 0;
//       done = 1;
//     } else
//       mres.start = strdup(mres.start);
//     struct sresult *p = &back.buffer[back.end++];
//     if (p->allocd) {
//       free(p->start); // free previously allocated crap
//     }
//     *p = mres;
//     if (strcmp(mres.tag, ptag) == 0) {
//       back.ready = 1;
//       mres.metadata |= 2; // sentence_delimiter
//     } else if (pos_fst)
//       fst = 1;
//   }
// }
// 
// void __nlex_postag(struct sresult *res) {
//   if (!__nlex_has_tagpos) {
//     __nlex_root(res);
//     return;
//   }
//   if (fst) {
//     while (!done && !back.ready)
//       __nlex_postag_m(res, &__nlex_ptag);
//     swap_buffers();
//     resetback();
//     libpredict_tag_sequence(predictor, front.buffer+front.start, front.end-front.start);
//     if (front.start < front.end)
//       *res = front.buffer[front.start++];
//     else {
//       front.ready = 0;
//     }
//     fst = 0;
//   } else
//     __nlex_postag_m(res, &__nlex_ptag);
// }

// int __nlex_can_produce_token() {
//   return !done || front.start < front.end;
// }

// static void __nlex_reset_state() {
//   resetback();
//   resetfront();
//   done = 0;
//   fst = 1;
// }

// void __nlex_load_tagpos() {
//   if (__nlex_has_tagpos)
//     predictor = libpredict_predictor_from_file(&__nlex_tagpos_model_file);
// }
// void __nlex_unload_tagpos() {
//   if (__nlex_has_tagpos) {
//     libpredict_free_predictor(predictor);
//     predictor = NULL;
//   }
// }

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

double dprintdf(double fd, double ptr, ...) {
  va_list ap;
  va_start(ap, ptr);

  __intptr_t ip = ptr;
  double res = (double) vdprintf((int) fd, (char*) ip, ap);
  va_end(ap);
  return res;
}

double mallocd(double s) {
  size_t sze = (size_t) s;
  void *mem = malloc(sze);
  __intptr_t ip = (__intptr_t) mem;
  return (double) ip;
}

double memsetd(double p, double s, double n) {
  __intptr_t ip = p;
  ip = (__intptr_t) memset((void*) ip, (int) s, (size_t) n);
  return (double) ip;
}

double freed(double ptr) {
  __intptr_t p = ptr;
  free((void*) p);
  return 0.0;
}

double cderef(double ptr) {
  __intptr_t p = ptr;
  return * ((char*) p);
}

double cderefset(double ptr, double value) {
  __intptr_t p = ptr;
  memmove((char*) p, (void*) &value, sizeof(char));
  return value;
}

double dderef(double ptr) {
  __intptr_t p = ptr;
  return * ((double*) p);
}

double dderefset(double ptr, double value) {
  __intptr_t p = ptr;
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
/* lib code */

int main() {
  struct sresult res = {0};
  // __nlex_load_tagpos();
  size_t size = 1024000;
  char *s = malloc(size);
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
            fprintf(stderr, "Error from read(2): %s", strerror(errno));
            break;
        }
        if (els == 0) break;
    }
    s[els - 1] = 0;
    printf("processing - '%s'\n", s);
    // __nlex_reset_state();
    __nlex_feed(s);
    while (1 /* __nlex_can_produce_token() */) {
      // __nlex_postag(&res);
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
  // __nlex_unload_tagpos();
}

