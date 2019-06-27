/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * HTTP PUT with easy interface and read callback
 * </DESC>
 */ 
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>
 
/*
 * This example shows a HTTP PUT operation. PUTs a file given as a command
 * line argument to the URL also given on the command line.
 *
 * This example also uses its own read callback.
 *
 * Here's an article on how to setup a PUT handler for Apache:
 * http://www.apacheweek.com/features/put
 */ 
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
  retcode = fread(ptr, size, nmemb, stream);
  nread = (curl_off_t)retcode;
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
  return retcode;
}
 
int main(int argc, char **argv)
{
  if(argc < 3)
    return 1;

  CURL *curl;
  CURLcode res;

  //==============unique code1 start=================
  FILE *hd_src;
  struct stat file_info;
  char *file;
  char *url;
  file = argv[1];
  url = argv[2];
  stat(file, &file_info);
  hd_src = fopen(file, "rb");
  //==============unique code1 end=================
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    //==============unique code2 start=================
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
    //===============unique code2 end==================

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    curl_easy_cleanup(curl);
  }

  fclose(hd_src); //==============unique code3================= 
 
  return 0;
}