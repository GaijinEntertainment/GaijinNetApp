#include <ctime>
#include "requestHelpers.h"
#include "config.h"


nlohmann::json HTTPRequestClient::parse(const std::string & json)
{
  nlohmann::json res;

  try
  {
    res = nlohmann::json::parse(json);
  }
  catch(const nlohmann::json::parse_error& e)
  {
    std::cerr << "Parse error '" << json << "':" << e.what() << std::endl;
  }

  return res;
}


std::string HTTPRequestClient::post_core(
    const std::string & path,
    const httplib::MultipartFormDataItems & form) const
{
  if (auto res = cli.Post(path.c_str(), form))
  {
    return res->body;
  }
  else
  {
    std::cerr << "Connection error occurred: " << res.error() << std::endl;
  }
  return "";
}


std::string JsonRpcClient::gen_transact_id()
{
  static time_t seed = std::time(nullptr);
  return std::to_string(++seed);
}


std::string JsonRpcClient::post_core(
    const std::string & json_rpc_body,
    const std::string & jwt,
    const std::string & userid) const
{
  httplib::Headers headers = {
    {"appid", config::appid},
    {"token", jwt},
    {"transactid", gen_transact_id()}
  };

  if (!userid.empty())
  {
    headers.emplace("userid", userid);
  }

  if (auto res = cli.Post("/json-rpc", headers, json_rpc_body, "application/json"))
  {
    return res->body;
  }
  else
  {
    std::cerr << "Connection error occurred during the call " << json_rpc_body << res.error() << std::endl;
  }
  return "";
}


nlohmann::json JsonRpcClient::make_json_rpc_body(const std::string & method, const nlohmann::json & params)
{
  return nlohmann::json({
      {"jsonrpc", "2.0"},
      {"id", gen_transact_id()},
      {"method", method},
      {"params", params}});
}


nlohmann::json JsonRpcClient::post(
    const std::string & method,
    const nlohmann::json & params,
    const std::string & jwt,
    const std::string & userid) const
{
  std::string response = post_core(make_json_rpc_body(method, params).dump(), jwt, userid);
  const nlohmann::json parsed_response = parse(response);

  if (parsed_response.contains("error"))
  {
    std::cerr << "Got error: " << parsed_response.at("error") << " ."
              << "Method " << method << ", params: " << params << std::endl;
  }

  return parsed_response;
}
