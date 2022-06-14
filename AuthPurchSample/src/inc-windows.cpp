#include <windows.h>

#include <conio.h>


#define YUPLAY2_DLL_NAME        "yuplay2.dll"


static IYuplay2Session* createYuplay2Session(const char* client_name)
{
  HMODULE yu2Dll = ::LoadLibraryA(YUPLAY2_DLL_NAME);

  if (!yu2Dll)
  {
    printf("Couldn't load " YUPLAY2_DLL_NAME "\n");
    return NULL;
  }

  typedef IYuplay2Session* (YU2CALL *Yuplay2CreateSessionProc)(const char*);

  Yuplay2CreateSessionProc Yuplay2CreateSession =
    (Yuplay2CreateSessionProc)::GetProcAddress(yu2Dll, "yuplay2_create_session");

  IYuplay2Session* ret = Yuplay2CreateSession(client_name);

  ::FreeLibrary(yu2Dll);

  return ret;
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
