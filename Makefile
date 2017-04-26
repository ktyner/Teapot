# the compiler we are using
CXX = gcc
# some warning generators
CXXFLAGS= -g
# libraries to include
LDFLAGS = -lGL -lGLU -lglut -lm
# the executable to create
EXEC = run

default: $(OBJS)
	$(CXX) $(CXXFLAGS) main.c -o $(EXEC) $(LDFLAGS)

$(EXEC): default
	./$(EXEC)

driver.o: main.c 

clean:
	rm -rf $(EXEC)
