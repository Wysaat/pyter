typedef struct pyslice pyslice;
struct pyslice {
    int type;
    pyint *start;
    pyint *stop;
    pyint *step;
};