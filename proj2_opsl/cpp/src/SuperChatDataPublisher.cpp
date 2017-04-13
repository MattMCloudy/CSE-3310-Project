/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2013 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE
 *
 *   for full copyright notice and license terms.
 *
 */

/************************************************************************
 * LOGICAL_NAME:    SuperChatDataPublisher.cpp
 * FUNCTION:        OpenSplice Tutorial example code.
 * MODULE:          Tutorial for the C++ programming language.
 * DATE             September 2010.
 ************************************************************************
 *
 * This file contains the implementation for the 'SuperChatDataPublisher' executable.
 *
 ***/
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "DDSEntityManager.h"
#include "ccpp_SuperChatData.h"
#include "os.h"



#include "example_main.h" //find this in _hde/x64,.../examples/include to see what this does. but you can just put the main file in the same directory as your other stuff to make it easier.  

////////////INCLUDES FOR THE UUID FUNCTION//////////////////////////
// from stack overflow
// http://stackoverflow.com/questions/3247861/example-of-uuid-generation-using-boost-in-c
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
const char* message; 

long long int create_uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::cout << uuid << std::endl;
    long long int x;
    // lets go old school, and copy it over
    memcpy ( &x, &uuid, sizeof (x) );
    std::cout << "as a number " << std::hex << x << std::endl;
    return x;
}
///FUNCTION THAT READS MESSAGE FROM DUMMY FILE. PLACE HOLDER FOR READING MESSAGE FROM UID//////
const char* get_message(){

  string my_str; 
  ifstream fp;
  fp.open("test_messages.txt");
  
  if (!fp){
    cout << "\nERROR: can't open file\n"; 
    return 0; 
  }
  getline(fp, my_str); 
  message = my_str.c_str();

  fp.close(); 
  return message; 

}

using namespace DDS;
using namespace SuperChatData;

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
  OS_API_EXPORT
  int SuperChatDataPublisher(int argc, char *argv[]);
}

int SuperChatDataPublisher(int argc, char *argv[])
{
  os_time delay_1s = { 1, 0 };
  DDSEntityManager mgr;

  // create domain participant
  mgr.createParticipant("SuperChat!!!!!");


//I DON'T UNDERSTAND WHAT "TYPE" IS EVEN THOUGH I READ IT IN THE TUTORIAL DOCUMENT.*********************************************************

  //create type
  MsgTypeSupport_var mt = new MsgTypeSupport();
  mgr.registerType(mt.in());

  //create Topic
  char topic_name[] = "SuperChatData_Msg";
  mgr.createTopic(topic_name);

  //create Publisher
  mgr.createPublisher();

  // create DataWriter :
  // If autodispose_unregistered_instances is set to true (default value),
  // you will have to start the subscriber before the publisher
  bool autodispose_unregistered_instances = false;
  mgr.createWriter(autodispose_unregistered_instances);

  // Publish Events
  DataWriter_var dwriter = mgr.getWriter();
  MsgDataWriter_var SuperChatWriter = MsgDataWriter::_narrow(dwriter.in());

//use ID generator code provided by Davis to set the userID**********************************************************************************
//change message to inputted message from user**********************************************************************************************

  Msg msgInstance; /* Example on Stack */
  msgInstance.userID = create_uuid();
  msgInstance.message = get_message(); 
  //msgInstance.message = DDS::string_dup("Welcome to SuperChat, yo.");
  cout << "=== [Publisher] writing a message containing :" << endl;
  cout << "    userID  : " << msgInstance.userID << endl;
  cout << "    Message : \"" << msgInstance.message << "\"" << endl;

  ReturnCode_t status = SuperChatWriter->write(msgInstance, DDS::HANDLE_NIL);
  checkStatus(status, "MsgDataWriter::write");
  os_nanoSleep(delay_1s);

  /* Remove the DataWriters */
  mgr.deleteWriter();

  /* Remove the Publisher. */
  mgr.deletePublisher();

  /* Remove the Topics. */
  mgr.deleteTopic();

  /* Remove Participant. */
  mgr.deleteParticipant();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
  return SuperChatDataPublisher (argc, argv);
}
