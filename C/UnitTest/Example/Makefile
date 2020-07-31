CC		:= gcc
C_FLAGS := -g -pthread

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= test.exe
else
EXECUTABLE	:= test
endif

all: 	$(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)

run: all
	$(EXECUTABLE)

$(EXECUTABLE): *.c
	$(CC) $(C_FLAGS) -I -L $^ -o $@ $(LIBRARIES)
