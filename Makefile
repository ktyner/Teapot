# the compiler we are using
CXX = gcc -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -L/usr/lib64 -O0
# some warning generators
CXXFLAGS= -g
# libraries to include
LDFLAGS = -lX11 -lGL -lGLU -lGLEW -lglut -lm -lXmu -lXi
# the executable to create
EXEC = run

default: $(OBJS)
	$(CXX) $(CXXFLAGS) main.c -o $(EXEC) $(LDFLAGS)

$(EXEC): default
	./$(EXEC)

driver.o: main.c 

clean:
	rm -rf $(EXEC)
