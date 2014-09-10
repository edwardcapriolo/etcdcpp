
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <memory>
#include "libetcdcpp.h"

using namespace std;
using namespace rapidjson;
using namespace etcdcpp;

template<typename T>
string append(string s, T t) {
  ostringstream sstream;
  sstream << s << t;
  return sstream.str();
}

/* curl uses a callback to read urls. It passes the result buffer reference as an argument */
int etcdcpp::writer(char *data, size_t size, size_t nmemb, string *buffer){
  int result = 0;
  if(buffer != NULL) {
    buffer -> append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
} 

template <typename fun>
std::unique_ptr<Document> with_curl(vector<etcd_host> server_list, fun process) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  string result;
  if (curl) {
    for (auto server : server_list) {
      process(server, curl);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK){
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        throw res;
      }
      curl_easy_cleanup(curl);

      Document *d = new Document;
      d->Parse(result.c_str());
      return std::unique_ptr<Document>(std::move(d));
    }
  } 
  throw "Curl failed to initialize";
}


string build_url(etcd_host &host, string key) {
  ostringstream url;
  url << "http://" << host.get_host() << ":" <<  host.get_port() << "/v2/keys" << key;
  return url.str();
}

etcd_session::etcd_session(vector<etcd_host> server_list) {
  if (server_list.size() == 0) {
    throw "Startup failed: At least one host is required";
  }
  this->server_list = server_list;
  curl_global_init(CURL_GLOBAL_ALL);
}

etcd_session::etcd_session(etcd_host host) {
  vector<etcd_host> hosts;
  hosts.push_back(host);
  this->server_list = hosts;
  curl_global_init(CURL_GLOBAL_ALL);
}

std::unique_ptr<Document> etcd_session::get(string key) {
  return with_curl(server_list, [=](etcd_host server, CURL *curl) {
    string url = build_url(server, key) + "?sorted=true&recursive=true";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  });
}

 
std::unique_ptr<Document> etcd_session::set(string key, string value, int ttl) {
  return this->set(key, value + append("&ttl=", ttl));
}

std::unique_ptr<Document> etcd_session::set(string key, string value) {
  return with_curl(server_list, [=](etcd_host server, CURL *curl) {
    string url = build_url(server, key);
    string buffer = "value=" + value;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  });
}

