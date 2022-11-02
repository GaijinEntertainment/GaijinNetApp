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
  void startSession(const int64_t userid) const;
  SessionStats endSession(const int64_t userid) const;
  SessionStats getSessionStats(const int64_t userid) const;

  Orc generateRandomOrc(const int64_t userid) const;
  int hitCurrentOrc(const int64_t userid) const;

private:
  UserstatApi userstatApi;
  mutable std::unordered_map<int64_t, GameSession> gameSessions;
};
