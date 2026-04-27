#include"json.hpp"
using json=nlohmann::json;
#include<vector>
#include<map>
#include<iostream>
#include<string>
using namespace std;


string fun(){
json js;
// 添加数组
js["id"] = {1,2,3,4,5};
// 添加key-value
js["name"] = "zhang san";
// 添加对象
js["msg"]["zhang san"] = "hello world";
js["msg"]["liu shuo"] = "hello china";
// 上面等同于下面这句一次性添加数组对象
js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
string sendbuf=js.dump();
return sendbuf;
}
string fun1(){
json js;
// 直接序列化一个vector容器
vector<int> vec;
vec.push_back(1);
vec.push_back(2);
vec.push_back(5);
js["list"] = vec;
// 直接序列化一个map容器
map<int, string> m;
m.insert({1, "黄山"});
m.insert({2, "华山"});
m.insert({3, "泰山"});
js["path"] = m;
string sendbuf=js.dump();
return sendbuf;
}

int main(){
string sendbuf=fun();
json jsbuf=json::parse(sendbuf);
string sendbuf1=fun1();
json jsbuf1=json::parse(sendbuf1);
// 直接取key-value
string name = jsbuf["name"];
cout << "name:" << name << endl;
// 直接反序列化vector容器
vector<int> v = jsbuf1["list"];
for(int val : v)
{
cout << val << " ";
}
cout << endl;
// 直接反序列化map容器
map<int, string> m2 = jsbuf1["path"];
for(auto p : m2)
{
cout << p.first << " " << p.second << endl;
}
cout << endl;
return 0;
}
