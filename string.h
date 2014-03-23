#define string char *

string *string_init();
void string_print_nnl(string *);
void string_print(string *);
int string_len(string *);
string *string_add(string *, string *);
int string_eq(string *, string *);
string *string_cpy(string *);
void string_del(string *);
char *string_get(string *val, int ind);
void string_set(string *val, int ind, char *ch);
string *string_slice(string *val, int start, int stop, int step);

/*--------------------------------------------------------------*/

string *string_frchs(char *);
int string_eqchs(string *, char *);
char *string_tochs(string *);