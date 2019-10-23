#include <stdio.h>

struct sresult {
    char const* start;
    int length;
    char const *tag;
    char errc;
    unsigned char metadata; // bit 0: stopword
};
extern void __nlex_root(struct sresult*);
extern void __nlex_feed(char const *p);

int main() {
    struct sresult res = {0};
    size_t size = 1024;
    char *s = malloc(size);
    printf("res at %p, s at %p\n", &res, s);
    while(1) {
      char *last = -1;
      printf("> ");
      size_t els = getline(&s, &size, stdin);
      s[els-1] = 0;
      printf("processing - '%s'\n", s);
      __nlex_feed(s);
      while (1) {
        __nlex_root(&res);
        printf("%smatch {'%.*s' %d %s} is%sa stopword\n", (res.errc?"no ":""), res.length, res.start, res.length, res.tag, (res.metadata&1?" ":" not "));
        if (res.errc || res.length == 0)
          break;
        last = res.start;
      }
    }
    free(s);
}
