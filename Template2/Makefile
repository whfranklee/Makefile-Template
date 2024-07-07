
OBJDIR	    := obj
SYS_INC	    := ./3rd
SYS_LIB	    := ./3rd

INC_DIRS    := $(SYS_INC)/dbus/include/dbus-1.0 $(SYS_LIB)/dbus/lib/dbus-1.0/include \
		$(SYS_INC)/glib/include/glib-2.0 $(SYS_LIB)/glib/lib/glib-2.0/include \
		$(SYS_INC)/pcre/include \
		$(SYS_INC)/libffi/include \
		$(SYS_INC)/zlib/include \
		$(SYS_INC)/protobuf/include \
		$(SYS_INC)/proto
		

LIBS	    := dbus-1 glib-2.0 pcre pthread gio-2.0 gobject-2.0 gmodule-2.0 z ffi protobuf atomic
LIBS_PATH   := $(SYS_LIB)/dbus/lib $(SYS_LIB)/glib/lib $(SYS_LIB)/pcre/lib $(SYS_LIB)/libffi/lib $(SYS_LIB)/zlib/lib $(SYS_LIB)/protobuf/lib $(SYS_LIB)/atomic/lib

COMPILE_FLAGS := \
	-c \
	-g \
	-O2 \
	$(INC_DIRS:%=-I%)

CC_FLAG	:= \
	$(COMPILE_FLAGS)

LINK_LIBS := \
	$(LIBS:%=-l%)

LIBRARY_PATH := \
	$(LIBS_PATH:%=-L%)


CROSS_COMPILE := mipsel-openwrt-linux-
CXX	:= $(CROSS_COMPILE)g++

OBJ_NAME=ble_uart_server

#_APP_OBJS   := main.o advertising.o log.o gatt.o uart_server.o adapter.o char_respond.o sys_utils.o AddGateway.pb.o AssertLocation.pb.o Diagnostics.pb.o WiFiConnect.pb.o WiFiRemove.pb.o WiFiServices.pb.o 
#APP_OBJS    := $(addprefix $(OBJDIR)/, $(_APP_OBJS))

SOURCES = $(wildcard ./*.cpp ./proto/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

$(OBJ_NAME) : $(OBJECTS)
	$(CXX) $^ -o $@ $(LIBRARY_PATH) $(LINK_LIBS)

$(OBJECTS) : %.o : %.cpp 
	$(CXX) -c $(CC_FLAG) $< -o $@
	
.PHONY : clean

clean :
	rm -rf $(OBJ_NAME) $(OBJECTS) 

