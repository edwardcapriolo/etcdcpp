#include <iostream>
#include <curl/curl.h>
#include <etcdcpp.h>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace etcdcpp;

int main () {
  try {
    vector<etcd_host> host_list { etcd_host("localhost", 4001l) };
    etcd_session s(host_list);

    std::unique_ptr<Document> setA = s.set("/message/a", "bla", 60);
    s.set("/message/b", "dah");
    s.set("/message", "???");
    s.get("/message");
    s.get("/message/b");
    s.set("/c", "5" );

    std::unique_ptr<Document> result = s.get("/c");

    // write result to stdout
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    result->Accept(writer);
    std::cout << buffer.GetString() << std::endl;
  } catch (exception& e) {
    cerr << e.what() << endl;
  } catch (const char* message) {
    cerr << message << endl;
  }

  return 0;
}

