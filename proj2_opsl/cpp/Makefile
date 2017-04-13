all:bin/SuperChatPublisher bin/SuperChatSubscriber

FILES_GENERATED_FROM_THE_IDL_BY_IDLPP= ./bin/SuperChatDataDcps_impl.h ./bin/SuperChatData.cpp \
                                     ./bin/SuperChatDataSplDcps.cpp ./bin/SuperChatDataDcps.cpp \
                                     ./bin/SuperChatDataDcps_impl.cpp ./bin/ccpp_SuperChatData.h \
                                     ./bin/SuperChatDataDcps.h ./bin/SuperChatData.h ./bin/SuperChatDataSplDcps.h

${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP}:./idl/SuperChatData.idl
	idlpp -I ${OSPL_HOME}/etc/idl -l cpp ./idl/SuperChatData.idl -d ./bin

COMMON_SRC_FILES=./src/CheckStatus.cpp  ./src/DDSEntityManager.cpp
COMMON_H_FILES=./src/CheckStatus.h

# this library list may need some adjust for different operating systems
# works on cse3320.org
COMMON_LIBS=-L${OSPL_HOME}/lib/ \
            -ldcpssacpp \
            -lddskernel \
            -lpthread

CXXFLAGS=-Wall -O0 -g

bin/SuperChatPublisher : src/SuperChatDataPublisher.cpp ${COMMON_SRC_FILES} ${COMMON_H_FILES} ${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP}
	g++  ${CXXFLAGS} \
        -o bin/SuperChatPublisher src/SuperChatDataPublisher.cpp  ${COMMON_SRC_FILES} ${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP} \
        -I./src/ -I./include -I./bin \
        -I${OSPL_HOME}/include/dcps/C++/SACPP/ \
        ${COMMON_LIBS}

bin/SuperChatSubscriber : src/SuperChatDataSubscriber.cpp ${COMMON_SRC_FILES} ${COMMON_H_FILES} ${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP}
	g++  ${CXXFLAGS} \
        -o bin/SuperChatSubscriber src/SuperChatDataSubscriber.cpp  ${COMMON_SRC_FILES} ${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP} \
        -I./src/ -I./include -I./bin \
        -I${OSPL_HOME}/include/dcps/C++/SACPP/ \
        ${COMMON_LIBS}

clean:
	-rm -f ${FILES_GENERATED_FROM_THE_IDL_BY_IDLPP}
	-rm -f bin/SuperChatPublisher bin/SuperChatSubscriber
