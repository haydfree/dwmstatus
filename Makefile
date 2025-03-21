# project name
TARGET   = dwmstatus

# compiler and compiler flags
CC       = gcc
CFLAGS   = -std=c99 -Iinc -D_GLIBCXX_DEBUG -D_GNU_SOURCE -g -I/usr/X11R6/include
# -fsanitize=address -fsanitize=undefined

# linker and linker flags
LINKER   = gcc
LFLAGS   = -std=c99 -Iinc -D_GLIBCXX_DEBUG -D_GNU_SOURCE -g -L/usr/lib -L/usr/X11R6/lib -lX11 -lasound -lc

SRCDIR   = src
INCDIR   = inc
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "*****LINKING COMPLETE*****"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "*****COMPILATION COMPLETE*****"

.PHONY: run
run:
	@./$(BINDIR)/$(TARGET)
	@echo "*****EXECUTION COMPLETE*****"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "*****CLEANUP COMPLETE*****"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "*****EXECTUABLE REMOVED*****"

.PHONY: install
install:
	@cp ./bin/$(TARGET) /usr/local/bin/
	@echo "*****EXECUTABLE INSTALLED*****"  

.PHONY: all
all: $(BINDIR)/$(TARGET)
