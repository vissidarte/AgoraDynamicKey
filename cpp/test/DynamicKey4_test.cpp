#include "../src/DynamicKey4.h"
#include <gtest/gtest.h>
#include <string>
#include <stdint.h>

class DynamicKey4_test : public testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}

public:
    void test_DynamicKey4();

    void test_RecordTicket();
};

void DynamicKey4_test::test_RecordTicket(){
    auto staticKey  = "970ca35de60c44645bbae8a215061b33";
    auto  signKey   = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "7d72365eb983485397e3e3f9d460bdda";
    auto  unixTs = 1446455472;
    auto  randomInt = 58964981;
    uint32_t uid=2882341273u;
    auto  expiredTs=1446455471;
    std::string result = agora::tools::DynamicKey4::generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
    EXPECT_EQ(result, "004e0c24ac56aae05229a6d9389860a1a0e25e56da8970ca35de60c44645bbae8a215061b3314464554720383bbf51446455471");
}

void DynamicKey4_test::test_DynamicKey4(){
    auto staticKey  = "970ca35de60c44645bbae8a215061b33";
    auto  signKey   = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "7d72365eb983485397e3e3f9d460bdda";
    auto  unixTs = 1446455472;
    auto  randomInt = 58964981;
    uint32_t uid=2882341273u;
    auto  expiredTs=1446455471;
    std::string result = agora::tools::DynamicKey4::generateMediaChannelKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
    EXPECT_EQ(result, "004d0ec5ee3179c964fe7c0485c045541de6bff332b970ca35de60c44645bbae8a215061b3314464554720383bbf51446455471");
}

TEST_F(DynamicKey4_test, test_RecordTicket)
{
    test_RecordTicket();
}
TEST_F(DynamicKey4_test, test_DynamicKey4)
{
    test_DynamicKey4();
}
