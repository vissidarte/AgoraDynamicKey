#include "../src/DynamicKey6.h"
#include <gtest/gtest.h>
#include <string>
#include <stdint.h>
using namespace agora::tools;

class DynamicKey6_test : public testing::Test
{
protected:
    virtual void SetUp()
    {
        appID  = "970CA35de60c44645bbae8a215061b33";
        appCertificate   = "5CFd2fd1755d40ecb72977518be15d3b";
        channelName= "7d72365eb983485397e3e3f9d460bdda";
        uid=2882341273u;
        uidStr="2882341273";
        expiredTs=1446455471;
    }

    virtual void TearDown(){}

    void testDynamicKey(std::string expected, DynamicKey6 key){
      std::string result = key.Build();
      EXPECT_EQ(expected, result);

      if (expected == "") {
        return;
      }
      DynamicKey6 k6;
      bool parsed = k6.FromString(result);
      ASSERT_TRUE(parsed);
      EXPECT_EQ(k6.app_id_, key.app_id_);
      EXPECT_EQ(k6.crc_channel_name_, key.crc_channel_name_);
      EXPECT_EQ(k6.crc_uid_, key.crc_uid_);
      uint32_t crc_channel_name = crc32(
        0, reinterpret_cast<Bytef*>(const_cast<char*>(key.channel_name_.c_str())),
        key.channel_name_.length());
      uint32_t crc_uid =
        crc32(0, reinterpret_cast<Bytef*>(const_cast<char*>(key.uid_.c_str())),
              key.uid_.length());

      EXPECT_EQ(k6.crc_channel_name_, crc_channel_name);
      EXPECT_EQ(k6.crc_uid_, crc_uid);
      EXPECT_EQ(k6.message_.ts, key.message_.ts);
      EXPECT_EQ(k6.message_.salt, key.message_.salt);
      EXPECT_EQ(k6.message_.messages[DynamicKey6::Privileges::kJoinChannel], key.message_.messages[DynamicKey6::Privileges::kJoinChannel]);

      std::string signature = DynamicKey6::GenerateSignature(
          appCertificate
          , k6.app_id_
          , key.channel_name_
          , key.uid_ 
          , k6.message_raw_content_
          );
      EXPECT_EQ(k6.signature_, signature);
    }
    void testDynamicKeyWithIntUid()
    {
      std::string expected = "006F897E2BFEA30A733B9B4AA05AD6375B456AF483A970CA35de60c44645bbae8a215061b3302766460713847331927AQAAAEf0EAABAAEAryg3Vg==";
      DynamicKey6 key(appID, appCertificate, channelName, uid);
      key.message_.salt = 1;
      key.message_.ts = 1111111;  
      key.AddPrivilege(DynamicKey6::Privileges::kJoinChannel, expiredTs);
      testDynamicKey(expected, key);
    }

    void testDynamicKeyWithStringUid()
    {
      std::string expected = "006F897E2BFEA30A733B9B4AA05AD6375B456AF483A970CA35de60c44645bbae8a215061b3302766460713847331927AQAAAEf0EAABAAEAryg3Vg==";
      DynamicKey6 key(appID, appCertificate, channelName, uidStr);
      key.AddPrivilege(DynamicKey6::Privileges::kJoinChannel, expiredTs);
      key.message_.salt = 1;
      key.message_.ts = 1111111;  
      testDynamicKey(expected, key);
    }

    void testDynamicKeyWithErrorUid()
    {
      std::string expected = "";
      DynamicKey6 key(appID, appCertificate, channelName, "error");
      key.message_.salt = 1;
      key.message_.ts = 1111111;  
      key.AddPrivilege(DynamicKey6::Privileges::kJoinChannel, expiredTs);
      testDynamicKey(expected, key);
    }
public:

private:
    std::string appID;
    std::string appCertificate;
    std::string channelName;
    uint32_t uid;
    std::string uidStr;
    uint32_t expiredTs;
};

TEST_F(DynamicKey6_test, testDynamicKeyWithErrorUid)
{
    testDynamicKeyWithErrorUid();
}
TEST_F(DynamicKey6_test, testDynamicKeyWithIntUid)
{
    testDynamicKeyWithIntUid();
}
TEST_F(DynamicKey6_test, testDynamicKeyWithStringUid)
{
    testDynamicKeyWithStringUid();
}

