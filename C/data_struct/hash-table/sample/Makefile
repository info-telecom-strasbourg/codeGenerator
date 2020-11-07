CC ?=	gcc
CFLAGS ?=	-Wall -Wextra -Werror -g
INCLUDE_PATH = ./include
TARGET   =	main
SRCDIR   =	src
OBJDIR   =	obj
BINDIR   =	bin
SOURCES  :=	$(wildcard $(SRCDIR)/*.c)
INCLUDES :=	$(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  :=	$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LIBS := hashmap

# Update include path
INCLUDE_PATH := $(INCLUDE_PATH) $(foreach lib_path, $(LIBS), ./$(lib_path))
INCLUDE := $(foreach lib_dir, $(INCLUDE_PATH), $(addprefix -I, $(lib_dir)))

# .o files of the library
LIB_OBJ := $(foreach lib_obj, $(LIBS), $(OBJDIR)/$(lib_obj).o)

# compile all the librairies
$(foreach lib, $(LIBS), $(shell $(CC) -o $(OBJDIR)/$(lib).o -c $(lib)/$(lib).c $(CFLAGS)))

$(BINDIR)/$(TARGET): $(OBJECTS) $(LIB_OBJ)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(CFLAGS)
	@echo "Linking complete!"
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

run:
	make clean
	make

.PHONY: clean test
clean:
	rm -rf obj/*.o
	rm -f bin/*
