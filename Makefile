OUT = temp3

all:
	gcc cpyter.c evaluate.c execute.c integer.c string.c list.c \
	pytype/methods.c \
	pytype/pyint.c pytype/pyfloat.c pytype/pycomplex.c pytype/pystr.c pytype/pybool.c \
	pytype/pylist.c pytype/pytuple.c \
	environment.c \
	scan/scanner.c scan/buffer.c scan/token.c -o $(OUT) -g -lm

clean:
	rm $(OUT)