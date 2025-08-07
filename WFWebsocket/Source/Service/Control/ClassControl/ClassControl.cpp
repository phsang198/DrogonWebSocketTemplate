#include "pch.h"
#include "ClassControl.h"

using namespace workflow::v2;

ws::ws()
{
    auto wrap = [this]( const std::string& msg) {
        // parse msg lay instanceId
		json j = json::parse(msg);
        instanceTab_.publish(j["fullDocument"]["_id"]["$oid"], msg);
        };
    mmd.setChatCallback(wrap);
    
    watchThread_ = std::thread([this]() {
        mmd.watchChanges("wf_bpmn", "wf_instances");
        });
}
ws::~ws() {
    if (watchThread_.joinable())
        watchThread_.join();  // đảm bảo thread kết thúc an toàn
}
void ws::handleNewMessage(const WSC& wsConnPtr,std::string&& message,const WSMT& type)
{
    // write your application logic here
    LOG_DEBUG << "Client:" << message;
    if (type == WebSocketMessageType::Ping)
    {
        LOG_DEBUG << "recv a ping";
    }
    else if (type == WebSocketMessageType::Text)
    {
        auto& s = wsConnPtr->getContextRef<Subscriber>();
        instanceTab_.publish(s.instanceId, message);
    }
}

void ws::handleConnectionClosed(const WSC& conn)
{
    LOG_DEBUG << "websocket closed!";
    auto& s = conn->getContextRef<Subscriber>();
    instanceTab_.unsubscribe(s.instanceId, s.id_);
}

void ws::handleNewConnection(const REQ& req, const WSC& conn)
{
    std::string user = req->getParameter("user");
    auto log = "Add new user to room: " + user;
    LOG_DEBUG << log;

    conn->send("Hello " + user);

    Subscriber s;
    s.instanceId = req->getParameter("instanceId");
    s.id_ = instanceTab_.subscribe(s.instanceId,
        [conn](const std::string& topic,
            const std::string& message) {
                (void)topic;
                conn->send(message);
        });
    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}