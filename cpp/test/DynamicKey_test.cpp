#include "../src/DynamicKey.h"
#include <gtest/gtest.h>
#include <string>

class DynamicKey_test : public testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}

public:
    void test_DynamicKey();
};

void DynamicKey_test::test_DynamicKey(){
    auto staticKey = "970ca35de60c44645bbae84215061b33b";
    auto  signKey = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "7d72365eb983485397e3e3f9d460bdda";
    auto  unixTs = 1446455472;
    auto  randomInt = 58964981;
    std::string result = agora::tools::DynamicKey::generate(staticKey, signKey, channelName, unixTs, randomInt);
    EXPECT_EQ(result, "70e0e5b0a137630190bfa475506de9fb56012576970ca35de60c44645bbae84215061b33b14464554720383bbf5");
}

TEST_F(DynamicKey_test, test_DynamicKey)
{
    test_DynamicKey();
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
