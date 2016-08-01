EXE = test-code-parser
CATCH = catch
SRC = statements.cpp range.cpp global-setup.cpp
OBJ = $(SRC:.cpp=.o)

CXX    =  g++
DEBUG = -g
CFLAGS = -Wall -std=c++11 $(DEBUG)

# * * * * * Subdirectories * * * * *
SUB_OBJ = $(addprefix exp-parser/, shunting-yard.o packToken.o objects.o functions.o)

MODULES += $(sort $(dir $(SUB_OBJ)))
CFLAGS += $(addprefix -I./, $(MODULES))

# * * * * * Recipes * * * * *

all: $(EXE)

%.o: %.cpp *.h; $(CXX) $(CFLAGS) -c $<

$(EXE): modules $(OBJ) $(SUB_OBJ) $(CATCH).o
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) $(CATCH).o $(EXE).cpp -o $(EXE)

jspy: modules $(OBJ) $(SUB_OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) jspy.cpp -o jspy

run: jspy
	./jspy

test: $(EXE); ./$(EXE)

check: $(EXE); valgrind ./$(EXE)

modules: $(MODULES)
	@echo
	@echo "Checking submodule $<..."
	make -s -C $<
	@echo

clean:; rm -f $(EXE) jspy *.o
