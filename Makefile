.PHONY: all clean

all: example/gorgon.capnp.h
	$(MAKE) -C example

example/gorgon.capnp.h: gorgon/gorgon.capnp
	capnp compile --src-prefix=gorgon/ -oc++:example gorgon/gorgon.capnp
	mv example/gorgon.capnp.c++ example/gorgon.capnp.cpp

clean:
	rm -f example/gorgon.capnp.h
	rm -f example/gorgon.capnp.cpp
	$(MAKE) -C example clean
