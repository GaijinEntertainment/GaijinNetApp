#include "gaijin-sample.h"

#include <windows.h>

#include <conio.h>


#define YUPLAY2_DLL_NAME          "yuplay2.dll"
#define YUPLAY2_CREATE_SESSION    "yuplay2_create_session"
#define YUPLAY2_STATUS_STRING     "yuplay2_status_string"


extern Yuplay2CreateSessionProc yuplay2CreateSessionProc;
extern Yuplay2StatusStringProc yuplay2StatusStringProc;


bool initProcHandles()
{
  HMODULE yu2Dll = ::LoadLibraryA(YUPLAY2_DLL_NAME);

  if (!yu2Dll)
  {
    printf("Couldn't load " YUPLAY2_DLL_NAME "\n");
    return false;
  }

  yuplay2CreateSessionProc =
    (Yuplay2CreateSessionProc)::GetProcAddress(yu2Dll, YUPLAY2_CREATE_SESSION);

  if (!yuplay2CreateSessionProc)
  {
   ::FreeLibrary(yu2Dll);

    printf("Couldn't locate " YUPLAY2_CREATE_SESSION "() in " YUPLAY2_DLL_NAME "\n");
    return false;
  }

  yuplay2StatusStringProc =
    (Yuplay2StatusStringProc)::GetProcAddress(yu2Dll, YUPLAY2_STATUS_STRING);

  if (!yuplay2StatusStringProc)
  {
   ::FreeLibrary(yu2Dll);

    printf("Couldn't locate " YUPLAY2_STATUS_STRING "() in " YUPLAY2_DLL_NAME "\n");
    return false;
  }

  return true;
}


std::string readConsoleString(char echo_char)
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

