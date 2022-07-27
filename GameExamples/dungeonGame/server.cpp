#include "server.h"
#include "config.h"
#include "gapApi/gapApi.h"


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
