# Define the source and object files for the executable
SRC     = $(wildcard *.cpp)
OBJ 		= $(SRC:.cpp=.o)
EXE 		= $(SRC:.cpp=)
LDFLAGS 		= -L./algorithm -lglut -lGL -lGLU -lGLEW
# set up extra flags for explicitly setting mode
debug:      CFLAGS    += -g -O0
release:    CFLAGS    += -O0

#
# Link all the objects into an executable.
all:    $(OBJ)
	$(CXX) -o main.out $(CFLAGS) $(LDFLAGS) $(SRC) $(LOADLIBES) $(LDLIBS)

# $(EXE)		$(OBJ)
#
# Though both modes just do a normal build.
debug:      all
release:    all

clean:
	rm $(OBJ)
