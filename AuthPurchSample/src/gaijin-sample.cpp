#include "gaijin-sample.h"

#include <stdint.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
  //Create user session
  IYuplay2Session* yuplay2 = createYuplay2Session("Gaijin.Net Sample App");

  if (!yuplay2)
  {
    printf("Couldn't create session\n");
    return 1;
  }

  printf("Session created\n");

  //Ask for login and password
  std::string login;
  std::string password;
  std::string totp;

  printf("Login: ");
  login = ::getLogin();

  printf("Password: ");
  password = ::getPassword();

  Yuplay2Status res;

  //Check login/password. This time we don't know is 2-step enabled for this account or not, so
  //check only login and password.
  //false in 3rd parameter forces authorization server to ask for 2-step code if 2-step was
  //enabled for this account. It is like check "Not my computer" checkbox in game UI.
  //If 3rd parameter is true, after first successful TOTP authorization machine ID will be
  //added to trusted computers list for this account and 2-step code will not be asked for some time
  res = yuplay2->loginSync(login.c_str(), password.c_str(), false);

  //2-step code required. We have to ask about it and send with login and password
  if (res == YU2_2STEP_AUTH)
  {
    printf("2-step code: ");
    totp = ::getLogin();

    res = yuplay2->twoStepLoginSync(login.c_str(), password.c_str(), totp.c_str(), false);

    if (res != YU2_OK)
    {
      printf("Couldn't check authorization of %s: %s\n", login.c_str(),
        ::yuplay2StatusString(res));

      yuplay2->free();
      return 1;
    }
  }
  //Authorization failed for some reason
  else if (res != YU2_OK)
  {
    printf("Couldn't check authorization of %s: %s\n", login.c_str(),
      ::yuplay2StatusString(res));

    yuplay2->free();
    return 1;
  }

  printf("Authorization succsessful\n");

  //Get some basic user information
  IYuplay2UserInfo* userInfo = NULL;

  res = yuplay2->user()->getUserInfoSync(&userInfo);

  if (res != YU2_OK)
  {
    printf("Couldn't get user info of %s: %s\n", login.c_str(),
      ::yuplay2StatusString(res));

    yuplay2->free();
    return 1;
  }

  printf("User ID: %lld\n", (long long int)userInfo->getId());
  printf("Nick: %s\n", userInfo->getNick());

  userInfo->free();

  //GUIDs to check purchase count
  const char* guids[] = {
    "CFAF36F2-4448-478E-BA0C-C0DC5C829BE1" //Gaijin.Net Developer item
    //Add more GUIDs of your game items in this array
  };

  unsigned guidsCnt = sizeof(guids) / sizeof(guids[0]);

  //Check purchase count of selected items
  IYuplay2ItemPurchases* purch = NULL;
  res = yuplay2->item()->getMultiPurchasesCountSync(guids, guidsCnt, &purch);

  if (res != YU2_OK)
  {
    printf("Couldn't check purchases of %u items: %s\n", guidsCnt, ::yuplay2StatusString(res));

    yuplay2->free();
    return 1;
  }

  //Also get basic items info
  IYuplay2ItemsInfo* itemInfo = NULL;
  res = yuplay2->item()->getMultiItemsInfoSync(guids, guidsCnt, &itemInfo);

  if (res != YU2_OK)
  {
    printf("Couldn't get info of %u items: %s\n", guidsCnt, ::yuplay2StatusString(res));

    purch->free();
    yuplay2->free();
    return 1;
  }

  //List purhase count with item info
  for (unsigned i = 0; i < purch->getGuidsCount(); ++i)
  {
    const char* guid = purch->getGuid(i);
    IYuplay2ItemInfoBase* item = itemInfo->itemInfo(guid);

    printf("GUID: %s\n", guid);
    printf("Title: %s\n", item->getTitle());
    printf("URL: %s\n", item->getUrl());
    printf("Purchase count: %d\n\n", purch->purchaseCount(guid));
  }

  purch->free();
  itemInfo->free();

  yuplay2->free();

  return 0;
}
