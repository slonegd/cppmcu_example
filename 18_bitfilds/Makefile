all: clean $(BUILD_DIR)
	arm-none-eabi-g++ -c -mcpu=cortex-m0 -mthumb -Os -g -gdwarf-2 -Wno-packed-bitfield-compat -std=c++17 -Wa,-a,-ad,-alms=build/main.lst main.cpp -o build/main.o

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	-rm -fR .dep $(BUILD_DIR)
