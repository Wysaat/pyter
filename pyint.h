struct pyint {
    int type;
    integer *value;
};

pyint *pyint__init__();
pyint *pyint__add__(pyint *, pyint *);
pyint *pyint__sub__(pyint *, pyint *);
pyint *pyint__mul__(pyint *, pyint *);
pyint *pyint__div__(pyint *, pyint *);
pyint *pyint__mod__(pyint *, pyint *);
int pyint__cmp__(pyint *, pyint *);