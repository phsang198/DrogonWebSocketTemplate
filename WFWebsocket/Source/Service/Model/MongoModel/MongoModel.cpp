#include "pch.h"
#include "MongoModel.h"

MongoModel::MongoModel(const std::string& uri)
    : client_(mongocxx::uri{ uri }) {
}

mongocxx::database MongoModel::getDatabase(const std::string& dbName) {
    return client_[dbName];
}

mongocxx::collection MongoModel::getCollection(const std::string& dbName, const std::string& collName) {
    return client_[dbName][collName];
}

void MongoModel::watchChanges(const std::string& dbName, const std::string& collName) {
    auto collection = getCollection(dbName, collName);

    //std::cout << "🔍 Watching changes on " << dbName << "." << collName << "...\n";
    LOG_INFO << "🔍 Watching changes on " << dbName << "." << collName << "...\n";

    while (true) // Loop forever
    {
        try
        {
            mongocxx::options::change_stream options;
            options.full_document("updateLookup");
            const std::chrono::milliseconds await_time{ 1000 };
            options.max_await_time(await_time);
            mongocxx::change_stream streamChanges = collection.watch(options);
            for (const bsoncxx::document::view& event : streamChanges)
            {
                LOG_INFO << "Change event received: " << bsoncxx::to_json(event); 

                if (chatCallback_) {
                    chatCallback_( bsoncxx::to_json(event));  // chủ động gọi publish ra WebSocket
                }

            }
        }
        catch (const std::exception& e)
        {
            LOG_INFO << "MongoDB watcher caught exception: " << e.what();
        }
    }
}

void MongoModel::setChatCallback(ChatCallback cb) {
    chatCallback_ = std::move(cb);
}
