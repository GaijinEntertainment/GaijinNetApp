#ifndef __YUPLAY2_USER__
#define __YUPLAY2_USER__
#pragma once


#include <yuplay2_def.h>


struct IYuplay2Strings;


struct IYuplay2UserBase
{
  virtual int64_t YU2VCALL getId() = 0;

  virtual const char* YU2VCALL getNick() = 0;
};


struct IYuplay2UserInfo : public IYuplay2UserBase
{
  virtual const char* YU2VCALL getToken() = 0;
  virtual IYuplay2Strings* YU2VCALL getTags() = 0;
  virtual unsigned YU2VCALL getPaymentMethods() = 0;
  virtual const char* YU2VCALL getOriginalNick() = 0;

  virtual void YU2VCALL free() = 0;
};


#endif //__YUPLAY2_USER__
