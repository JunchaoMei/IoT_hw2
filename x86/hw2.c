#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2

void printHelp()
{
	printf("Student:  Junchao Mei\n");
	printf("Course:  ECE-531:Internet of Things   Summer2019\n");
	printf("Assignment2:  Week3 - Network Communication on ARM\n");
	printf("Usage:  hw2 [OPTION]... [URL]...\n");
	printf("Send http GET/POST/PUT/DELETE request to some url\n");
	printf("  -h   --help	display this help info\n");
	printf("  -u   --url	the URL to receive Http request\n");
	printf("HTTP Requests:\n");
	printf("  -g   --get     GET	 eg.\"./hw2 -g/--get -u/--url [URL]\"\n");
	printf("  -o   --post    POST	 eg.\"./hw2 -o/--post [somefile] -u/--url [URL]\"\n");
	printf("  -p   --put     PUT	 eg.\"./hw2 -p/--put [something] -u/--url [URL]\"\n");
	printf("  -d   --delete  DELETE	 eg.\"./hw2 -d/--delete -u/--url [URL]\"\n");
	printf("Exit status:\n");
	printf(" 0	OK\n");
	printf(" 1	Initializing Error\n");
	printf(" 2	Requesting Error\n");
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
  retcode = fread(ptr, size, nmemb, stream);
  nread = (curl_off_t)retcode;
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
  return retcode;
}

char parseArg(int argc, char **argv, char **input)
{
	// return operation flag
	char returnValue = 'e';
	int i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0)
		{
			return 'h';
		} else if (strcmp(argv[i],"-o")==0 || strcmp(argv[i],"--post")==0 || strcmp(argv[i],"-p")==0 || strcmp(argv[i],"--put")==0)
		{
			if (i <= argc-2)
			{
				if (strcmp(argv[i],"-o")==0 || strcmp(argv[i],"--post")==0)
				{
					input[1] = argv[i+1];
					returnValue = 'o';
				} else
				{
					input[2] = argv[i+1];
					returnValue = 'p';
				}
			} else
			{
				return 'e';
			}
		} else if (strcmp(argv[i],"-g")==0 || strcmp(argv[i],"--get")==0)
		{
			returnValue = 'g';
		} else if (strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--delete")==0)
		{
			returnValue = 'd';
		} else if (strcmp(argv[i],"-u")==0 || strcmp(argv[i],"--url")==0)
		{
			if (i <= argc-2)
			{
				input[0] = argv[i+1];
			} else
			{
				return 'e';
			}
		}

		i++;
	}
	return returnValue;
}

void setupGET(CURL *curl)
{
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
}

void setupPOST(CURL *curl, char *postthis)
{
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
}

void setupPUT(CURL *curl, struct stat file_info, FILE *hd_src)
{
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
}

void setupDELETE(CURL *curl)
{
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
}

int main(int argc, char **argv)
{
	int UnixStandardStatus = OK;
	char *input[3] = {"url","(something)","(somefile)"};
	char operation = parseArg(argc, argv, input); //initialize input[]

	// general initialization for GET POST PUT DELETE
    CURL        *curl;
    CURLcode    res;
    // special initialization for PUT
    if (strcmp(input[2],"(somefile)")==0) //not initialized
    	input[2] = "makefile"; //default file to use
    FILE *hd_src;  hd_src = fopen(input[2], "rb");
    struct stat file_info;  stat(input[2], &file_info);
	
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, input[0]);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "usernmae");
    	curl_easy_setopt(curl, CURLOPT_PASSWORD, "password");

		switch (operation)
		{
			case 'g':
				setupGET(curl);
				break;
			case 'o':
				setupPOST(curl, input[1]);
				break;
			case 'p':
				setupPUT(curl, file_info, hd_src);;
				break;
			case 'd':
				setupDELETE(curl);
				break;
			case 'h':
				printHelp();
				break;
			case 'e':
			default:
				printf("inappropriate command!  Type \"./hw2 -h\" to see help info\n");
				return INIT_ERR;
		}

        res = curl_easy_perform(curl);
        long http_status = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status);
        printf("%s %ld\n", "\nReturn HTTP Status: ",http_status);
        if (res != CURLE_OK)
        {
        	UnixStandardStatus = REQ_ERR;
        }
        curl_easy_cleanup(curl);
    } else
    {
    	UnixStandardStatus = INIT_ERR;
    }

    fclose(hd_src);
    return UnixStandardStatus;
}