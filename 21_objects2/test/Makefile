CPP_FLAGS = -std=c++17 -I../../CMSIS -I.. -DTEST -g -Wno-register
BUILD_DIR = build

all: $(BUILD_DIR)
	# g++-7 test_pin.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_pin
	g++-7 test_cyclic_task.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_cyclic_task
	g++-7 test_button.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_button
	# ./$(BUILD_DIR)/test_pin
	./$(BUILD_DIR)/test_cyclic_task
	./$(BUILD_DIR)/test_button

$(BUILD_DIR):
	mkdir $@