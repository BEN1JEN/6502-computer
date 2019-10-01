ASM         := ca65
LINKER      := ld65
AFLAGS      := 
LDFLAGS     := -C comp6502.ld
BUILDFOLDER := make

ifeq (run,$(firstword $(MAKECMDGOALS)))
	# use the 2nd arg as the target
	TARGET := $(firstword $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS)))
else ifeq (all,$(firstword $(MAKECMDGOALS)))
else ifneq (clean,$(firstword $(MAKECMDGOALS)))
	# use the first arg as the target
	TARGET := $(firstword $(MAKECMDGOALS))
endif
SOURCES = $(wildcard $(TARGET)/*.asm)
MODULES = $(patsubst %,$(BUILDFOLDER)/%,$(SOURCES:.asm=.o))

.PHONY: all
all: $(shell echo */)
	@echo Made Everything

.PHONY: run
run: $(TARGET)
	@echo Ran $(TARGET)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
	rm -rf *.bin

$(TARGET): $(MODULES)
	$(LINKER) -o $(TARGET).bin $(LDFLAGS) $(MODULES)

$(BUILDFOLDER)/$(TARGET)/%.o: $(TARGET)/%.asm
	@echo "if [ ! -d $(BUILDFOLDER) ]; then mkdir $(BUILDFOLDER); fi" | bash
	@echo "if [ ! -d $(BUILDFOLDER)/$(TARGET) ]; then mkdir $(BUILDFOLDER)/$(TARGET); fi" | bash
	$(ASM) $(AFLAGS) $< -o $@
