#ifndef PYTYPE_OTHERS_H
#define PYTYPE_OTHERS_H

typedef struct pyslice pyslice;
struct pyslice {
    int type;
    pyint *start;
    pyint *stop;
    pyint *step;
};

#endif /* PYTYPE_OTHERS_H */