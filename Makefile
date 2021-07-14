TARGET ?= a.out
SRC_DIRS ?= ./src

#PATH=/usr/local/opt/llvm/bin:$PATH
#CPLUS_INCLUDE_PATH=$(llvm-config --includedir):$CPLUS_INCLUDE_PATH 
#LD_LIBRARY_PATH=$(llvm-config --libdir):$LD_LIBRARY_PATH 

#CC=/usr/local/opt/llvm/bin/clang
CC=gcc

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CDEFS := -DMINIZ_NO_TIME
CPPFLAGS ?= $(CDEFS) $(INC_FLAGS) -MMD -MP -fsanitize=address -O1 -fno-omit-frame-pointer -g
#LDFLAGS="-L/usr/local/opt/llvm@8/lib -Wl,-rpath,/usr/local/opt/llvm@8/lib"

#LDFLAGS += -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib
#CPPFLAGS += -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
