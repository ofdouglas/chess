include ./makefile.common

TEST_DIR := tests

# The cpp->h file dependency rules are in makefiles (.d files) created by g++ (-MMD)
DEP = $(OBJ:.o=.d)

.PHONY: all tests clean

all: main

# include rules from the .d files
-include $(DEP)

main: $(OBJ) unit_tests
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

unit_tests:
	@$(MAKE) -C $(TEST_DIR)

clean:
	@rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d main
