OUT = temp6

all:
	gcc cpyter.c evaluate.c execute.c integer.c string.c list.c \
	gen_evaluate.c gen_execute.c \
	pytype/methods.c \
	pytype/pyint.c pytype/pyfloat.c pytype/pycomplex.c pytype/pystr.c pytype/pybool.c \
	pytype/pylist.c pytype/pytuple.c pytype/pyset.c pytype/pydict.c \
	pytype/pyfunction.c pytype/pyclass.c \
	pytype/py__builtins__.c pytype/pygenerator.c pytype/others.c pytype/pymodule.c \
	environment.c \
	__builtins__.c \
	builtins/str.c \
	scan/scanner.c scan/buffer.c scan/token.c -o $(OUT) -g -lm

clean:
	rm $(OUT)