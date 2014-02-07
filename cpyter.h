#define ITEMSIZE 1024
#define STRINGSIZE 1024

#define match(x, y) !strcmp(x, y)

typedef struct {
    char identifier[ITEMSIZE];
} identifier;

identifier *IDENTIFIER(char *item) {
    identifier *retval = (identifier *)malloc(sizeof(identifier));
    strcpy(retval->identifier, item);
    return retval;
}

typedef struct {
	char value[ITEMSIZE];
} pyint;

pyint *PYINT(char *item) {
	pyint *retval = (pyint *)malloc(sizeof(pyint));
	strcpy(retval->value, item);
	return retval;
}

typedef struct {
	char value[ITEMSIZE];
} pystr;

pystr *PYSTR(char *item) {
	pystr *retval = (pystr *)malloc(sizeof(pystr));
	strcpy(retval->value, item);
	return retval;
}

typedef struct {
} tuple;

typedef struct expr_list {
	expression expr;
	struct expr_list *next;
};