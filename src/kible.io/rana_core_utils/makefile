CC = em++ -O3 -sDISABLE_EXCEPTION_CATCHING=1 -sUSE_SDL=2
AR = emar rcs
EXEC = librana.a
SOURCES = $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

default: $(OBJECTS)
	$(AR) $(EXEC) $(OBJECTS)

# Main target
$(EXEC): $(OBJECTS)
	$(AR) $(EXEC) $(OBJECTS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_BUILD_FLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXEC)