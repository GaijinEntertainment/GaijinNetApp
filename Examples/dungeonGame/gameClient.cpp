#include <iostream>
#include <limits>
#include <iomanip>
#include "gameClient.h"
#include "config.h"
#include "gapApi/gapApi.h"


template <typename T> static T get_stat_from_resp(
    const nlohmann::json & resp,
    const std::string & stat_name,
    T default_value,
    const std::string & table = "global",
    const std::string & mode = "default")
{
  try
  {
    return resp.at("result").at("stats").at(table).at(mode).at(stat_name).get<T>();
  }
  catch(const nlohmann::json::out_of_range)
  {
    return default_value;
  }
};


nlohmann::json get_unlock_from_resp(
    const nlohmann::json & resp,
    const std::string & unlock)
{
  try
  {
    return resp.at("result").at("unlocks").at(unlock);
  }
  catch(const nlohmann::json::out_of_range)
  {
    return nlohmann::json::object();
  }
};


void GameClient::run()
{
  if (login())
  {
    nlohmann::json stats = userstatApi.getStats(login_jwt);
    Player player;
    player.level = get_stat_from_resp<int>(stats, "level", player.level);

    server.startSession(userid);
    play(player);
    server.endSession(userid);

    grantRewards(player);
    showLeaderboard();
  }
}


bool GameClient::login()
{
  std::cout << "Waiting for the authorisation. "
               "Please, check your browser for the login page and switch back." << std::endl;
  if(!authApi.web_login())
    return false;

  login_jwt = authApi.getJwtToken();
  userid = authApi.getId();
  gjnick = authApi.getNick();

  return true;
}


void GameClient::grantRewards(Player & player) const
{
  // check auto rewarding
  nlohmann::json stats = userstatApi.getStats(login_jwt);
  int new_level = get_stat_from_resp<int>(stats, "level", player.level);

  if (new_level > player.level)
  {
    std::cout << "Congratulations! You have reached a new level: " << new_level << std::endl;
    player.level = new_level;
  }

  // manual rewarding
  nlohmann::json unlocks = userstatApi.getUnlocks(login_jwt);
  nlohmann::json gems_unlock = get_unlock_from_resp(unlocks, "gems");

  if (gems_unlock.empty())
  {
    return;
  }

  if (gems_unlock.at("lastRewardedStage").get<int>() < gems_unlock.at("stage").get<int>())
  {
    std::cout << "You have awards, receive now? (y/n)" << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    char decision;
    std::cin >> decision;

    int old_count, new_count;

    switch (decision)
    {
    case 'y':
      old_count = get_stat_from_resp<int>(stats, "gems", 0);
      rewardUnlockFull("gems", gems_unlock);
      stats = userstatApi.getStats(login_jwt);
      new_count = get_stat_from_resp<int>(stats, "gems", 0);
      std::cout << "You have received " << (new_count - old_count) << " gems!" << std::endl;
      break;
    default:
      break;
    }
  }
}


void GameClient::showLeaderboard() const
{
  const int lbCount = 10;
  nlohmann::json leaderboard = leaderboardApi.getLeaderboard("kills", "global", "default", 1,
                                                                lbCount, login_jwt).at("result");

  std::vector<nlohmann::json> orderedLb(lbCount);

  for (nlohmann::json::iterator it = leaderboard.begin(); it != leaderboard.end(); ++it)
  {
    const int place = it.value().at("idx");

    if (place >= lbCount)
    {
      std::cerr << "Can't show leaderboard: wrong data received." << std::endl;
      return;
    }
    else
    {
      nlohmann::json playerData({
        {"player_name", it.key()},
        {"total_kills", it.value().at("kills").at("value_total")}});
      orderedLb[place] = playerData;
    }
  }

  std::cout << std::endl << "At the exit written the exploits of great heroes." << std::endl;
  std::cout << std::setw(20) << std::left << "Name" << "Defeated enemies" << std::endl;

  for (auto it = orderedLb.begin(); it != orderedLb.end(); ++it)
  {
    if(!it->empty())
    {
      std::cout << std::setw(20) << std::left << it->at("player_name").get<std::string>()
                                              << it->at("total_kills").get<int>() << std::endl;
    }
  }
}


void GameClient::rewardUnlockFull(const std::string & name, nlohmann::json & unlock) const
{
  while(unlock.at("lastRewardedStage").get<int>() < unlock.at("stage").get<int>())
  {
    nlohmann::json unlocks = userstatApi.rewardUnlock(name, unlock.at("stage").get<int>(), login_jwt);
    unlock = get_unlock_from_resp(unlocks, name);
  }
}


void GameClient::play(Player & player)
{
  std::cout << std::endl << "[" << gjnick << ", " << "level " << player.level << "]" << std::endl;
  std::cout << "You are entering the terrible dungeon..." << std::endl << std::endl;

  bool finishSession = false;

  while (!finishSession)
  {
    Orc orc = server.generateRandomOrc(userid);
    std::cout << std::endl << "Moving through a dark tonnel you meet a " << orc.description << std::endl;
    std::cout << "What will you do?.. (input [k] to kill, any to get back) " << std::endl;

    char decision;
    std::cin >> decision;

    switch (decision)
    {
    case 'k':
      // for simplify expect immediate kill
      if (!server.hitCurrentOrc(userid))
      {
        std::cout << "Your only hit kills the enemy!" << std::endl;
        SessionStats currenStats = server.getSessionStats(userid);
        player.kills = currenStats.kills;
        player.exp = currenStats.exp;
        std::cout << "Kills: " << player.kills << ", Expirience: " << player.exp << "." << std::endl;
      }
      break;
    default:
      finishSession = true;
      break;
    }
  }

  std::cout << "You are leaving the dungeon, it was a glorious adventure!" << std::endl;
}
