 
DIR_INC = ./
DIR_SRC = ./

CC=gcc
CXX=gcc
AR=gcc-ar
#ESTRIP = arm-linux-gnueabihf-strip

INCFLAGS = -I$(DIR_INC) 
CFLAGS   = -Wall -O2 
CXXFLAGS = -O2 -pipe -feliminate-unused-debug-types -Wall  

vpath %.h $(DIR_INC) 
vpath %.c $(DIR_SRC)
 
SOURCES = $(wildcard $(DIR_SRC)*.c)  
#OBJS = $(SOURCES:.c=.o)
OBJS=$(patsubst %.c,%.o,$(notdir $(SOURCES)))

TARGET = popen
 
all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) -lpthread -o $(TARGET) 
 
$(OBJS):$(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $^

clean:
	rm -f *.o $(TARGET)





