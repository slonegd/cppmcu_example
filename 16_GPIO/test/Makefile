CPP_FLAGS = -std=c++17 -I../../CMSIS -I.. -DTEST -g
BUILD_DIR = build

all: $(BUILD_DIR)
	g++ test_pin.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_pin
	g++ test_cyclic_task.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_cyclic_task
	g++ test_button.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/test_button
	./$(BUILD_DIR)/test_pin
	./$(BUILD_DIR)/test_cyclic_task
	./$(BUILD_DIR)/test_button

$(BUILD_DIR):
	mkdir $@