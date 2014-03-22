#define string char *

string *string_init();
string *string_add(string *, string *);
int string_eq(string *, string *);
string *string_cpy(string *);
void *string_del(string *);
char *string_ssci(string *val, int ind);
string *string_slice(string *val, int start, int stop, int step);

/*--------------------------------------------------------------*/

string string_frchs(char *);
int string_eqchs(string *, char *);
char *string_tochs(string *);