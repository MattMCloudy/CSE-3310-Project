CXXFLAGS = -lncurses
INC_DIR = ../include
SRC_DIR = ../src
LIB_DIR = ../lib
OBJ_DIR = ../bin


OBJS = $(OBJ_DIR)/Initializer.o $(OBJ_DIR)/ChatDaemon.o $(OBJ_DIR)/UserInterface.o $(OBJ_DIR)/Chatroom.o $(OBJ_DIR)/User.o $(OBJ_DIR)/Message.o $(OBJ_DIR)/ViewController.o

all: executable

debug: CXXFLAGS += -g
debug: executable

rebuild: clean executable
executable: $(OBJS)
	$(CXX) $(CXXFLAGS) -o -C $(OBJS)
$(OBJ_DIR)/Initializer.o: $(SRC_DIR)/Initializer.cpp $(INC_DIR)/Initializer.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/Intializer.cpp
$(OBJ_DIR)/UserInterface.o: $(SRC_DIR)/UserInterface.cpp $(INC_DIR)/UserInterface.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/UserInterface.cpp
$(OBJ_DIR)/ChatDaemon.o: $(SRC_DIR)/ChatDaemon.cpp $(INC_DIR)/ChatDaemon.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/ChatDaemon.cpp
$(OBJ_DIR)/Chatroom.o: $(SRC_DIR)/Chatroom.cpp $(INC_DIR)/Chatroom.h $(INC_DIR)/chatroom_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/Chatroom.cpp
$(OBJ_DIR)/User.o: $(SRC_DIR)/User.cpp $(INC_DIR)/User.h $(INC_DIR)/user_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/User.cpp
$(OBJ_DIR)/Message.o: $(SRC_DIR)/Message.cpp $(INC_DIR)/Message.h $(INC_DIR)/message_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/Message.cpp
$(OBJ_DIR)/ViewController.o: $(SRC_DIR)/ViewController.cpp $(INC_DIR)/ViewController.h
	$(CXX) $(CXXFLAGS) -w -c -I $(SRC_DIR)/ViewController.cpp
clean:
	cd .. && cd bin && rm *.o
