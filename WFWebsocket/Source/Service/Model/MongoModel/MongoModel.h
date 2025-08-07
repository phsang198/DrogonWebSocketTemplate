#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/change_stream.hpp>
#include <mongocxx/options/change_stream.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/string/to_string.hpp>

using ChatCallback = std::function<void( const std::string& message)>;

class MongoModel {
public:
    MongoModel(const std::string& uri = "mongodb://10.225.0.240:27017");
private:
    mongocxx::database getDatabase(const std::string& dbName);
    mongocxx::collection getCollection(const std::string& dbName, const std::string& collName);
public:
    void watchChanges(const std::string& dbName, const std::string& collName);
    void setChatCallback(ChatCallback cb);

private:
    mongocxx::instance instance_;
    mongocxx::client client_;
    ChatCallback chatCallback_;
};