#include <stdio.h>
#include <stdlib.h>
#include "curl.h"

#define GMAIL_URL "https://mail.google.com/mail/feed/atom/"
#define PROXY "proxy.school.com:8080"

// url encoding: curl_easy_escape

char *g_url;
char g_auth[512];
char *g_proxy;

size_t write_data( void *ptr, size_t size, size_t nmeb, void *stream)
{
	return fwrite(ptr,size,nmeb,(FILE*)stream);
}

void parse_arguments( int argc, char** argv)
{
	char *pszUrl=0;
	char *pszLabel=0;
	char *pszUser=0;
	char *pszPw=0;
	int i;

	for(i=1; i< argc; i++) {
		// user
		if(argv[i][1] == 'u')
		{
			pszUser = &argv[i][2];
			continue;
		}
		// password
		if(argv[i][1] == 'p')
		{
			pszUser = &argv[i][2];
			continue;
		}
		// label
		if(argv[i][1] == 'l')
		{
			pszLabel = &argv[i][2];
			continue;
		}
		// url
		if(argv[i][1] == 'u' && argv[i][2] == 'r' && argv[i][3] == 'l')
			g_url = &argv[i][4];
		else
			pszUrl = GMAIL_URL;
		// proxy
		if(argv[i][1] == 'p' && argv[i][2] == 'r' && argv[i][3] == 'o' && argv[i][4] == 'p' && argv[i][5] == 'x' && argv[i][6] == 'y')
			g_proxy = &argv[i][7];
		else
			g_proxy = PROXY;
	}

	if (pszLabel)
	{
		g_url = (char*)malloc( strlen(pszUrl)+strlen(pszLabel)+1 );
		sprintf( g_url, "%s/%s", pszUrl, pszLabel );
	}
	sprintf( g_auth, "%s:%s", pszUser, pszPw );	
}

int main(int argc, char **argv)
{
	CURLcode res;
	CURL *hCurl;
	
	parse_arguments( argc, argv );

	hCurl = curl_easy_init();
	curl_easy_setopt( hCurl, CURLOPT_URL, g_url );
	curl_easy_setopt( hCurl, CURLOPT_PROXY, g_proxy );
	curl_easy_setopt( hCurl, CURLOPT_SSL_VERIFYPEER, 0L );
	curl_easy_setopt( hCurl, CURLOPT_SSL_VERIFYHOST, 2 );

	curl_easy_setopt( hCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
	curl_easy_setopt( hCurl, CURLOPT_USERPWD, g_auth );

	curl_easy_setopt( hCurl, CURLOPT_WRITEFUNCTION, write_data );
	curl_easy_setopt( hCurl, CURLOPT_WRITEDATA, stdout );
	res = curl_easy_perform( hCurl );

	curl_easy_cleanup(hCurl);
	return 0;
}