CC		:= g++
C_FLAGS := -std=c++11 -g -pthread

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= tests
endif

all: 	$(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)

run: all
	$(EXECUTABLE)

$(EXECUTABLE): *.cpp
	$(CC) $(C_FLAGS) -I -L $^ -o $@ $(LIBRARIES)
