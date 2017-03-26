SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BIN_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard $(INC_DIR)/*.h)

LDFLAGS = -w -lncurses -I$(INC_DIR)/..

all: super

super: $(OBJS) $(DEPS)
	$(CXX) -o $@ $^ $(LDFLAGS)

all: 
	cd src && mv *.o ../bin

clean:
	rm super && cd bin && rm *.o
