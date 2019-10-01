# Variables
Gxx = g++
LIBS = gl
CPPFLAGS = -Isource -Wall -Wextra $(shell pkg-config --cflags $(LIBS))
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -pthread -lpng -lX11

BUILDFOLDER = make
# Files
TARGET = comp6502
SOURCES = $(wildcard source/*.cpp) $(wildcard source/*/*.cpp)
MODULES = $(patsubst %,make/%,$(SOURCES:.cpp=.o))

# Main target
$(TARGET): $(MODULES)
	$(Gxx) $(MODULES) $(LDFLAGS) -o $(TARGET)

# To obtain object files
$(BUILDFOLDER)/%.o: %.cpp
	@echo "if [ ! -d $(BUILDFOLDER) ]; then mkdir $(BUILDFOLDER); fi" | bash
	@echo "if [ ! -d $(shell dirname $@) ]; then mkdir $(shell dirname $@); fi" | bash
	$(Gxx) -c $(CPPFLAGS) $< -o $@

# to run the application
.PHONY: run
run: $(TARGET) tests
	./$(TARGET) $(ARGS)

.PHONY: all
all: clean run

.PHONY: tests
tests:
	@echo "6502-computer/tests$ make all"
	@echo "pushd test && make all; popd" | bash

# To remove generated files
.PHONY: clean
clean:
	rm -rf $(EXEC) $(BUILDFOLDER)
