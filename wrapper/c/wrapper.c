#include <stdio.h>

struct sresult
{
    char const *start;
    int length;
    char const *tag;
    char errc;
    unsigned char metadata; // bit 0: stopword
};
extern void __nlex_root(struct sresult *);
extern void __nlex_feed(char const *p);
extern void __nlex_skip();
extern int  __nlex_distance();

#include <stdio.h>
#include <stdlib.h>

char *getf(char *p, char **e)
{
    FILE *f = fopen(p, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET); /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;
    *e = string+fsize;
    return string;
}

char *paths[] = {"/home/Test/shits/sample100150", "/home/Test/shits/sample10001", "/home/Test/shits/sample100034", "/home/Test/shits/sample100176", "/home/Test/shits/sample100156", "/home/Test/shits/sample100141", "/home/Test/shits/sample100100", "/home/Test/shits/sample100035", "/home/Test/shits/sample100107", "/home/Test/shits/sample100070", "/home/Test/shits/sample100149", "/home/Test/shits/sample100198", "/home/Test/shits/sample100053", "/home/Test/shits/sample100029", "/home/Test/shits/sample100025", "/home/Test/shits/sample100055", "/home/Test/shits/sample100041", "/home/Test/shits/sample100169", "/home/Test/shits/sample100006", "/home/Test/shits/sample100030", "/home/Test/shits/sample100186", "/home/Test/shits/sample100046", "/home/Test/shits/sample100207", "/home/Test/shits/sample100205", "/home/Test/shits/sample100047", "/home/Test/shits/sample100042", "/home/Test/shits/sample100140", "/home/Test/shits/sample100", "/home/Test/shits/sample100061", "/home/Test/shits/sample100092", "/home/Test/shits/sample100164", "/home/Test/shits/sample10007", "/home/Test/shits/sample100056", "/home/Test/shits/sample100137", "/home/Test/shits/sample100057", "/home/Test/shits/sample100108", "/home/Test/shits/sample100038", "/home/Test/shits/sample100003", "/home/Test/shits/sample100187", "/home/Test/shits/sample100018", "/home/Test/shits/sample100065", "/home/Test/shits/sample100129", "/home/Test/shits/sample100067", "/home/Test/shits/sample100048", "/home/Test/shits/sample100031", "/home/Test/shits/sample100040", "/home/Test/shits/sample100010", "/home/Test/shits/sample100124", "/home/Test/shits/sample100054", "/home/Test/shits/sample100097", "/home/Test/shits/sample100027", "/home/Test/shits/sample10000", "/home/Test/shits/sample100155", "/home/Test/shits/sample10020", "/home/Test/shits/sample100084", "/home/Test/shits/sample100009", "/home/Test/shits/sample100206", "/home/Test/shits/sample100179", "/home/Test/shits/sample100194", "/home/Test/shits/sample100045", "/home/Test/shits/sample100014", "/home/Test/shits/sample100064", "/home/Test/shits/sample100117", "/home/Test/shits/sample100091", "/home/Test/shits/sample1002", "/home/Test/shits/sample100152", "/home/Test/shits/sample100162", "/home/Test/shits/sample10016", "/home/Test/shits/sample100127", "/home/Test/shits/sample10006", "/home/Test/shits/sample100002", "/home/Test/shits/sample100188", "/home/Test/shits/sample100113", "/home/Test/shits/sample100001", "/home/Test/shits/sample100087", "/home/Test/shits/sample100208", "/home/Test/shits/sample100082", "/home/Test/shits/sample100195", "/home/Test/shits/sample100138", "/home/Test/shits/sample100115", "/home/Test/shits/sample100044", "/home/Test/shits/sample100023", "/home/Test/shits/sample100110", "/home/Test/shits/sample100022", "/home/Test/shits/sample100192", "/home/Test/shits/sample100081", "/home/Test/shits/sample1001", "/home/Test/shits/sample100145", "/home/Test/shits/sample100178", "/home/Test/shits/sample100005", "/home/Test/shits/sample100203", "/home/Test/shits/sample100028", "/home/Test/shits/sample100086", "/home/Test/shits/sample100090", "/home/Test/shits/sample100096", "/home/Test/shits/sample100024", "/home/Test/shits/sample100037", "/home/Test/shits/sample100098", "/home/Test/shits/sample100139", "/home/Test/shits/sample100016", "/home/Test/shits/sample100153", "/home/Test/shits/sample100181", "/home/Test/shits/sample100148", "/home/Test/shits/sample100159", "/home/Test/shits/sample100000", "/home/Test/shits/sample100143", "/home/Test/shits/sample100120", "/home/Test/shits/sample100202", "/home/Test/shits/sample100074", "/home/Test/shits/sample100160", "/home/Test/shits/sample100058", "/home/Test/shits/sample100050", "/home/Test/shits/sample100033", "/home/Test/shits/sample100072", "/home/Test/shits/sample10011", "/home/Test/shits/sample100167", "/home/Test/shits/sample100170", "/home/Test/shits/sample100200", "/home/Test/shits/sample100013", "/home/Test/shits/sample10005", "/home/Test/shits/sample100114", "/home/Test/shits/sample100008", "/home/Test/shits/sample10009", "/home/Test/shits/sample100173", "/home/Test/shits/sample10008", "/home/Test/shits/sample100099", "/home/Test/shits/sample100111", "/home/Test/shits/sample100144", "/home/Test/shits/sample100121", "/home/Test/shits/sample100183", "/home/Test/shits/sample10012", "/home/Test/shits/sample100109", "/home/Test/shits/sample100039", "/home/Test/shits/sample100085", "/home/Test/shits/sample100017", "/home/Test/shits/sample100131", "/home/Test/shits/sample100132", "/home/Test/shits/sample100189", "/home/Test/shits/sample100175", "/home/Test/shits/sample100199", "/home/Test/shits/sample100151", "/home/Test/shits/sample100177", "/home/Test/shits/sample1000", "/home/Test/shits/sample100051", "/home/Test/shits/sample100079", "/home/Test/shits/sample100112", "/home/Test/shits/sample100163", "/home/Test/shits/sample10017", "/home/Test/shits/sample100063", "/home/Test/shits/sample100021", "/home/Test/shits/sample100069", "/home/Test/shits/sample100007", "/home/Test/shits/sample100032", "/home/Test/shits/sample10", "/home/Test/shits/sample100060", "/home/Test/shits/sample100019", "/home/Test/shits/sample100101", "/home/Test/shits/sample100043", "/home/Test/shits/sample100123", "/home/Test/shits/sample10013", "/home/Test/shits/sample10004", "/home/Test/shits/sample100165", "/home/Test/shits/sample100026", "/home/Test/shits/sample100126", "/home/Test/shits/sample100142", "/home/Test/shits/sample10015", "/home/Test/shits/sample10014", "/home/Test/shits/sample100106", "/home/Test/shits/sample100190", "/home/Test/shits/sample100104", "/home/Test/shits/sample100119", "/home/Test/shits/sample100196", "/home/Test/shits/sample100020", "/home/Test/shits/sample100180", "/home/Test/shits/sample100197", "/home/Test/shits/sample100083", "/home/Test/shits/sample100075", "/home/Test/shits/sample10003", "/home/Test/shits/sample100049", "/home/Test/shits/sample100157", "/home/Test/shits/sample100118", "/home/Test/shits/sample100004", "/home/Test/shits/sample1", "/home/Test/shits/sample100168", "/home/Test/shits/sample100105", "/home/Test/shits/sample100134", "/home/Test/shits/sample100076", "/home/Test/shits/sample100166", "/home/Test/shits/sample100068", "/home/Test/shits/sample100182", "/home/Test/shits/sample100172", "/home/Test/shits/sample100052", "/home/Test/shits/sample100133", "/home/Test/shits/sample100185", "/home/Test/shits/sample100136", "/home/Test/shits/sample100077", "/home/Test/shits/sample100062", "/home/Test/shits/sample100088", "/home/Test/shits/sample100094", "/home/Test/shits/sample100147", "/home/Test/shits/sample100103", "/home/Test/shits/sample100071", "/home/Test/shits/sample100089", "/home/Test/shits/sample100193", "/home/Test/shits/sample100128", "/home/Test/shits/sample100093", "/home/Test/shits/sample100130", "/home/Test/shits/sample100059", "/home/Test/shits/sample100125", "/home/Test/shits/sample100204", "/home/Test/shits/sample100191", "/home/Test/shits/sample100158", "/home/Test/shits/sample100184", "/home/Test/shits/sample100174", "/home/Test/shits/sample100209", "/home/Test/shits/sample10010", "/home/Test/shits/sample100146", "/home/Test/shits/sample100011", "/home/Test/shits/sample100012", "/home/Test/shits/sample100036", "/home/Test/shits/sample100161", "/home/Test/shits/sample100122", "/home/Test/shits/sample100015", "/home/Test/shits/sample100171", "/home/Test/shits/sample100078", "/home/Test/shits/sample10002", "/home/Test/shits/sample100066", "/home/Test/shits/sample100095", "/home/Test/shits/sample100135", "/home/Test/shits/sample100201", "/home/Test/shits/sample100102", "/home/Test/shits/sample100073", "/home/Test/shits/sample10018", "/home/Test/shits/sample100116", "/home/Test/shits/sample100080", "/home/Test/shits/sample10019", "/home/Test/shits/sample100154", NULL};
//

