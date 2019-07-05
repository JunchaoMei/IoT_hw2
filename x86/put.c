#include <stdio.h>
#include <curl/curl.h>
 
int main(int argc, char **argv)
{
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://postb.in/1562368510601-9907525288872");
    
    //==============unique code2 start=================
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "junchao's PUT"); /* data goes here */
    //===============unique code2 end==================

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    curl_easy_cleanup(curl);
  }
 
  return 0;
}