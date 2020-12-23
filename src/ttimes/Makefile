#FFLAGS=-O -xlibmil -fsimple

BIN    = ../../bin

progs = remodl setbrn ttimes tcurve

all: $(progs) clean

remodl: remodl.o libtau.o emiasp91.o libsun.o
	$(LINK.f) -o $(BIN)/remodl remodl.o libtau.o emiasp91.o libsun.o
setbrn: setbrn.o libtau.o libsun.o
	$(LINK.f) -o $(BIN)/setbrn setbrn.o libtau.o libsun.o
ttimes: ttimes.o libtau.o libsun.o
	$(LINK.f) -o $(BIN)/ttimes ttimes.o libtau.o libsun.o
tcurve: tcurve.o libtau.o libsun.o
	$(LINK.f) -o $(BIN)/tcurve tcurve.o libtau.o libsun.o

clean:
	-rm -f $(progs) *.o core *.lis
