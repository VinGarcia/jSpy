TEST_BIN = Test_bin
TEST_SOURCE = $(addprefix tests/, language.cpp arrayClass.cpp blockClass.cpp strClass.cpp objectClass.cpp)
CATCH = catch
SRC = statements.cpp range.cpp global-setup.cpp matcher.cpp pattern.cpp
OBJ = $(SRC:.cpp=.o)

CXX    =  g++
DEBUG = -g
CFLAGS = -Wall -std=c++11 $(DEBUG)

# * * * * * Subdirectories * * * * *
SUB_OBJ = $(addprefix cparse/, core-shunting-yard.o builtin-features.o)

MODULES += $(sort $(dir $(SUB_OBJ)))
CFLAGS += $(addprefix -I./, $(MODULES))

# * * * * * Recipes * * * * *

all: jspy $(TEST_BIN)

%.o: %.cpp *.h; $(CXX) $(CFLAGS) -c $<

$(TEST_BIN): $(TEST_SOURCE) $(OBJ) $(SUB_OBJ) $(CATCH).o
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) $(CATCH).o $(TEST_SOURCE) -o $(TEST_BIN)

jspy: jspy.cpp $(OBJ) $(SUB_OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(SUB_OBJ) jspy.cpp -o jspy

run: jspy
	./jspy usage.spy

shell: jspy
	./jspy

test: $(TEST_BIN); ./$(TEST_BIN)

check: $(TEST_BIN); valgrind ./$(TEST_BIN)

$(SUB_OBJ):
	@echo
	@echo "Checking submodules..."
	make -s -C $(foreach m, $(MODULES), $(m))
	@echo

clean:; rm -f $(TEST_BIN) jspy *.o
