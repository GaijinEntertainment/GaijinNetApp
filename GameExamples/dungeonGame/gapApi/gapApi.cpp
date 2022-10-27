#include "requestHelpers.h"
#include "gapApi.h"
#include "config.h"


const std::string GSID = "gsid";
const std::string STATUS_ = "status";


nlohmann::json UserstatApi::getStats(const std::string & jwt) const
{
  nlohmann::json body;
  return post("GetStats", body, jwt);
}


nlohmann::json UserstatApi::getUnlocks(const std::string & jwt) const
{
  nlohmann::json body;
  return post("GetUnlocks", body, jwt);
}


nlohmann::json UserstatApi::rewardUnlock(const std::string & unlock, int stage, const std::string & jwt) const
{
  nlohmann::json body = {
    {"unlock", unlock},
    {"stage", stage},
  };

  return post("GrantRewards", body, jwt);
}


nlohmann::json UserstatApi::changeStats(
    const std::string & stat_name,
    int value,
    const std::string & userid,
    const std::string & jwt) const
{
  nlohmann::json body = {
    {"$mode", {"default"}},
    {"$tables", {"global"}},
    {stat_name, value}
  };

  return post("ChangeStats", body, jwt, userid);
}


nlohmann::json UserstatApi::post(
    const std::string & method,
    nlohmann::json & body,
    const std::string & jwt,
    const std::string & userid) const
{
  nlohmann::json params = {{"__body__", body}};
  return jsonRpcClient.post(method, params, jwt, userid);
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


bool AuthorizationApi::web_login()
{
  bool successLogin = false;
  std::string gsid = getGsid();
  std::string url = config::login_url + "?gsid=" + gsid;

  openBrowser(url);

  while (true)
  {
    nlohmann::json gsid_result = getGsidResult(gsid);

    if (gsid_result.empty())
    {
      std::cerr << "Login error occurred: invalid response" << std::endl;
      break;
    }
    else
    {
      try
      {
        if (gsid_result.at(STATUS_) == "OK")
        {
          jwt_token = gsid_result.at("jwt");
          userid = gsid_result.at("user_id");
          gjnick = gsid_result.at("nick");

          successLogin = true;
          break;
        }
        else if (gsid_result.at(STATUS_) == "RETRY")
        {
          sleep_sec(gsid_result.at("delay"));
        }
      }
      catch(const nlohmann::json::out_of_range& e)
      {
        std::cerr << "Response error occurred: expected field is absent: " << e.what() << '\n';
        break;
      }
    }
  }
  return successLogin;
}


std::string AuthorizationApi::getGsid(const nlohmann::json & optionals) const
{
  httplib::MultipartFormDataItems gsid_form;

  for (auto it = optionals.begin(); it != optionals.end(); ++it)
  {
    httplib::MultipartFormData field = {it.key(), it.value(), "", ""};
    gsid_form.push_back(field);
  }

  nlohmann::json result = httpClient.parse(httpClient.post_core("/proxy/api/gameSession/getGsid", gsid_form));
  return result.contains(GSID) ? result[GSID] : "";
}


nlohmann::json AuthorizationApi::getGsidResult(std::string gsid)
{
  httplib::MultipartFormDataItems form = {
    {GSID, gsid, "", ""},
  };

  nlohmann::json result = httpClient.parse(httpClient.post_core("/proxy/api/gameSession/getGsidResult", form));

  return result;
}


void AuthorizationApi::openBrowser(const std::string & url)
{
#if _WIN32
  ::ShellExecuteA(NULL, NULL, url.c_str(), NULL, NULL, SW_SHOW);
#elif __linux__
  std::string command = "xdg-open " + url;
  system(command.c_str());
#else
  std::cerr << "In-browser web auth not implemented on this platform\n";
#endif //_WIN32
}
