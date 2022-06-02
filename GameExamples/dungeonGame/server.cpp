#include "server.h"
#include "config.h"
#include "gapApi/gapApi.h"


namespace config
{
  const std::string server_jwt =
      "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiI1NCIsImV4cCI6MTg5NDY2NTYwMCwiaGFzaCI"
      "6IjU1N2M5YTc4M2VlNzE5MjkzYWE1YmJiOWUwNjM3ZjYxIiwiaWF0IjoxNjUzNDg5MTIyLCJpc3MiOiIxIiw"
      "ia2lkIjoiMzgxNDc2IiwibG5nIjoicnUiLCJuaWNrIjoiZHVuZ2VvblRlc3RCb3QiLCJzbHQiOiJha0JkT3p"
      "aWCIsInRncyI6IjJzdGVwLGxhbmdfcnUsc3NvIiwidWlkIjoiMTMyNzc0MjgzIn0.CGctPuoL9QGn6MgDCIa"
      "NxaxAuQI9WXNWpvnJ2C08ai843k2Chz873pI-jZ-ulbB-GAL_w611ktha_5-RIJ8TCQXNpMcVbzk7EWH5qSS"
      "91xWBLw4YYftGMfi-CVSpOvTecmjbpJdro2YLu5CHe6vSYlywwZpd_EH-nxn0m5D-ZGvHeuKWT5eatzqUe-H"
      "z63kWlwa3Fh-zpGOZOJGGXOOI9Y9BCc4nNlFGx5Ek6n8-ZliBNV4NHEZjpTQWn-s2alqU-V1yhAIgMSxsXDx"
      "HoYADs73ZWBxHKzsfaW7f4xiFd3bWNcDitxFP5tQGURtn4SI0wKtJZghQZv1xhcelRY8X6A";
}


Orc Server::generateRandomOrc(const std::string & userid) const
{
  static int seed = 0;
  seed++;
  gameSessions[userid].currentOrc = (seed%2) ? Orc() : Orc{50, 3, "big orc"};
  return gameSessions[userid].currentOrc;
}


int Server::hitCurrentOrc(const std::string & userid) const
{
  GameSession & gameSession = gameSessions[userid];

  gameSession.currentOrc.hp = 0;
  gameSession.sessionStats.kills++;
  gameSession.sessionStats.exp += gameSession.currentOrc.gen_exp;

  return gameSession.currentOrc.hp;
}


void Server::startSession(const std::string & userid) const
{
  gameSessions.emplace(userid, GameSession());
}


SessionStats Server::endSession(const std::string & userid) const
{
  GameSession & gameSession = gameSessions[userid];

  userstatApi.changeStats("kills", gameSession.sessionStats.kills, userid, config::server_jwt);
  userstatApi.changeStats("exp", gameSession.sessionStats.exp, userid, config::server_jwt);

  SessionStats stats = gameSession.sessionStats;
  gameSessions.erase(userid);

  return stats;
}


SessionStats Server::getSessionStats(const std::string & userid) const
{
  return gameSessions[userid].sessionStats;
}
