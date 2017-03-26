CXXFLAGS = -lncurses

all: executable

debug: CXXFLAGS += -g
debug: executable

rebuild: clean executable
executable: Initializer.o UserInterface.o ChatDaemon.o Chatroom.o User.o Message.o ViewController.o
	$(CXX) $(CXXFLAGS) Initializer.o UserInterface.o ChatDaemon.o Chatroom.o User.o Message.o ViewController.o 
Initializer.o: ../lib/Initializer.cpp ../include/Initializer.h
	$(CXX) $(CXXFLAGS) -w -c ../lib/Intializer.cpp 
UserInterface.o: UserInterface.cpp UserInterface.h
	$(CXX) $(CXXFLAGS) -w -c UserInterface.cpp
ChatDaemon.o: ChatDaemon.cpp ChatDaemon.h
	$(CXX) $(CXXFLAGS) -w -c ChatDaemon.cpp
Chatroom.o: Chatroom.cpp Chatroom.h chatroom_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c Chatroom.cpp
User.o: User.cpp User.h user_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c User.cpp
Message.o: Message.cpp Message.h message_c_struct.h
	$(CXX) $(CXXFLAGS) -w -c Message.cpp
ViewController.o: ViewController.cpp ViewController.h
	$(CXX) $(CXXFLAGS) -w -c ViewController.cpp
clean:
	rm -rf *.o a.out
