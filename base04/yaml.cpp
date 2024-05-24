#include <iostream>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include "common.cpp"

int main(int argc, char **argv)
{
    std::unordered_map<std::string, std::string> argsMap = praseArgs(argc, argv);
    std::string path = argsMap["f"];
    std::cout << "读取路径=" << path << std::endl;
    YAML::Node config = YAML::LoadFile(path);
    if (!config)
    {
        std::cout << "读取文件失败" << std::endl;
        exit(-1);
    }
    std::cout << "host=" << config["host"] << std::endl;
    std::cout << "port=" << config["port"] << std::endl;
    std::cout << "file.maxSize=" << config["file"]["maxSize"] << std::endl;
}