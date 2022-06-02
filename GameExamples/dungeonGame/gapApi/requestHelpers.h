#pragma once
#include <string>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "libs/httplib.h"
#include "libs/nlohmann/json.hpp"


class HTTPRequestClient
{
  public:
    explicit HTTPRequestClient(const std::string & url): cli(url) {};

    static nlohmann::json parse(const std::string & json);

    std::string post_core(
        const std::string & path,
        const httplib::MultipartFormDataItems & form) const;
  protected:
    mutable httplib::Client cli;
};


class JsonRpcClient: public HTTPRequestClient
{
  public:
    explicit JsonRpcClient(const std::string & url): HTTPRequestClient(url) {};

    static std::string gen_transact_id();

    static nlohmann::json make_json_rpc_body(
        const std::string & method,
        const nlohmann::json & params = nlohmann::json::object());

    std::string post_core(
        const std::string & json_rpc_body,
        const std::string & jwt,
        const std::string & userid = "") const;

    nlohmann::json post(
        const std::string & method,
        const nlohmann::json & params,
        const std::string & jwt,
        const std::string & userid = "") const;
};
