CC=g++-10
TARGET = all
SRCS = main.cc loader.cc
OBJS = $(SRCS:.cc=.o)
CPPFLAGS = -g -pthread -O3 -std=c++2a
.SUFFIXES: .cc

all : a.out

a.out: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS)

.cc.o:
	$(CC) $(CPPFLAGS) $< -c -o $@


clean:
	rm -rf *.o a.out core
