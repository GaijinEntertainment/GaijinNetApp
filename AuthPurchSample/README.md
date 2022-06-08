Authorization and purchase sample
===

This sample demonstrates how to check Gaijin.Net authorization and purchase count using yuplay2 API library.

### Overview

yuplay2 library offers its API via session object. One should create this session via `yuplay2_create_session()` function or similar.

After session created it may be authorized and used to retrieve some user-specific data like basic profile or purchases. This sample demonstrates authorization with login and password.

#### Synchronous and asynchronous API

Most of API methods have both synchronous (using `Sync` suffix) and asynchronous (using `Async` suffix) versions.

Synchronous method lock thread until API answer received and return `Yuplay2Status` with status of operation. `YU2_OK` means success.

```cpp
Yuplay2Status result = yuplay2->loginSync(login, password, false);

if (result == YU2_OK)
{
  //Authorization successful, let's check game purchases
}
else
{
  printf("Couldn't check authorization: %s\n", ::yuplay2_status_string(result));
}
```

Asynchronous method run separate thread and send its result in `IYuplay2Cb::onMessage()` callback. It return `Yuplay2Status` with status of thread start. `YU2_OK` means thread was run successfully and callback will be called with thread work result.

```cpp
class LoginStatus : public IYuplay2Cb
{
public:
  bool complete() const { return answerReceived.load(std::memory_order_acquire); }
  int status() const { return loginStatus.load(std::memory_order_acquire); }

private:
  std::atomic<bool> answerReceived = { false };
  std::atomic<int> loginStatus = { YU2_FAIL };

  virtual void YU2VCALL onMessage(int msg, Yuplay2Handle req, int status, const void* data)
  {
    if (msg == YUPLAY2_CHECK_LOGIN || msg == YUPLAY2_2STEP_LOGIN)
    {
      loginStatus.store(status, std::memory_order_release);
      answerReceived.store(true, std::memory_order_release);
    }
  }
};


LoginStatus loginStatus;
Yuplay2AsyncCall call = { &loginStatus };

Yuplay2Status result = yuplay2->loginAsync(login, password, false, &call);

if (result != YU2_OK)
{
  //YU2_BUSY -- login action is already running and not yet complete
  //YU2_WRONG_PARAMETER -- some function parameters is bad. Empty string or something like this

  printf("Couldn't check authorization: %s\n", ::yuplay2_status_string(result));
  return false;
}

while (!loginStatus.complete())
{
  //Do something useful while answer not received
}

if (loginStatus.status() == YU2_OK)
{
  //Authorization successful, let's check game purchases
}
else if (loginStatus.status() == YU2_2STEP_AUTH)
{
  //2-step code required. Ask user for it and call twoStepLoginAsync()
}
else
{
  printf("Couldn't check authorization: %s\n", ::yuplay2_status_string(result));
}

```

### Creaing yuplay2 session

Each yuplay2 session must be created via `yuplay2_create_session()` function or similar.

```cpp
IYuplay2Session* yuplay2 = ::yuplay2_create_session("Gaijin.Net Sample App");
```
Don't forget to free session after use.

```cpp
yuplay2->free();
```

`free()` stops all active asyncronous API requests.

### Checking authorization

Gaijin.Net password authorization use obligatory login and password with optional TOTP 2-step code required for some accounts.

First we try to check login and password only. If this account have no 2-step assigned or this computer was authorized as trusted, TOTP will not be required for authorization.

```cpp
Yuplay2Status res;

res = yuplay2->loginSync(login.c_str(), password.c_str(), false);
```

Operation result `YU2_2STEP_AUTH` means autorization server needs additional TOTP code. Ask user for it and use with `twoStepLogin`

```cpp
if (res == YU2_2STEP_AUTH)
{
  printf("2-step code: ");
  totp = getLogin();

  res = yuplay2->twoStepLoginSync(login.c_str(), password.c_str(), totp.c_str(), false);

  if (res != YU2_OK)
  {
    printf("Couldn't check authorization of %s: %s\n", login.c_str(),
      ::yuplay2_status_string(res));

    yuplay2->free();
    return 1;
  }
}
```

`YU2_OK` status from `login` or `twoStepLogin` means session was authorized successfully.

### Get basic user info

User API methods are provided by `IYuplay2UserProc` interface provided by `IYuplay2Session::user()`.

Basic profile is implemented by `IYuplay2UserInfo` interface and include user ID, UTF-8 nick and tags. It may be received by `getUserInfo` method.

```cpp
IYuplay2UserInfo* userInfo = NULL;

res = yuplay2->user()->getUserInfoSync(&userInfo);

if (res != YU2_OK)
{
  printf("Couldn't get user info of %s: %s\n", login.c_str(),
    ::yuplay2_status_string(res));

  yuplay2->free();
  return 1;
}

printf("User ID: %lld\n", userInfo->getId());
printf("Nick: %s\n", userInfo->getNick());

userInfo->free();
```

### Check purchase

Game items API methods are provided by `IYuplay2ItemProc` interface provided by `IYuplay2Session::item()`.

It is better to check all game items by one request so we use `getMultiPurchasesCount` method. It accepts array of item GUIDs and returns `IYuplay2ItemPurchases` object to check purchase count of each GUID.

```cpp
//GUIDs to check purchase count
const char* guids[] = {
  "CFAF36F2-4448-478E-BA0C-C0DC5C829BE1" //Gaijin.Net Developer item
  //Add more GUIDs of your game items in this array
};

unsigned guidsCnt = sizeof(guids) / sizeof(guids[0]);

IYuplay2ItemPurchases* purch = NULL;
res = yuplay2->item()->getMultiPurchasesCountSync(guids, guidsCnt, &purch);

if (res != YU2_OK)
{
  printf("Couldn't check purchases of %u items: %s\n", guidsCnt, ::yuplay2_status_string(res));

  yuplay2->free();
  return 1;
}

//List purhase count
for (unsigned i = 0; i < purch->getGuidsCount(); ++i)
{
  const char* guid = purch->getGuid(i);

  printf("GUID: %s\n", guid);
  printf("Purchase count: %d\n\n", purch->purchaseCount(guid));
}

purch->free();
```

### Get basic item info

Basic item info is implemented by `IYuplay2ItemInfoBase` interface and include GUID, title, store URL and short description.

It is better to receive multiple items by one request, so we use `getMultiItemsInfo` method. It accepts array of item GUIDs and returns `IYuplay2ItemsInfo` object to get info about each GUID.

```cpp
IYuplay2ItemsInfo* itemInfo = NULL;
res = yuplay2->item()->getMultiItemsInfoSync(guids, guidsCnt, &itemInfo);

if (res != YU2_OK)
{
  printf("Couldn't get info of %u items: %s\n", guidsCnt, ::yuplay2_status_string(res));

  yuplay2->free();
  return 1;
}

//List items
for (unsigned i = 0; i < itemInfo->getGuidsCount(); ++i)
{
  const char* guid = itemInfo->getGuid(i);
  IYuplay2ItemInfoBase* item = itemInfo->itemInfo(guid);

  printf("GUID: %s\n", guid);
  printf("Title: %s\n", item->getTitle());
  printf("URL: %s\n", item->getUrl());
}

itemInfo->free();
```
