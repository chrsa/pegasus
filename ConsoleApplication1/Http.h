#pragma once

#include <string>

class Http
{
public:

    Http();

    static size_t callback(void* ptr, size_t, size_t nmemb, void* userp);

    std::string request(int &httpStatus, const std::string &url, const std::string& data);

    ~Http();
};

