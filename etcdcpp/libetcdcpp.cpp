
#include <curl/curl.h>
#include <iostream>
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

/* curl uses a callback to read urls. It passes the result buffer reference as an argument */
int etcdcpp::writer(char *data, size_t size, size_t nmemb, string *buffer){
  int result = 0;
  if(buffer != NULL) {
    buffer -> append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
} 

etcd_session::etcd_session(vector<etcd_host> server_list) {
  if (server_list.size() == 0) {
    throw "startup failed";
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
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  string buffer;	
  if (curl) {
    for (int i=0 ; i< server_list.size() ; i++){
      curl_easy_setopt(curl, CURLOPT_URL, ("http://" + server_list[0].get_host() + ":" + "4001" + "/v2/keys" + key + "?sorted=true&recursive=true").c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
      res = curl_easy_perform(curl);
      if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw res;
      }
      curl_easy_cleanup(curl);

      Document *d = new Document;
      d->Parse(buffer.c_str());
      return std::unique_ptr<Document>(std::move(d));
    }
  }
  throw "curl issue";
}
  
std::unique_ptr<Document> etcd_session::set(string key, string value, int ttl) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  string buffer = "value=" + value + "&ttl=" + "60";
  string result;
  if(curl) {
    for (int i=0 ; i< server_list.size() ; i++){
      curl_easy_setopt(curl, CURLOPT_URL, ("http://" + server_list[0].get_host() + ":" + "4001" + "/v2/keys" + key).c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
      res = curl_easy_perform(curl);
      if(res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw res;
      }
      curl_easy_cleanup(curl);
      
      Document *d = new Document;
      d->Parse(result.c_str());
      return std::unique_ptr<Document>(std::move(d));
    }
  } 
  throw "curl issue";
}

std::unique_ptr<Document> etcd_session::set(string key, string value) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  string buffer = "value=" + value;
  string result;
  if(curl) {
    for (int i=0 ; i< server_list.size() ; i++){
      curl_easy_setopt(curl, CURLOPT_URL, ("http://" + server_list[0].get_host() + ":" + "4001" + "/v2/keys" + key).c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
      res = curl_easy_perform(curl);
      if(res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw res;
      }
      curl_easy_cleanup(curl);

      Document *d = new Document;
      d->Parse(result.c_str());
      return std::unique_ptr<Document>(std::move(d));
    }
  }
  throw "curl issue";
}

