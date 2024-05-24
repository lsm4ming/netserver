#include <unordered_map>
#include <string>

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