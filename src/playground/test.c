#include <stdio.h>

struct sresult {
  char const *start;
  int length;
  char const *tag;
  char errc;
  unsigned char metadata; // bit 0: stopword
  char const *pos;
  int allocd;
};
extern void __nlex_root(struct sresult *);
extern void __nlex_feed(char const *p);

// might be stubbed out 
extern void __nlex_generated_postag();
// might be null
extern char const* __nlex_ptag;
extern char __nlex_has_postag;

struct sresult bufferA[1024];
struct sresult bufferB[1024];

struct WorkingBuffer {
  struct sresult *buffer;
  int size;
  int start;
  int end;
  int ready;
};

static struct WorkingBuffer back = {bufferA, 1024, 0, 0, 0};

static struct WorkingBuffer front = {bufferB, 1024, 0, 0, 0};

static void swap_buffers() {
  struct WorkingBuffer buf = back;
  back = front;
  front = buf;
}

static void resetfront() {
  front.ready = 0;
  front.start = 0;
  front.end = 0;
}

static void resetback() {
  back.ready = 0;
  back.start = 0;
  back.end = 0;
}

static int done = 0;
static int fst = 1;
static int dumpout = 0;

void __nlex_postag_m(struct sresult *res, char const *ptag) {
  int pos_fst = 0;
  if (front.ready) {
    // front is not depleted but back has a full sentence
    *res = front.buffer[front.start++];
    if (front.start >= front.end) {
      dumpout = 0;
      resetfront();
      pos_fst = 1;
    }
    if (dumpout)
      return;
  } else
    pos_fst = 1;
  if (done)
    return;
  if (back.ready) {
    swap_buffers();
    resetback();
    __nlex_generated_postag();
    if (front.start < front.end)
      *res = front.buffer[front.start++];
    else {
      front.ready = 0;
    }
  } else {
    struct sresult mres = {0};
    __nlex_root(&mres);
    if (mres.errc || mres.length == 0) {
      // something is fucked up, we finished before we got a complete sentence,
      // or we ran out of tokens in the stream
      mres.start = ""; // set it to empty
      mres.length = 0;
      done = 1;
    } else
      mres.start = strdup(mres.start);
    struct sresult *p = &back.buffer[back.end++];
    if (p->allocd) {
      free(p->start); // free previously allocated crap
    }
    *p = mres;
    if (strcmp(mres.tag, ptag) == 0)
      back.ready = 1;
    else if (pos_fst)
      fst = 1;
  }
}

static void __nlex_postag(struct sresult *res) {
  if (!__nlex_has_postag) {
    __nlex_root(res);
    return;
  }
  if (fst) {
    while (!done && !back.ready)
      __nlex_postag_m(res, __nlex_ptag);
    swap_buffers();
    resetback();
    __nlex_generated_postag();
    if (front.start < front.end)
      *res = front.buffer[front.start++];
    else {
      front.ready = 0;
    }
    fst = 0;
  } else
    __nlex_postag_m(res, __nlex_ptag);
}

static int __nlex_can_produce_token() {
  return !done || front.start < front.end;
}

static void __nlex_reset_state() {
  resetback();
  resetfront();
  done = 0;
  fst = 1;
}
/* lib code */

int main() {
  struct sresult res = {0};
  size_t size = 1024;
  char *s = malloc(size);
  printf("res at %p, s at %p\n", &res, s);
  while (1) {
    char *last = -1;
    printf("> ");
    size_t els = getline(&s, &size, stdin);
    s[els - 1] = 0;
    printf("processing - '%s'\n", s);
    __nlex_reset_state();
    __nlex_feed(s);
    while (__nlex_can_produce_token()) {
      __nlex_postag(&res);

      printf("%smatch {'%.*s' - %s - %d %s} is%sa stopword\n",
             (res.errc ? "no " : ""), res.length, res.start, res.pos,
             res.length, res.tag, (res.metadata & 1 ? " " : " not "));
      if (res.errc || res.length == 0 || res.start == last)
        break;
      last = res.start;
    }
  }
  free(s);
}
