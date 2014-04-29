EXEC=loader
PKGS=`pkg-config --cflags --libs sdl2 gl capnp`

.PHONY: all clean

all: $(EXEC)

$(EXEC): main.cpp model.h gorgon.capnp.h gorgon.capnp.cpp
	g++ -pipe -Wall -Wextra -g -pedantic -std=c++11 $(PKGS) main.cpp gorgon.capnp.cpp -o $(EXEC)

clean:
	rm -f $(EXEC)
