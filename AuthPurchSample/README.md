Authorization and purchase sample
===

This sample demonstrates how to check Gaijin.Net authorization and purchase count using yuplay2 API library

### Overview

yuplay2 library offers its API via session object. One should create this session with `yuplay2_create_session()` function or similar.

After session created it may be authorized and used to retrieve some user-specific data like basic profile or purchases. This sample demonstrates authorization with login and password.

#### Synchronous and asynchronous API

Most of API methods have both synchronous (using `Sync` suffix) and asynchronous (using `Async` suffix) versions.

Synchronous method lock thread until API answer received and return `Yuplay2Status` with status of operation. `YU2_OK` means success.

```
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

```
struct LoginStatus : public IYuplay2Cb
{
  Yuplay2Status status = YU2_FAIL;
  bool answerReceived = false;
  bool loggedIn = false;

  virtual void YU2VCALL onMessage(int msg, Yuplay2Handle req, int status, const void* data)
  {
    if (msg == YUPLAY2_CHECK_LOGIN || msg == YUPLAY2_2STEP_LOGIN)
    {
      status = status;
      answerReceived = true;

      if (status == YU2_OK)
        loggedIn = true;
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

while (!loginStatus.answerReceived)
{
  //Do something useful while answer not received
}

if (loginStatus.status == YU2_OK)
{
  //Authorization successful, let's check game purchases
}
else
{
  printf("Couldn't check authorization: %s\n", ::yuplay2_status_string(result));
}

```
