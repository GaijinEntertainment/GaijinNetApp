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
        const int64_t userid,
        const std::string & jwt) const;
  private:
    nlohmann::json post(
        const std::string & method,
        nlohmann::json & body,
        const std::string & jwt,
        const int64_t userid = NULL_USERID) const;
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
    explicit AuthorizationApi(const std::string & auth_url): httpClient(auth_url)
    {
      httpClient.set_read_timeout(60, 0);
    };

    bool web_login();

    const std::string & getJwtToken() {return jwt_token;};
    int64_t getId() {return userid;};
    const std::string & getNick() {return gjnick;};
  private:
    std::string getGsid(const nlohmann::json & optionals = nlohmann::json::object()) const;
    nlohmann::json getGsidResult(std::string gsid);

    void openBrowser(const std::string & url);

    std::string jwt_token;
    int64_t userid;
    std::string gjnick;

    HTTPRequestClient httpClient;
};
