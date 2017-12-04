#include "../src/DynamicKey6.h"
#include <gtest/gtest.h>
#include <string>
#include <stdint.h>
using namespace agora::tools;

std::string appID;
std::string appCertificate;
std::string channelName;
uint32_t unixTs;
uint32_t randomInt;
uint32_t uid;
uint32_t expiredTs;

class DynamicKey6_test : public testing::Test
{
protected:
    virtual void SetUp()
    {
        appID  = "970CA35de60c44645bbae8a215061b33";
        appCertificate   = "5CFd2fd1755d40ecb72977518be15d3b";
        channelName= "7d72365eb983485397e3e3f9d460bdda";
        unixTs = 1446455472;
        randomInt = 58964981;
        uid=2882341273u;
        expiredTs=1446455471;
    }
    virtual void TearDown(){}

public:

private:
};

