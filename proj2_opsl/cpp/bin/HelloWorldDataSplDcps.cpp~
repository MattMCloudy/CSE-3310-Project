#include "HelloWorldDataSplDcps.h"
#include "ccpp_HelloWorldData.h"
#include "dds_type_aliases.h"

const char *
__HelloWorldData_Msg__name(void)
{
    return (const char*)"HelloWorldData::Msg";
}

const char *
__HelloWorldData_Msg__keys(void)
{
    return (const char*)"userID";
}

#include <v_kernel.h>
#include <v_topic.h>
#include <os_stdlib.h>
#include <string.h>
#include <os_report.h>

c_bool
__HelloWorldData_Msg__copyIn(
    c_base base,
    struct ::HelloWorldData::Msg *from,
    struct _HelloWorldData_Msg *to)
{
    c_bool result = OS_C_TRUE;
    (void) base;

    to->userID = (c_long)from->userID;
#ifdef OSPL_BOUNDS_CHECK
    if(from->message){
        to->message = c_stringNew(base, from->message);
    } else {
        OS_REPORT (OS_ERROR, "copyIn", 0,"Member 'HelloWorldData::Msg.message' of type 'c_string' is NULL.");
        result = OS_C_FALSE;
    }
#else
    to->message = c_stringNew(base, from->message);
#endif
    return result;
}

void
__HelloWorldData_Msg__copyOut(
    void *_from,
    void *_to)
{
    struct _HelloWorldData_Msg *from = (struct _HelloWorldData_Msg *)_from;
    struct ::HelloWorldData::Msg *to = (struct ::HelloWorldData::Msg *)_to;
    to->userID = (::DDS::Long)from->userID;
    to->message = DDS::string_dup(from->message ? from->message : "");
}

