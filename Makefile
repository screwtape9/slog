CC=g++
CXXFLAGS=-std=c++11 -c -g -Wall -Wextra -Wshadow -Wredundant-decls -Wunreachable-code -Winline
INCLUDES=-I/usr/include/libxml2
LDFLAGS=-lxml2 -lncurses

OBJ:=main.o
EXE=slog

COMPILE.1=$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $<
ifeq ($(VERBOSE),)
COMPILE=@printf "  > compiling %s\n" $(<F) && $(COMPILE.1)
else
COMPILE=$(COMPILE.1)
endif

%.o: %.cpp
	$(COMPILE)

.PHONY: all clean rebuild

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(EXE) $(OBJ)

rebuild: clean all
