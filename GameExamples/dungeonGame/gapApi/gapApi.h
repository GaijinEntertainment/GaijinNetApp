// Gaijin Application Platform API
#pragma once
#include "libs/nlohmann/json.hpp"
#include "gapApi/requestHelpers.h"


class UserstatApi
{
  public:
    explicit UserstatApi(const std::string & userstat_url): jsonRpcClient(userstat_url) {};
    nlohmann::json getStats(const std::string & jwt) const;
    nlohmann::json getUnlocks(const std::string & jwt) const;
    nlohmann::json rewardUnlock(const std::string & unlock, int stage, const std::string & jwt) const;
    nlohmann::json changeStats(
        const std::string & stat_name,
        int value,
        const std::string & userid,
        const std::string & jwt) const;
  private:
    JsonRpcClient jsonRpcClient;
};


class LeaderboardApi
{
  public:
    explicit LeaderboardApi(const std::string & leaderboard_url): jsonRpcClient(leaderboard_url) {};
    nlohmann::json getLeaderboard(
        const std::string & category,
        const std::string & table,
        const std::string & gameMode,
        int resolveNick,
        int count,
        const std::string & jwt) const;
  private:
    JsonRpcClient jsonRpcClient;
};


class AuthorizationApi
{
  public:
    explicit AuthorizationApi(const std::string & auth_url): httpClient(auth_url) {};
    nlohmann::json auth(const std::string & login, const std::string & pass, const std::string & code) const;
  private:
    HTTPRequestClient httpClient;
};
