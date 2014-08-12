#include <curl/curl.h>
#include <libetcdcpp.cpp>
#include <vector>

using namespace etcdcpp;

int main () {
  etcd_host h("localhost", 4001l);
  vector<etcd_host> host_list;
  host_list.push_back(h);
  etcd_session s(host_list);
  std::unique_ptr<Document> setA = s.set("/message/a", "bla", 60);
  s.set("/message/b", "dah");
  s.set("/message", "???");
  s.get("/message");
  s.get("/message/b");
  s.set("/c", "5" );
  std::unique_ptr<Document> result = s.get("/c");
  Value& v = (*result)["action"];
  cout << v.GetString();
  return 0;
}
