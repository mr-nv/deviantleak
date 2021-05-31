#pragma once

#include "recv.h"
#include "enums.h"
#include "../common.h"
class ClientClass;
class IClientNetworkable;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
    CreateClientClassFn      m_pCreateFn;
    CreateEventFn            m_pCreateEventFn;
    char*                    m_pNetworkName;
	recvtable_t*               m_pRecvTable;
    ClientClass*             m_pNext;
    ClassId                  m_ClassID;
};