#include "gaijin-sample.h"

#if _WIN32
#include <conio.h>
#endif //_WIN32

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


static std::string readConsoleString(char echo_char)
{
  std::string ret;

  for (;;)
  {
    switch (char c = ::_getch())
    {
      case '\r':
        printf("\n");
        return ret;

      case '\b':
        if (ret.length())
        {
          printf("\b \b");
          ret.erase(ret.length() - 1, 1);
        }
        break;

      case 0x03:
        ::exit(1);
        break;

      default:
        ret += c;
        printf("%c", echo_char ? echo_char : c);
    }
  }

  return ret;
}


std::string getLogin()
{
  return readConsoleString(0);
}


std::string getPassword()
{
  return readConsoleString('*');
}

