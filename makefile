CC = cc
LINK = gcc
CCFLAGS = -O -I.
LINKFLAGS = -lusb-1.0

%.o: %.c
	$(CC) -c $(CCFLAGS) -o $@ $<

all: tools

tools: noolitepc nooliterx nooliterxcfg

noolitepc: noolitepc.o
	$(LINK) -o $@ $< $(LINKFLAGS)

nooliterx: nooliterx.o
	$(LINK) -o $@ $< $(LINKFLAGS)

nooliterxcfg: nooliterxcfg.o
	$(LINK) -o $@ $< $(LINKFLAGS)

clean:
	/bin/rm -f *.o *~ *.obj noolitepc nooliterx nooliterxcfg

install:
	@echo You must be root to install