// char* paths[] = {"/home/Test/Documents/nlp-lex/wrapper/c/test"};

int main(int argc, char* argv[])
{
  int start_of_paths = 1;
  int json = 0;
    for (int i = 1; i < argc; i++) {
      start_of_paths = i + 1;
      if (strcmp("--", argv[i]) == 0) { start_of_paths = i + 1; break; }
      char *arg = argv[i];
      if (*arg == '-') {
        if (strcmp(arg+1, "-json") == 0) {
          json = 1;
          continue;
        }
        if (strcmp(arg+1, "h") == 0) {
          puts("nlex wrapper interface\n  wrap [options] -- input_file...\n\n[OPTIONS]\n\t--json : generate json output\n\t-h : print this help and exit");
          exit(0);
        }
        printf("Error: unknown argument '%s'", arg);
        exit(1);
      } else {
        start_of_paths = i + 1;
        break;
      }
    }
    int y = 0;
    struct sresult res = {0};
    if (json)
      printf("{\"description\": \"nlex\", \"documents\": [");
    for (char **x = &argv[start_of_paths]; *x; x++) {
      if (json)
        printf("\n\t{\"filename\": \"%s\", \"tokens\": [", *x);
        char *end;
        char *s = getf(*x, &end);
        __nlex_feed(s);
        char *start = s;
        int pos, last_pos = -1;
        int first = 1;
        int id = 0;
      while (1) {
          y++;
          id++;
          if (start>=end) break;
        __nlex_root(&res);
          if (res.length == 0 || (pos=__nlex_distance()) == last_pos) {
              start++;
            __nlex_skip();
            continue;
            }
          last_pos = pos;
        if ((start += res.length) >= end)
            break;
        if (res.errc) {
            break;
        }
        if (json)
          printf("%s\n\t\t{\"id\": %d, \"is_stopword\": %s, \"token\": \"%.*s\", \"type\": \"%s\"}", (first ? "" : ","), id*10, res.metadata&1?"true":"false", res.length, res.start, res.tag);
        else 
        printf("%smatch {'%.*s' %d %s} is%sa stopword\n", (res.errc?"no ":""), res.length, res.start, res.length, res.tag, (res.metadata&1?" ":" not "));
        if (first) first = 0;
      }
        free(s);
        if(json) printf("\n\t]}");
        if (json && x[1]) printf(",");
    }
    if (json) printf("\n]}");
    if (!json)
    printf("\n%d\n", y);
}
