EXE = test-code-parser
SRC = $(EXE).cpp code-parser.cpp catch.cpp
OBJ = $(SRC:.cpp=.o)

CXX    =  g++
DEBUG = -g
CFLAGS = -Wall -std=c++11 $(DEBUG)

# * * * * * Subdirectories * * * * *
VPATH=exp-parser
MODULES=exp-parser
CFLAGS += -I./exp-parser
#SUB_OBJ = $(filter-out exp-parser/catch.o $(wildcard exp-parser/*.o))
SUB_OBJ = exp-parser/functions.o exp-parser/packToken.o exp-parser/shunting-yard.o

# * * * * * Recipes * * * * *

all: $(EXE)

%.o: %.cpp *.h; $(CXX) $(CFLAGS) -c $<

$(EXE): $(MODULES) $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) -o $(EXE)

test: $(EXE); ./$(EXE)

$(MODULES):
	make -C $(MODULES)/

clean:; rm -f $(EXE) *.o
