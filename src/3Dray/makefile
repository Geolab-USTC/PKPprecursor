
LIB = -lm 
LIB = -L../getpar/lib  -lget -lm
BIN = ../../bin/
CC  = cc 

all: findpatch globalave

findpatch: findpatch.c
	gcc -g -DPC findpatch.c -o $(BIN)findpatch \
	grid.c Spe_Cart.c delta.c rwsac.c open_file.c $(LIB)

globalave: globalave.c
	gcc -g globalave.c -o $(BIN)globalave \
	open_file.c $(LIB)

