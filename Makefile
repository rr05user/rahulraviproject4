CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = load_balancer.exe

SOURCES = main.cpp \
          Request.cpp \
          WebServer.cpp \
          LoadBalancer.cpp \
          IPFirewall.cpp \
          Config.cpp \
          Logger.cpp

OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)