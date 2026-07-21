CXX = g++
CXXFLAGS = -std=c++20 -I./src/includes

# very messy sources thingy
SRCS := main.cpp $(shell find ./src -type f -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)
TARGET = flourish

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install:
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)

build:
	make
	make install

clean:
	rm -f $(OBJS) $(TARGET)

# tests
hello:
	make
	./flourish ./src/tests/hello-world/ 3000
var:
	make
	./flourish ./src/tests/variables/ 3000
zyrikl:
	make
	./flourish ./src/tests/zyrikl/ 3000