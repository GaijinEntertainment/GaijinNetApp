#ifndef __YUPLAY2_DEF__
#define __YUPLAY2_DEF__
#pragma once


//Functions calling convention
#ifndef YU2CALL
#define YU2CALL     __stdcall
#endif //YU2CALL


//Virtual methods calling convention
#ifndef YU2VCALL
#define YU2VCALL    __stdcall
#endif //YU2VCALL


//Dllexport if library was compiled as .dll
#ifdef YU2_LIBRARY_DLL
#ifdef YU2_LIBRARY_DLL_EXPORT
#define YU2DLL __declspec(dllexport)
#else
#define YU2DLL __declspec(dllimport)
#endif //YU2_LIBRARY_DLL_EXPORT
#else
#define YU2DLL
#endif //YU2_LIBRARY_DLL


//IYuplay2Session allowed to use eastl::string
#ifndef YU2EASTL_STRING
#define YU2EASTL_STRING                 0
#endif //YU2EASTL_STRING


//IYuplay2UserProc version
#ifndef IYUPLAY2_USER_PROC_VER
#define IYUPLAY2_USER_PROC_VER          2
#endif //IYUPLAY2_USER_PROC_VER


//IYuplay2ItemProc version
#ifndef IYUPLAY2_ITEM_PROC_VER
#define IYUPLAY2_ITEM_PROC_VER          1
#endif //IYUPLAY2_ITEM_PROC_VER


//IYuplay2GameProc version
#ifndef IYUPLAY2_GAME_PROC_VER
#define IYUPLAY2_GAME_PROC_VER          1
#endif //IYUPLAY2_GAME_PROC_VER


//IYuplay2Session version
#ifndef IYUPLAY2_SESSION_VER
#define IYUPLAY2_SESSION_VER            1
#endif //IYUPLAY2_SESSION_VER


//int64_t definition
#ifndef YUPLAY2_HAVE_INT64_T
#if _WIN32
typedef __int64   int64_t;
#endif
#define YUPLAY2_HAVE_INT64_T            1
#endif //YUPLAY2_HAVE_INT64_T

#endif //__YUPLAY2_DEF__
