#pragma once
#include <string>
#include "libs/nlohmann/json.hpp"


namespace config
{
  const std::string login_url = "https://login.gaijin.net:443";
  const std::string stat_url = "http://test-char-01.gaijin.lan:13080";
  const std::string leaderboard_url = "http://moon-contact.gaijin.ops:7443";

  const std::string appid = "1197";
  const unsigned app_perm_id = 50017;

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
