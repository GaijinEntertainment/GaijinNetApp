#include <iostream>
#include "server.h"
#include "gameClient.h"


int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    std::cout << "You have to pass the arguments: login(e-mail) and password" << std::endl;
    return 1;
  }

  // For simplify, interaction between client and server is realised througth the direct calls.
  // Server imitates multicliency, but supports only synchronous work.

  Server server;
  GameClient client(server);

  client.run(argv[1], argv[2]);

  return 0;
}
