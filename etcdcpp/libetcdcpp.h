
#ifndef LIBETCDCPP_cxx_
#define LIBETCDCPP_cxx_

#include <vector>
#include <string.h>
#include "rapidjson/document.h"
#include <memory>

using namespace std;
using namespace rapidjson;

namespace etcdcpp {

/* curl uses a callback to read urls. It passes the result buffer reference as an argument */
int writer(char *data, size_t size, size_t nmemb, string *buffer);

class etcd_host {
  public:
    etcd_host(string host, short port): host(host), port(port) {};

    string get_host() { return host; }

    unsigned short get_port() { return port; }

  private: 
    string host;
    unsigned short port;
};

class etcd_session {
  public:
    etcd_session(vector<etcd_host> server_list);

    etcd_session(etcd_host host);

    /**
    * User must free returned Document *
    */
    std::unique_ptr<Document> get(string key);
    
    /**
    * User must free returned Document *
    */
    std::unique_ptr<Document> set(string key, string value, int ttl);

    /**
    * User must free returned Document *
    */
    std::unique_ptr<Document> set(string key, string value);

    private:
      vector<etcd_host> server_list;
};

} //end namespace

#endif
