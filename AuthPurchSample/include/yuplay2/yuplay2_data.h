#ifndef __YUPLAY2_DATA__
#define __YUPLAY2_DATA__
#pragma once


struct IYuplay2Binary
{
  virtual void YU2VCALL free() = 0;

  virtual unsigned YU2VCALL size() = 0;
  virtual const void* YU2VCALL data() = 0;
};


struct IYuplay2String
{
  virtual void YU2VCALL free() = 0;

  virtual unsigned YU2VCALL length() = 0;
  virtual const char* YU2VCALL string() = 0;
};


struct IYuplay2Strings
{
  virtual void YU2VCALL free() = 0;

  virtual unsigned YU2VCALL count() = 0;
  virtual const char* YU2VCALL get(unsigned idx) = 0;
};


#endif //__YUPLAY2_DATA__
