#include "pch.h"
#include "WFWebsocket.h"
#include <Class/Tools/Tools.h>
#include <fmt/base.h>
#include <fmt/format.h>

WFWebsocket::~WFWebsocket()
{
}

void WFWebsocket::RunDrogon()
{
    
    auto conf = getConfig();

    if (1) // test with all configs before running
    {
        RootService::RunDrogon();
    }
}
