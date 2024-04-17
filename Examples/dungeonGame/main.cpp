#include <iostream>
#include "server.h"
#include "gameClient.h"


int main(int argc, char const *argv[])
{
  // For simplify, interaction between client and server is realised througth the direct calls.
  // Server imitates multicliency, but supports only synchronous work.

  Server server;
  GameClient client(server);

  client.run();

  return 0;
}
