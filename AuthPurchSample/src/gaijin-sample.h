#ifndef __GAIJIN_NET_SAMPLE__
#define __GAIJIN_NET_SAMPLE__
#pragma once


#include <string>

#include <yuplay2_session.h>


typedef IYuplay2Session* (YU2CALL *Yuplay2CreateSessionProc)(const char*);
typedef const char* (YU2CALL *Yuplay2StatusStringProc)(int, bool);


IYuplay2Session* createYuplay2Session(const char* client_name);
const char* yuplay2StatusString(int status);

std::string getLogin();
std::string getPassword();

bool initProcHandles();

#ifndef _WIN32
char _getch();
#endif //!_WIN32

#endif //__GAIJIN_NET_SAMPLE__
