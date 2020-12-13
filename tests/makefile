include ../makefile.common
INC_DIR = ../inc

# CppUTest stuff
CPPUTEST_INC_DIR =  /usr/include/CppUTest
TEST_CPPFLAGS += -include $(CPPUTEST_INC_DIR)/MemoryLeakDetectorMallocMacros.h
TEST_LDLIBS += -lCppUTest

# The cpp->h file dependency rules are in makefiles (.d files) created by g++ (-MMD)
DEP = $(OBJ:.o=.d)

.PHONY: all clean

all: unit_tests

# include rules from the .d files
-include $(DEP)

unit_tests: $(OBJ)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $^ $(TEST_LDLIBS) -o $@ && ./unit_tests

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(TEST_CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

tests:
	$(MAKE) -C $(TEST_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d unit_tests
