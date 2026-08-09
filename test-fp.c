/*
 * test-fp.c — HTTP/3 fingerprint test client.
 *
 * Usage: test-fp <url> [options...]
 *
 * Options: curves=<list> perm=<perm> sigalgs=<list> fp-quic=<str>
 *          fp-settings=<str> fp-tp=<str> fp-tls=<str>
 *
 * Writes the response body (the fingerprint JSON from
 * https://fp.impersonate.pro/api/http3) to stdout.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  FILE *f = (FILE *)userdata;
  return fwrite(ptr, size, nmemb, f);
}

int main(int argc, char **argv)
{
  CURL *e;
  CURLcode rc;
  int i;
  char errbuf[CURL_ERROR_SIZE];

  if(argc < 2) {
    fprintf(stderr, "usage: test-fp <url> [options...]\n");
    return 2;
  }

  errbuf[0] = '\0';
  e = curl_easy_init();
  if(!e)
    return 1;

  curl_easy_setopt(e, CURLOPT_URL, argv[1]);
  curl_easy_setopt(e, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_3ONLY);
  curl_easy_setopt(e, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(e, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(e, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(e, CURLOPT_WRITEDATA, stdout);
  curl_easy_setopt(e, CURLOPT_TIMEOUT, 30L);
  curl_easy_setopt(e, CURLOPT_ERRORBUFFER, errbuf);
  curl_easy_setopt(e, CURLOPT_VERBOSE, 1L);

  for(i = 2; i < argc; i++) {
    if(!strncmp(argv[i], "curves=", 7))
      curl_easy_setopt(e, CURLOPT_SSL_EC_CURVES, argv[i] + 7);
    else if(!strncmp(argv[i], "perm=", 5))
      curl_easy_setopt(e, CURLOPT_TLS_EXTENSION_PERMUTATION, argv[i] + 5);
    else if(!strncmp(argv[i], "sigalgs=", 8))
      curl_easy_setopt(e, CURLOPT_TLS_VERIFY_SIGALGS, argv[i] + 8);
    else if(!strncmp(argv[i], "fp-quic=", 8))
      curl_easy_setopt(e, CURLOPT_HTTP3_FP_QUIC, argv[i] + 8);
    else if(!strncmp(argv[i], "fp-settings=", 12))
      curl_easy_setopt(e, CURLOPT_HTTP3_FP_SETTINGS, argv[i] + 12);
    else if(!strncmp(argv[i], "fp-tp=", 6))
      curl_easy_setopt(e, CURLOPT_HTTP3_FP_TRANSPORT_PARAMS, argv[i] + 6);
    else if(!strncmp(argv[i], "fp-tls=", 7))
      curl_easy_setopt(e, CURLOPT_HTTP3_FP_TLS, argv[i] + 7);
    else {
      fprintf(stderr, "test-fp: unknown option '%s'\n", argv[i]);
      curl_easy_cleanup(e);
      return 2;
    }
  }

  rc = curl_easy_perform(e);
  if(rc != CURLE_OK)
    fprintf(stderr, "test-fp: %s\n  %s\n",
            curl_easy_strerror(rc), errbuf);

  curl_easy_cleanup(e);
  return rc ? 1 : 0;
}
