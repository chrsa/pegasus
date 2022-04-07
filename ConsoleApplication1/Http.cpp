#include "Http.h"

#include <curl/curl.h>
#include <string>
#include <stdexcept>

Http::Http()
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);

    /* Check for errors */
    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("Http Module couldn't be initialized: ") + curl_easy_strerror(res));
    }
    
}

Http::~Http()
{
    curl_global_cleanup();
}

size_t Http::callback(void* ptr, size_t, size_t nmemb, void* userp)
{
    if (!nmemb)
    {
        return nmemb;
    }

    std::string* mem = (std::string*)userp;

    size_t oldSize = mem->size();
    mem->resize(mem->size() + nmemb);
    std::memcpy(&(mem->operator[](0)) + oldSize, ptr, nmemb);

    return nmemb;
}

std::string Http::request(int &httpStatus, const std::string &url, const std::string& Data)
{
    CURL* curl;
    CURLcode res = CURLE_OK;

    std::string writeData;

    /* get a curl handle */
    curl = curl_easy_init();

    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: */*");


        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // 

     

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeData);

        if (!Data.empty())
        {
            /* Now specify we want to POST data */
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Data.c_str());
        }

        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }


    return writeData;
}