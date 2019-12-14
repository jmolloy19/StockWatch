#pragma once
#include <iostream>
#include <string>
#include <curl/curl.h>

static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp);
void MakeHttpRequest(const std::string& url, std::string* buffer);