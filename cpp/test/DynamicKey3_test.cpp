#include "../src/DynamicKey3.h"
#include <gtest/gtest.h>
#include <string>

class DynamicKey3_test : public testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}

public:
    void test_DynamicKey3();
};

void DynamicKey3_test::test_DynamicKey3(){
    auto staticKey = "970ca35de60c44645bbae84215061b33b";
    auto  signKey = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "7d72365eb983485397e3e3f9d460bdda";
    auto  unixTs = 1446455472;
    auto  randomInt = 58964981;
    uint32_t uid=2882341273u;
    auto  expiredTs=1446455471;
    std::string result = agora::tools::DynamicKey3::generate(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
    EXPECT_EQ(result, "00381e1588b09e6e76a0d2e8fd9e5257dbddd9c60a0970ca35de60c44645bbae84215061b33b14464554720383bbf528823412731446455471");
}

TEST_F(DynamicKey3_test, test_DynamicKey3)
{
    test_DynamicKey3();
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
