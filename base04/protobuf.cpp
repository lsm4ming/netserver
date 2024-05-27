#include <iostream>
#include "demo.pb.h"

using String = std::string;

int main()
{
    // 创建一个 Person 对象并设置值
    Person person;
    person.set_name("lsm1998");
    person.set_id(1);
    person.set_email("lsm1998@qq.com");

    // 将对象序列化到字符串中
    String serialized_data;
    person.SerializeToString(&serialized_data);
    std::cout << "序列化字符串: " << serialized_data << std::endl;

    // 从字符串中解析出对象
    Person parsed_person;
    parsed_person.ParseFromString(serialized_data);

    // 打印解析后的对象
    std::cout << "Name: " << parsed_person.name() << std::endl;
    std::cout << "ID: " << parsed_person.id() << std::endl;
    std::cout << "Email: " << parsed_person.email() << std::endl;
    return 0;
}