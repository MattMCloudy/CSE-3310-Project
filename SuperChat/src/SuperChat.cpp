#include <signal.h>
#include <assert.h>
#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>

#include "DDSEntityManager.h"
#include "ccpp_SuperChat.h"

// This code is derived from the PrismTech HelloWorld examples



using namespace DDS;
using namespace SuperChat;

// these classes abstract out the sending and recieving of topics
// perhaps with some cleverness, generics could be used to eliminate
// some of the code 

typedef std::vector<chatroom> chatroom_list_t;

class chatroom_data
{
  private:
  DDSEntityManager em;
  chatroomDataWriter_var Writer;
  chatroomDataReader_var Reader;
  public:
  void send ( chatroom messageInstance )
  {
     ReturnCode_t status = Writer->write(messageInstance, DDS::HANDLE_NIL);
     checkStatus(status, "Writer::write");
  }
  void recv ( chatroom_list_t* chatroomList )
  {
     // if any data is available, it returns a list
     assert ( chatroomList );
     chatroomSeq msgList;
     SampleInfoSeq infoSeq;
     ReturnCode_t status =  - 1;
     status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
                             ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
     checkStatus(status, "msgDataReader::take");
     for (DDS::ULong j = 0; j < msgList.length(); j++)
     {
       chatroomList->push_back ( msgList[j] );
     }
     status = Reader->return_loan ( msgList, infoSeq );
     checkStatus(status, "MsgDataReader::return_loan");
  }
  chatroom_data (char *topic_name)
  {
    // participant
    em.createParticipant ("");

    // create types
    chatroomTypeSupport_var T = new chatroomTypeSupport ();
    em.registerType ( T.in() );

    // create Topic
    em.createTopic ( topic_name );

    // create Publisher
    em.createPublisher ();

    // create dataWriter
    em.createWriter ( false );
    DataWriter_var dwriter = em.getWriter ();
    Writer = chatroomDataWriter::_narrow ( dwriter.in() );

    //create Subscriber
    em.createSubscriber ();

    // create DataReader
    em.createReader ();
    DataReader_var dreader = em.getReader ();
    Reader = chatroomDataReader::_narrow( dreader.in () );
    checkHandle( Reader.in (), "MsgDataReader::_narrow" );
  }
  ~chatroom_data ()
  {
    // Remove the DataWriters.
    em.deleteWriter ();

    // Remove the DataReaders
    em.deleteReader ();

    // Remove the Publisher.
    em.deletePublisher ();

    // Remove the Subscriber.
    em.deleteSubscriber ();

    // Remove the Topics.
    em.deleteTopic ();

    // Remove Participant.
    em.deleteParticipant ();
  }
};

typedef std::vector<message> message_list_t;

class message_data
{
  private:
  DDSEntityManager em;
  messageDataWriter_var Writer;
  messageDataReader_var Reader;
  public:
  void send ( message messageInstance )
  {
     ReturnCode_t status = Writer->write(messageInstance, DDS::HANDLE_NIL);
     checkStatus(status, "Writer::write");
  }
  void recv ( message_list_t* messageList )
  {
     // if any data is available, it returns a list
     assert ( messageList );
     messageSeq msgList;
     SampleInfoSeq infoSeq;
     ReturnCode_t status =  - 1;
     status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
                             ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
     checkStatus(status, "msgDataReader::take");
     for (DDS::ULong j = 0; j < msgList.length(); j++)
     {
       messageList->push_back ( msgList[j] );
     }
     status = Reader->return_loan ( msgList, infoSeq );
     checkStatus(status, "MsgDataReader::return_loan");
  }
  message_data (char *topic_name)
  {
    // participant
    em.createParticipant ("");

    // create types
    messageTypeSupport_var T = new messageTypeSupport ();
    em.registerType ( T.in() );

    // create Topic
    em.createTopic ( topic_name );

    // create Publisher
    em.createPublisher ();

    // create dataWriter
    em.createWriter ( false );
    DataWriter_var dwriter = em.getWriter ();
    Writer = messageDataWriter::_narrow ( dwriter.in() );

    //create Subscriber
    em.createSubscriber ();

    // create DataReader
    em.createReader ();
    DataReader_var dreader = em.getReader ();
    Reader = messageDataReader::_narrow( dreader.in () );
    checkHandle( Reader.in (), "MsgDataReader::_narrow" );
  }
  ~message_data ()
  {
    // Remove the DataWriters.
    em.deleteWriter ();

    // Remove the DataReaders
    em.deleteReader ();

    // Remove the Publisher.
    em.deletePublisher ();

    // Remove the Subscriber.
    em.deleteSubscriber ();

    // Remove the Topics.
    em.deleteTopic ();

    // Remove Participant.
    em.deleteParticipant ();
  }
};

typedef std::vector<user> user_list_t;

