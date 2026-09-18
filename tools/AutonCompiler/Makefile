CXX := c++
FTXUI_PREFIX := $(shell brew --prefix ftxui)
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(FTXUI_PREFIX)/include
LDFLAGS := -L$(FTXUI_PREFIX)/lib
LDLIBS := -lftxui-component -lftxui-dom -lftxui-screen

autonCompiler: autonCompiler.cpp

.PHONY: run clean
run: autonCompiler
	./autonCompiler
clean:
	rm -f autonCompiler
