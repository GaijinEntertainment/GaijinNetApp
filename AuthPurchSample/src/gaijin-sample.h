#ifndef __GAIJIN_NET_SAMPLE__
#define __GAIJIN_NET_SAMPLE__
#pragma once


#include <string>

#include <yuplay2_session.h>


typedef IYuplay2Session* (YU2CALL *Yuplay2CreateSessionProc)(const char*);
typedef const char* (YU2CALL *Yuplay2StatusStringProc)(int, bool);


IYuplay2Session* createYuplay2Session(const char* client_name);
const char* yuplay2StatusString(int status);

std::string readConsoleString(char echo_char);
std::string getLogin();
std::string getPassword();

bool initProcHandles();

#endif //__GAIJIN_NET_SAMPLE__
