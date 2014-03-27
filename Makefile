OUT = temp3

all:
	gcc cpyter.c execute.c integer.c string.c list.c \
	pytype/pyint.c pytype/pystr.c pytype/pybool.c pytype/pylist.c pytype/pytuple.c \
	scan/scanner.c scan/buffer.c scan/token.c -o $(OUT)

clean:
	rm $(OUT)