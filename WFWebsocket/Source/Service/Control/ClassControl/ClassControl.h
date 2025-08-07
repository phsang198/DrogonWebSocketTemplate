#pragma once
#include "pch.h"
#include "ObjectManager/ObjectManager.h"
#include "Object/Object.h"
#include "Model/MongoModel/MongoModel.h"

namespace workflow::v2 //  /root/v1/resource
{
    class ws : public WSCONTROLLER<ws>
    {
    public:
        ws();
        ~ws() override;
    public:
        void handleNewMessage(const WSC&,std::string&&,const WSMT&) override;
        void handleConnectionClosed(const WSC&) override;
        void handleNewConnection(const REQ&,const WSC&) override;
        WS_PATH_LIST_BEGIN
            WS_PATH_ADD("/chat", Get);
        WS_ADD_PATH_VIA_REGEX("/[^/]*", Get);
        WS_PATH_LIST_END
    private:
        PubSubService<std::string> instanceTab_;
        MongoModel mmd;
        std::thread watchThread_;
    };

    struct Subscriber
    {
        std::string instanceId;
        SubscriberID id_;
    };
    
}
