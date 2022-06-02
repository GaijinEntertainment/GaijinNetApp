#pragma once
#include <string>
#include <unordered_map>
#include "gapApi/gapApi.h"
#include "gameEntitys.h"
#include "config.h"


struct GameSession
{
  SessionStats sessionStats;
  Orc currentOrc;
};


class Server
{
public:
  explicit Server(): userstatApi(config::stat_url) {};
  void startSession(const std::string & userid) const;
  SessionStats endSession(const std::string & userid) const;
  SessionStats getSessionStats(const std::string & userid) const;

  Orc generateRandomOrc(const std::string & userid) const;
  int hitCurrentOrc(const std::string & userid) const;

private:
  UserstatApi userstatApi;
  mutable std::unordered_map<std::string, GameSession> gameSessions;
};
