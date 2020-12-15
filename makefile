PROD_SRC_DIR := src
TEST_SRC_DIR := tests
APP_SRC_DIR  := app
APP_BUILD_DIR := build/app
TEST_BUILD_DIR := build/test
INC_DIR := inc


PROD_SRC := $(wildcard $(PROD_SRC_DIR)/*.cpp)
TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.cpp)
APP_SRC  := $(wildcard $(APP_SRC_DIR)/*.cpp)

PROD_OBJ := $(patsubst $(PROD_SRC_DIR)/%.cpp, $(APP_BUILD_DIR)/%.o, $(PROD_SRC)) \
			$(patsubst $(APP_SRC_DIR)/%.cpp, $(APP_BUILD_DIR)/%.o, $(APP_SRC))

TEST_OBJ := $(patsubst $(PROD_SRC_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(PROD_SRC)) \
			$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SRC))


$(info $(TEST_OBJ))
$(info $(PROD_OBJ))

CPPFLAGS += -I$(INC_DIR) -g
TEST_CPPFLAGS := -I$(INC_DIR) -g -include /usr/include/CppUTest/MemoryLeakDetectorMallocMacros.h
TEST_LDLIBS += -lCppUTest


.PHONY: all run_tests clean
all: run_tests main


# Include header dependency rules from the .d files (created by g++ option -MMD)
DEP = $(OBJ:.o=.d)
-include $(DEP)


# Build the main application
$(APP_BUILD_DIR)/%.o: $(PROD_SRC_DIR)/%.cpp | $(APP_BUILD_DIR)
	$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

$(APP_BUILD_DIR)/%.o: $(APP_SRC_DIR)/%.cpp | $(APP_BUILD_DIR)
	$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

main: $(PROD_OBJ)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(PROD_OBJ) $(LDLIBS) -o $@


# Build the unit tests
$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(TEST_CPPFLAGS) -MMD -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(PROD_SRC_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(TEST_CPPFLAGS) -MMD -c $< -o $@

unit_tests: $(TEST_OBJ)
	$(CXX) $(TEST_CPPFLAGS) $(TEST_LDFLAGS) $(TEST_OBJ) $(TEST_LDLIBS) -o $@


# Miscellaneous rules
run_tests: unit_tests
	'./unit_tests'

$(APP_BUILD_DIR):
	mkdir -p $@

$(TEST_BUILD_DIR):
	mkdir -p $@

clean:
	@find ./ -iregex '.*\.[od]' -exec rm {} +
	@rm -f main unit_tests
