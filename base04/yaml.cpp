#include <iostream>
#include <yaml-cpp/yaml.h>
#include <unordered_map>

std::unordered_map<std::string, std::string> praseArgs(int argc, char **argv)
{
    std::unordered_map<std::string, std::string> result;
    for (size_t i = 1; i < argc; i += 2)
    {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        if (key.starts_with('-'))
        {
            key = key.substr(1, key.length());
        }
        if (key.ends_with('='))
        {
            key = key.substr(0, key.length() - 1);
        }
        result[key] = value;
    }
    return result;
}

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