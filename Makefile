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

# to run the application
run: $(TARGET)
	./$(TARGET) $(ARGS)

all: clean $(TARGET) run

# To obtain object files
$(BUILDFOLDER)/%.o: %.cpp
	@echo "if [ ! -d $(BUILDFOLDER) ]; then mkdir $(BUILDFOLDER); fi" | bash
	@echo "if [ ! -d $(shell dirname $@) ]; then mkdir $(shell dirname $@); fi" | bash
	$(Gxx) -c $(CPPFLAGS) $< -o $@

# To remove generated files
clean:
	rm -rf $(EXEC) $(BUILDFOLDER)
