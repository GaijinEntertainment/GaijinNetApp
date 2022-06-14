#include "gaijin-sample.h"


Yuplay2CreateSessionProc yuplay2CreateSessionProc = NULL;
Yuplay2StatusStringProc yuplay2StatusStringProc = NULL;


IYuplay2Session* createYuplay2Session(const char* client_name)
{
  if (yuplay2CreateSessionProc || initProcHandles())
    return yuplay2CreateSessionProc(client_name);

  return NULL;
}


const char* yuplay2StatusString(int status)
{
  if (yuplay2StatusStringProc || initProcHandles())
    return yuplay2StatusStringProc(status, false);

  return "";
}


std::string getLogin()
{
  return readConsoleString(0);
}


std::string getPassword()
{
  return readConsoleString('*');
}

