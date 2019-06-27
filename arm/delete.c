#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    CURL *curl;
    CURLcode res;
    struct stat file_info;
    double speed_upload, total_time;
    FILE *fd;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl,CURLOPT_CUSTOMREQUEST,"DELETE");
        curl_easy_setopt(curl, CURLOPT_URL,"http://192.168.122.1:8000/");
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"username=unknown&password=unknown");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        }
        else {
            printf("data received");

        }

        curl_easy_cleanup(curl);

    }

    return 0;

}