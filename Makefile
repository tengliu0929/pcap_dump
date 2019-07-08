CXX = g++

MYLIBS =  -lpcap
LDFLAGS =
CCOPT =  -m64
#INCLS = -I../include 
DEFS =
CFLAGS = $(CCOPT) $(DEFS) $(CPPFLAGS) -g

SOURCES =  $(wildcard *.cpp) $(wildcard *.c)

OBJS := $(patsubst %.c,%.o, $(SOURCES))

TARGET = sender


all: $(TARGET)

$(TARGET) : $(OBJS)
	echo $(SOURCES)
	$(CXX)  -o $@ $^  $(CFLAGS) $(LDFLAGS) $(MYLIBS) 
	chmod a+x $(TARGET)

%.o:%.cpp
	$(CXX) -c $< $(CFLAGS) $(EXTRAFLAG)  
clean:
	rm -rf *.o $(TARGET)