class user_data
{
  private:
  DDSEntityManager em;
  userDataWriter_var Writer;
  userDataReader_var Reader;
  public:
  void send ( user userInstance )
  {
     ReturnCode_t status = Writer->write(userInstance, DDS::HANDLE_NIL);
     checkStatus(status, "Writer::write");
  }
  void recv ( user_list_t* userList )
  {
     // if any data is available, it returns a list
     assert ( userList );
     userSeq msgList;
     SampleInfoSeq infoSeq;
     ReturnCode_t status =  - 1;
     status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
                             ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
     checkStatus(status, "msgDataReader::take");
     for (DDS::ULong j = 0; j < msgList.length(); j++)
     {
       userList->push_back ( msgList[j] );
     }
     status = Reader->return_loan ( msgList, infoSeq );
     checkStatus(status, "MsgDataReader::return_loan");
  }
  user_data (char *topic_name)
  {
    // participant
    em.createParticipant ("");

    // create types
    userTypeSupport_var T = new userTypeSupport ();
    em.registerType ( T.in() );

    // create Topic
    em.createTopic ( topic_name );

    // create Publisher
    em.createPublisher ();

    // create dataWriter
    em.createWriter ( false );
    DataWriter_var dwriter = em.getWriter ();
    Writer = userDataWriter::_narrow ( dwriter.in() );

    //create Subscriber
    em.createSubscriber ();

    // create DataReader
    em.createReader ();
    DataReader_var dreader = em.getReader ();
    Reader = userDataReader::_narrow( dreader.in () );
    checkHandle( Reader.in (), "MsgDataReader::_narrow" );
  }
  ~user_data ()
  {
std::cout << "deconstructor" << '\n';
    // Remove the DataWriters.
    em.deleteWriter ();

    // Remove the DataReaders
    em.deleteReader ();

    // Remove the Publisher.
    em.deletePublisher ();

    // Remove the Subscriber.
    em.deleteSubscriber ();

    // Remove the Topics.
    em.deleteTopic ();

    // Remove Participant.
    em.deleteParticipant ();
  }
};
///////////////////////////////////////////////////////////////////////////////
bool exit_flag = false;

void ctrlc ( int )
{
   exit_flag = true;
}
///////////////////////////////////////////////////////////////////////////////

int main()
{
  // set up ctrl-c handler
  signal ( SIGINT, ctrlc );

  // instantiate classes
  chatroom_data chatRoom ( (char*) "chatroom" );
  user_data User ( (char*) "user" );
  message_data Message ( (char*) "msg" );

  // set up some variables
  int seconds = 0;

  // the main loop
  for (;!exit_flag;)
  {
    // send out each topic according to the rules of superchat
    // first is chatroom
    {
      if (seconds%60 == 0)
      {
        // once a minute change the chatroom name
        chatroom messageInstance;
        messageInstance.chatroom_idx = 1;
        if ( seconds%120 )
        {
          strncpy ( messageInstance.chatroom_name, "The pleasures", sizeof ( messageInstance.chatroom_name ) );
        }
        else
        {
          strncpy ( messageInstance.chatroom_name, "The !pleasures", sizeof ( messageInstance.chatroom_name ) );
        }
        chatRoom.send ( messageInstance );
      }
    }
    // user topic
    {
      user messageInstance;
      messageInstance.uuid = 123;
      strncpy ( messageInstance.nick, "Donny", sizeof ( messageInstance.nick ) );
      messageInstance.chatroom_idx = 0;  // public
      if (seconds%2 == 0) 
      {
         // 2.0 is less than 2.5, so this is still compliant
         User.send ( messageInstance );
      }
    }
    // message topic
    {
      message messageInstance;
      if (seconds%15 == 0)
      {
         if ( seconds%30 == 0 ) 
         {
           strncpy ( messageInstance.message, "This is the A message.", sizeof ( messageInstance.message ) );
         }
         else
         {
           strncpy ( messageInstance.message, "This is the B message.", sizeof ( messageInstance.message ) );
         }
         messageInstance.uuid = 123;
         messageInstance.chatroom_idx = 0;
         messageInstance.cksum = 0;
         Message.send ( messageInstance ); 
      }
    }

    // handle any input coming in
    {
      chatroom_list_t  List;
      chatRoom.recv ( &List );
      for (unsigned int i=0; i<List.size ();i++)
      {
         std::cout << "recieved new chatroom name " << List[i].chatroom_name <<
                      " chatroom index " << List[i].chatroom_idx << '\n';
      }
    } 
    {
      user_list_t  List;
      User.recv ( &List );
      for (unsigned int i=0; i<List.size ();i++)
      {
         std::cout << "recieved user " << List[i].nick <<
                      " chatroom index " << List[i].chatroom_idx << '\n';
      }
    } 
    {
      message_list_t  List;
      Message.recv ( &List );
      for (unsigned int i=0; i<List.size ();i++)
      {
         std::cout << "recieved user " << List[i].message <<
                      " chatroom index " << List[i].chatroom_idx << '\n';
      }
    } 
    seconds++;
    sleep (1);
  }
  std::cout << "normal exit" << '\n';
  return 0;
}
