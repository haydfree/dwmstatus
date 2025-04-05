TARGET   = dwmstatus

GENFLAGS = -ansi -Wall -Wextra -Werror -Wpedantic 
GDBFLAGS = -g -D_GNU_SOURCE

CC       = gcc
CINCS    = -I./inc -I/usr/X11R6/include
CFLAGS   = $(GENFLAGS) $(GDBFLAGS) $(CINCS)

LINKER   = gcc
LINCS    = -I./inc -L/usr/lib -L/usr/X11R6/lib -lX11 -lasound -lc 
LFLAGS   = $(GENFLAGS) $(GDBFLAGS) $(LINCS) 

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
	@pkill -x $(TARGET) || true
	@sleep 1
	@cp ./bin/$(TARGET) /usr/local/bin/
	@echo "*****EXECUTABLE INSTALLED*****"  

.PHONY: all
all: $(BINDIR)/$(TARGET)
