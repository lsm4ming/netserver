#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/database.hpp>

constexpr const char *MONGO_URI = "mongodb://192.168.239.50:27017";
constexpr const char *MONGO_DB = "testdb";
constexpr const char *MONGO_COLLECTION = "user";

int main()
{
    // docker run -d --name mongo -p 27017:27017 mongo

    // 初始化 MongoDB 驱动程序实例
    mongocxx::instance instance{};

    // 连接到 MongoDB 服务器
    mongocxx::client client{mongocxx::uri{MONGO_URI}};

    // 获取数据库和集合
    mongocxx::database db = client[MONGO_DB];
    mongocxx::collection coll = db[MONGO_COLLECTION];

    // 构建一个文档并插入到集合中
    bsoncxx::builder::stream::document document{};
    document << "name" << "MongoDB" << "type" << "database" << "count" << 1 << "versions"
             << bsoncxx::builder::stream::open_array << "v3.6" << "v4.0" << "v4.2"
             << bsoncxx::builder::stream::close_array << "info"
             << bsoncxx::builder::stream::open_document << "x" << 203 << "y" << 102
             << bsoncxx::builder::stream::close_document;

    coll.insert_one(document.view());

    // 查询并输出插入的文档
    auto cursor = coll.find({});
    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

    std::cout << "hello" << std::endl;
}