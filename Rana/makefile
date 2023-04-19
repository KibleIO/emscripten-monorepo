#use for debugging
#CC = em++ -O0 -sUSE_SDL=2 -sUSE_PTHREADS 
CC = em++ -O3 -sDISABLE_EXCEPTION_CATCHING=1 -sUSE_SDL=2 -sUSE_PTHREADS 
BC = index.bc
EXEC = index.js
SOURCES = $(filter-out $(wildcard Broadway-H.264-decoder/*/*/*.cpp), $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS = -lwebsocket.js -sASYNCIFY -sTOTAL_MEMORY=2048MB -sUSE_WEBGL2=1 \
-sAGGRESSIVE_VARIABLE_ELIMINATION=1 -sNO_EXIT_RUNTIME=1 \
-sALIASING_FUNCTION_POINTERS=1 -sDOUBLE_MODE=0 --memory-init-file 1 \
Broadway-H.264-decoder/libbroadway.a -sSTACK_SIZE=1024MB

#use for debugging
#CXXFLAGS = -lwebsocket.js -sASYNCIFY -sTOTAL_MEMORY=2048MB -sUSE_WEBGL2=1 \
#Broadway-H.264-decoder/libbroadway.a -sSTACK_SIZE=1024MB

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

# To remove generated files
clean:
	rm -rf $(OBJECTS) $(EXEC) $(BC) index.wasm index.worker.js
	
build_broadway:
	make -C Broadway-H.264-decoder

clean_broadway:
	make -C Broadway-H.264-decoder clean
