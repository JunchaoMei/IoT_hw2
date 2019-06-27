#include <stdio.h>
#include <curl/curl.h>

#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2

#define myURL       "http://localhost:8000"

int main(void)
{
    CURL        *curl;
    CURLcode    res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, myURL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(curl);
        long http_status = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status);
        printf("%s %ld\n", "Return HTTP Status: ",http_status);;
        if (res != CURLE_OK)
        {
        	return REQ_ERR;
        }
        curl_easy_cleanup(curl);
    } else
    {
    	return INIT_ERR;
    }
    return OK;
}
