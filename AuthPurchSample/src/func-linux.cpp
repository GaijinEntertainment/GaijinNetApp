#include "gaijin-sample.h"

#include <termios.h>
#include <dlfcn.h>


#define YUPLAY2_SO_NAME           "yuplay2.so"
#define YUPLAY2_CREATE_SESSION    "yuplay2_create_session"
#define YUPLAY2_STATUS_STRING     "yuplay2_status_string"


extern Yuplay2CreateSessionProc yuplay2CreateSessionProc;
extern Yuplay2StatusStringProc yuplay2StatusStringProc;


bool initProcHandles()
{
  typedef IYuplay2Session* (YU2CALL *Yuplay2CreateSessionProc)(const char*);

  void* yu2So = dlopen("./" YUPLAY2_SO_NAME, RTLD_LOCAL | RTLD_LAZY);

  if (!yu2So)
  {
    printf("Couldn't find or load " YUPLAY2_SO_NAME "\n");
    return false;
  }

  yuplay2CreateSessionProc =
    (Yuplay2CreateSessionProc)::dlsym(yu2So, YUPLAY2_CREATE_SESSION);

  if (!yuplay2CreateSessionProc)
  {
    printf("Couldn't locate " YUPLAY2_CREATE_SESSION "() in " YUPLAY2_SO_NAME "\n");
    return false;
  }

  yuplay2StatusStringProc =
    (Yuplay2StatusStringProc)::dlsym(yu2So, YUPLAY2_STATUS_STRING);

  if (!yuplay2StatusStringProc)
  {
    printf("Couldn't locate " YUPLAY2_STATUS_STRING "() in " YUPLAY2_SO_NAME "\n");
    return false;
  }

  return true;
}


static char _getch()
{
  static bool terminalSettingsSet = false;

  if (!terminalSettingsSet)
  {
    struct termios opt;

    ::tcgetattr(0, &opt);

    opt.c_lflag &= ~ICANON;
    opt.c_lflag &= ~ECHO;

    ::tcsetattr(0, TCSANOW, &opt);

    terminalSettingsSet = true;
  }

  return ::getchar();
}


std::string readConsoleString(char echo_char)
{
  std::string ret;

  for (;;)
  {
    switch (char c = ::_getch())
    {
      case '\n':
        printf("\n");
        return ret;

      case 0x7F:
        if (ret.length())
        {
          printf("\b \b");
          ret.erase(ret.length() - 1, 1);
        }
        break;

      default:
        ret += c;
        printf("%c", echo_char ? echo_char : c);
    }
  }

  return ret;
}

