PROG=main
OBJDIR=.obj
CC=g++

CFLAGS = -Wall --std=c++17 `pkg-config fuse3 --cflags` -I.. -DPRE350
LDFLAGS = `pkg-config fuse3 --libs`

$(shell mkdir -p $(OBJDIR)) 

OBJS = $(OBJDIR)/filesystemobject.o $(OBJDIR)/file.o $(OBJDIR)/directory.o $(OBJDIR)/io.o 
MAINOBJS = $(OBJS) $(OBJDIR)/main.o
TESTOBJS = $(OBJS) $(OBJDIR)/test.o

$(PROG) : $(MAINOBJS)
	$(CC) $(MAINOBJS) $(LDFLAGS) -o $(PROG)

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $(OBJDIR)/$*.o

clean:
	rm -rf $(PROG) $(OBJDIR)

test: $(OBJS) $(TESTOBJS) 
	$(CC) $(TESTOBJS) $(LDFLAGS) -o run_tests

format:
	clang-format -i *.h *.cpp