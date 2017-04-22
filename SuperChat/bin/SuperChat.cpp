//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: SuperChat.cpp
//  Source: idl/SuperChat.idl
//  Generated: Fri Apr 21 20:38:33 2017
//  OpenSplice V6.4.140407OSS
//  
//******************************************************************

#include "SuperChat.h"


#if DDS_USE_EXPLICIT_TEMPLATES
template class DDS_DCPS_FArray_var< SuperChat::user::_nick, SuperChat::user::_nick_slice, struct SuperChat::user::_nick_uniq_>;
template class DDS_DCPS_Array_forany< SuperChat::user::_nick, SuperChat::user::_nick_slice, struct SuperChat::user::_nick_uniq_>;
#endif

template <>
SuperChat::user::_nick_slice* DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::alloc ()
{
   return SuperChat::user::_nick_alloc ();
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::copy (SuperChat::user::_nick_slice *to, const SuperChat::user::_nick_slice* from)
{
   SuperChat::user::_nick_copy (to, from);
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::free (SuperChat::user::_nick_slice *ptr)
{
   SuperChat::user::_nick_free (ptr);
}

SuperChat::user::_nick_slice * SuperChat::user::_nick_alloc ()
{
   DDS::Char * ret = (DDS::Char*) new DDS::Char [8];
   return (_nick_slice *) ret;
}

void SuperChat::user::_nick_free (_nick_slice * s)
{
   delete [] s;
}

void SuperChat::user::_nick_copy
(
   _nick_slice * to,
   const _nick_slice * from
)
{
   const DDS::Char* sv = ( const DDS::Char*) from;
   DDS::Char* tv = (DDS::Char*) to;
   for (DDS::ULong i = 0; i < 8; i++) tv[i] = sv[i];
}

SuperChat::user::_nick_slice * SuperChat::user::_nick_dup
   (const _nick_slice * from)
{
   _nick_slice * to = _nick_alloc ();
   _nick_copy (to, from);
   return to;
}


#if DDS_USE_EXPLICIT_TEMPLATES
template class DDS_DCPS_FArray_var< SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, struct SuperChat::chatroom::_chatroom_name_uniq_>;
template class DDS_DCPS_Array_forany< SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, struct SuperChat::chatroom::_chatroom_name_uniq_>;
#endif

template <>
SuperChat::chatroom::_chatroom_name_slice* DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::alloc ()
{
   return SuperChat::chatroom::_chatroom_name_alloc ();
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::copy (SuperChat::chatroom::_chatroom_name_slice *to, const SuperChat::chatroom::_chatroom_name_slice* from)
{
   SuperChat::chatroom::_chatroom_name_copy (to, from);
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::free (SuperChat::chatroom::_chatroom_name_slice *ptr)
{
   SuperChat::chatroom::_chatroom_name_free (ptr);
}

SuperChat::chatroom::_chatroom_name_slice * SuperChat::chatroom::_chatroom_name_alloc ()
{
   DDS::Char * ret = (DDS::Char*) new DDS::Char [25];
   return (_chatroom_name_slice *) ret;
}

void SuperChat::chatroom::_chatroom_name_free (_chatroom_name_slice * s)
{
   delete [] s;
}

void SuperChat::chatroom::_chatroom_name_copy
(
   _chatroom_name_slice * to,
   const _chatroom_name_slice * from
)
{
   const DDS::Char* sv = ( const DDS::Char*) from;
   DDS::Char* tv = (DDS::Char*) to;
   for (DDS::ULong i = 0; i < 25; i++) tv[i] = sv[i];
}

SuperChat::chatroom::_chatroom_name_slice * SuperChat::chatroom::_chatroom_name_dup
   (const _chatroom_name_slice * from)
{
   _chatroom_name_slice * to = _chatroom_name_alloc ();
   _chatroom_name_copy (to, from);
   return to;
}


#if DDS_USE_EXPLICIT_TEMPLATES
template class DDS_DCPS_FArray_var< SuperChat::message::_message, SuperChat::message::_message_slice, struct SuperChat::message::_message_uniq_>;
template class DDS_DCPS_Array_forany< SuperChat::message::_message, SuperChat::message::_message_slice, struct SuperChat::message::_message_uniq_>;
#endif

template <>
SuperChat::message::_message_slice* DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::alloc ()
{
   return SuperChat::message::_message_alloc ();
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::copy (SuperChat::message::_message_slice *to, const SuperChat::message::_message_slice* from)
{
   SuperChat::message::_message_copy (to, from);
}

template <>
void DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::free (SuperChat::message::_message_slice *ptr)
{
   SuperChat::message::_message_free (ptr);
}

SuperChat::message::_message_slice * SuperChat::message::_message_alloc ()
{
   DDS::Char * ret = (DDS::Char*) new DDS::Char [144];
   return (_message_slice *) ret;
}

void SuperChat::message::_message_free (_message_slice * s)
{
   delete [] s;
}

void SuperChat::message::_message_copy
(
   _message_slice * to,
   const _message_slice * from
)
{
   const DDS::Char* sv = ( const DDS::Char*) from;
   DDS::Char* tv = (DDS::Char*) to;
   for (DDS::ULong i = 0; i < 144; i++) tv[i] = sv[i];
}

SuperChat::message::_message_slice * SuperChat::message::_message_dup
   (const _message_slice * from)
{
   _message_slice * to = _message_alloc ();
   _message_copy (to, from);
   return to;
}




