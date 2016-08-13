EXE = test-code-parser
CATCH = catch
SRC = statements.cpp range.cpp global-setup.cpp matcher.cpp pattern.cpp
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

$(EXE): $(EXE).cpp $(OBJ) $(SUB_OBJ) $(CATCH).o
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) $(CATCH).o $(EXE).cpp -o $(EXE)

jspy: $(OBJ) $(SUB_OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) jspy.cpp -o jspy

run: jspy
	./jspy usage.spy

shell: jspy
	./jspy

test: $(EXE); ./$(EXE)

check: $(EXE); valgrind ./$(EXE)

$(SUB_OBJ):
	@echo
	@echo "Checking submodules..."
	make -s -C $(foreach m, $(MODULES), $(m))
	@echo

clean:; rm -f $(EXE) jspy *.o
