#pragma once
#include <string>
#include "config.h"
#include "server.h"
#include "libs/nlohmann/json.hpp"
#include "gapApi/gapApi.h"


class GameClient
{
public:
  explicit GameClient(const Server & server_):
    server(server_),
    userstatApi(config::stat_url),
    leaderboardApi(config::leaderboard_url),
    authApi(config::login_url){};

  void run();

private:
  bool login();
  void play(Player & player);

  void rewardUnlockFull(const std::string & name, nlohmann::json & unlock) const;

  void grantRewards(Player & player) const;
  void showLeaderboard() const;

private:
  const Server & server;
  UserstatApi userstatApi;
  LeaderboardApi leaderboardApi;
  AuthorizationApi authApi;

  nlohmann::json loginInfo;
  std::string login_jwt;
  int64_t userid;
  std::string gjnick;
};
