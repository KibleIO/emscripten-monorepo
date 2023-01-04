# sanitize memory (also does leak checking)
#SAN +=-fsanitize=address
# sanitize leaks
# SAN +=-fsanitize=leak
# sanitize threads
#SAN +=-fsanitize=thread
# sanitize threads
#SAN +=-fsanitize=undefined
# sanitize off
SAN =

CC = g++ -O3 $(SAN)
CC_BUILD_FLAGS =
CXXFLAGS = -lwebsockets

# File names
EXEC = server
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

docker: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CC_COMPILE_FLAGS) $(CXXFLAGS)

xubuntu:
	docker run --rm -it -v `pwd`:/root/code kible/websocket make docker

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CC_COMPILE_FLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_BUILD_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

dockerimagebuild:
	docker build --file Dockerfile -t kible/websocket .

run:
	docker run --rm -it -p 4000:4000 -v `pwd`:/root/code kible/websocket ./server