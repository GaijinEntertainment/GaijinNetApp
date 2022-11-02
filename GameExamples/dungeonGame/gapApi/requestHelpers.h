#pragma once
#include <string>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "libs/httplib.h"
#include "libs/nlohmann/json.hpp"


static constexpr int64_t NULL_USERID = 0;


void sleep_sec(unsigned timeout_sec);


class HTTPRequestClient
{
  public:
    explicit HTTPRequestClient(const std::string & url): cli(url) {};

    static nlohmann::json parse(const std::string & json);

    inline void set_read_timeout(time_t sec, time_t usec){cli.set_read_timeout(sec, usec);};
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

    static nlohmann::json make_json_rpc_object(
        const std::string & method,
        const nlohmann::json & params = nlohmann::json::object());

    std::string post_core(const std::string & json_rpc_object) const;

    nlohmann::json post(
        const std::string & method,
        nlohmann::json & params,
        const std::string & jwt,
        const int64_t userid = NULL_USERID) const;
};
