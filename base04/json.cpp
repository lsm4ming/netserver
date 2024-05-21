#include <iostream>
#include <jsoncons/json.hpp>

int main()
{
    using namespace jsoncons;

    // 解析JSON字符串
    std::string json_str = R"({"name": "lsm", "age": 30, "city": "长沙"})";
    json j = json::parse(json_str);


    // 从JSON对象中获取值
    std::string name = j["name"].as<std::string>();
    int age = j["age"].as<int>();
    std::string city = j["city"].as<std::string>();

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    // 生成JSON对象
    json j2;
    j2["name"] = "mdy";
    j2["age"] = 25;
    j2["city"] = "广州";

    // 将JSON对象转换为字符串
    std::cout << "JSON字符串: " << j2 << std::endl;
    return 0;
}