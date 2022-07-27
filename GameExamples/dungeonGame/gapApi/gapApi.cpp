#include "requestHelpers.h"
#include "gapApi.h"
#include "config.h"


nlohmann::json UserstatApi::getStats(const std::string & jwt) const
{
  return jsonRpcClient.post("GetStats", nlohmann::json::object(), jwt);
}


nlohmann::json UserstatApi::getUnlocks(const std::string & jwt) const
{
  return jsonRpcClient.post("GetUnlocks", nlohmann::json::object(), jwt);
}


nlohmann::json UserstatApi::rewardUnlock(const std::string & unlock, int stage, const std::string & jwt) const
{
  nlohmann::json params = {
    {"unlock", unlock},
    {"stage", stage},
  };

  return jsonRpcClient.post("GrantRewards", params, jwt);
}


nlohmann::json UserstatApi::changeStats(
    const std::string & stat_name,
    int value,
    const std::string & userid,
    const std::string & jwt) const
{
  nlohmann::json params = {
    {"__body__", {
      {"$mode", {"default"}},
      {"$tables", {"global"}},
      {stat_name, value}}
    }
  };

  return jsonRpcClient.post("ChangeStats", params, jwt, userid);
}


nlohmann::json LeaderboardApi::getLeaderboard(
    const std::string & category,
    const std::string & table,
    const std::string & gameMode,
    int resolveNick,
    int count,
    const std::string & jwt) const
{
  nlohmann::json params = {
      {"table", table},
      {"gameMode", gameMode},
      {"category", category},
      {"resolveNick", resolveNick},
      {"count", count},
  };

  return jsonRpcClient.post("cln_get_leaderboard", params, jwt);
}


nlohmann::json AuthorizationApi::auth(
    const std::string & email,
    const std::string & pass,
    const std::string & code) const
{
  httplib::MultipartFormDataItems auth_form = {
    {"login", email, "", ""},
    {"password", pass, "", ""},
    {"2step", code, "", ""},
    {"app_id", config::app_id},
  };

  return httpClient.parse(httpClient.post_core("/login.php", auth_form));
}
