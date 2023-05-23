#use for debugging
CC = em++ -O0 -sUSE_SDL=2 -sUSE_PTHREADS --profiling-funcs -sSAFE_HEAP=1 -sASSERTIONS=1 -sSTACK_OVERFLOW_CHECK=2 -gsource-map
#CC = em++ -O3 -sDISABLE_EXCEPTION_CATCHING=1 -sUSE_SDL=2 -sUSE_PTHREADS -pthread
BC = index.bc
EXEC = index.js
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp) $(wildcard */*/*/*/*.cpp) $(wildcard ../gen/*/*.cpp) $(wildcard ../Broadway-H.264-decoder/src/*.c)
OBJECTS = $(SOURCES:.cpp=.o) $(SOURCES:.c=.o)

CC_BUILD_FLAGS = -I../protolib -I../gen

#CXXFLAGS = -lwebsocket.js -sASYNCIFY -sTOTAL_MEMORY=2048MB -sUSE_WEBGL2=1 \
-sAGGRESSIVE_VARIABLE_ELIMINATION=1 -sNO_EXIT_RUNTIME=1 \
-sALIASING_FUNCTION_POINTERS=1 -sDOUBLE_MODE=0 --memory-init-file 1 \
../Broadway-H.264-decoder/libbroadway.a ../opus/.libs/libopus.a \
../protolib/libprotobuf.a -sSTACK_SIZE=512MB -sPTHREAD_POOL_SIZE=20 -sFETCH

#use for debugging
CXXFLAGS = -lwebsocket.js -sASYNCIFY -sTOTAL_MEMORY=4086MB -sUSE_WEBGL2=1 \
../opus/.libs/libopus.a \
../protolib/libprotobuf.a -sSTACK_SIZE=10MB -sPTHREAD_POOL_SIZE=20 -sFETCH -sALLOW_MEMORY_GROWTH -sMAXIMUM_MEMORY=4096MB

default: $(OBJECTS)
	$(CC) -r $(OBJECTS) -o $(BC)
	$(CC) $(BC) -o $(EXEC) $(CXXFLAGS)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) -r $(OBJECTS) -o $(BC)
	$(CC) $(BC) -o $(EXEC) $(CXXFLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_BUILD_FLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CC_BUILD_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -rf $(OBJECTS) $(EXEC) $(BC) index.wasm index.worker.js
