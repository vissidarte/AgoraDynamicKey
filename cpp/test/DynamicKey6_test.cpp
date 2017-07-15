#include "../src/DynamicKey6.h"
#include <gtest/gtest.h>
#include <string>
#include <stdint.h>
using namespace agora::tools;

class DynamicKey6_test : public testing::Test
{
protected:
    std::string appID;
    std::string appCertificate;
    std::string channelName;
    uint32_t unixTs;
    uint32_t salt;
    uint32_t uid;
    uint32_t expiredTs;

    virtual void SetUp()
    {
        appID  = "970ca35de60c44645bbae8a215061b33";
        appCertificate   = "5cfd2fd1755d40ecb72977518be15d3b";
        channelName= "7d72365eb983485397e3e3f9d460bdda";
        unixTs = 1446455472;
        salt = 58964981;
        uid=2882341273u;
        expiredTs=1446455471;
    }
    virtual void TearDown(){}

public:
    void test_MediaChannelKey();
    void test_MediaChannelKeyUid();
    void test_MediaChannelKeyUidKick();
    void test_MediaChannelKeyFull();

    void test_RecordingKey();
    void test_RecordingKeyUid();
    void test_RecordingKeyFull();

    void test_PublicSharingKey();
    void test_PublicSharingKeyFull();

    void test_InChannelPermission();
    void test_InChannelPermissionFull();

    void test_rawAppId();

private:
    void test_InChannelPermission(const std::string& x, const std::string& expected);

    template<typename Generate, typename Verify>
    void testDynamicKey(DynamicKey6::ServiceType service, Generate g, Verify v)
    {
        std::string result = g();

        DynamicKey6 k6;
        bool parsed = k6.fromString(result);
        ASSERT_TRUE(parsed);

        EXPECT_EQ(k6.appID, appID);

        std::string signature = DynamicKey6::generateSignature(
                                        appCertificate
                                        , service
                                        , k6.appID
                                        , k6.content.unixTs
                                        , k6.content.salt
                                        , channelName
                                        , k6.content.uid
                                        , k6.content.expiredTs
                                        , k6.content.extra);
        EXPECT_EQ(k6.content.signature, signature);
        v(k6, result);
    }

};

void DynamicKey6_test::test_PublicSharingKey() {
    testDynamicKey(DynamicKey6::PUBLIC_SHARING_SERVICE, [this]() {
        return DynamicKey6::generatePublicSharingKey(
                    appID
                    , appCertificate
                    , channelName);
    }, [this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, 0);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
    });
}

void DynamicKey6_test::test_PublicSharingKeyFull() {
    auto expected = "006970ca35de60c44645bbae8a215061b33AwAoADc0QTk5RTVEQjI4MDk0NUI0NzUwNTk0MUFDMjM4MDU2NzIwREY3QjCZCc2rsCg3VvW7gwOvKDdWAAA=";

    testDynamicKey(DynamicKey6::PUBLIC_SHARING_SERVICE, [this]() {
        return DynamicKey6::generatePublicSharingKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs);
    }, [expected, this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(expected, result);
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_EQ(k6.content.unixTs, unixTs);
        EXPECT_EQ(k6.content.salt, salt);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
    });
}

void DynamicKey6_test::test_RecordingKey() {
    testDynamicKey(DynamicKey6::RECORDING_SERVICE, [this]() {
        return DynamicKey6::generateRecordingKey(
                    appID
                    , appCertificate
                    , channelName);
    }, [](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, 0);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
    });
}

void DynamicKey6_test::test_RecordingKeyUid() {
    testDynamicKey(DynamicKey6::RECORDING_SERVICE, [this]() {
        return DynamicKey6::generateRecordingKey(
                    appID
                    , appCertificate
                    , channelName
                    , uid);
    }, [this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
    });
}

void DynamicKey6_test::test_RecordingKeyFull() {
    auto expected = "006970ca35de60c44645bbae8a215061b33AgAoADkyOUM5RTQ2MTg3QTAyMkJBQUIyNkI3QkYwMTg0MzhDNjc1Q0ZFMUGZCc2rsCg3VvW7gwOvKDdWAAA=";

    testDynamicKey(DynamicKey6::RECORDING_SERVICE, [this]() {
        return DynamicKey6::generateRecordingKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs);
    }, [expected, this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(expected, result);
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_EQ(k6.content.unixTs, unixTs);
        EXPECT_EQ(k6.content.salt, salt);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
    });
}

void DynamicKey6_test::test_MediaChannelKey() {
    testDynamicKey(DynamicKey6::MEDIA_CHANNEL_SERVICE, [this]() {
        return DynamicKey6::generateMediaChannelKey(
                    appID
                    , appCertificate
                    , channelName);
    }, [](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, 0);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
    });
}

void DynamicKey6_test::test_MediaChannelKeyUid() {
    testDynamicKey(DynamicKey6::MEDIA_CHANNEL_SERVICE, [this]() {
        return DynamicKey6::generateMediaChannelKey(
                    appID
                    , appCertificate
                    , channelName
                    , uid);
    }, [this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
    });
}

void DynamicKey6_test::test_MediaChannelKeyUidKick(){
    testDynamicKey(DynamicKey6::MEDIA_CHANNEL_SERVICE, [this]() {
        return DynamicKey6::generateMediaChannelKey(
                    appID
                    , appCertificate
                    , channelName
                    , uid
                    , expiredTs);
    }, [this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
    });
}

void DynamicKey6_test::test_MediaChannelKeyFull(){
    auto expected = "006970ca35de60c44645bbae8a215061b33AQAoAEJERTJDRDdFNkZDNkU0ODYxNkYxQTYwOUVFNTM1M0U5ODNCQjFDNDSZCc2rsCg3VvW7gwOvKDdWAAA=";

    testDynamicKey(DynamicKey6::MEDIA_CHANNEL_SERVICE, [this]() {
        return DynamicKey6::generateMediaChannelKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs);
    }, [expected, this](const DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(expected, result);
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_EQ(k6.content.unixTs, unixTs);
        EXPECT_EQ(k6.content.salt, salt);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
    });
}

void DynamicKey6_test::test_InChannelPermission()
{
    testDynamicKey(DynamicKey6::IN_CHANNEL_PERMISSION, [this]() {
        return DynamicKey6::generateInChannelPermissionKey(
                    appID
                    , appCertificate
                    , channelName
                    , uid
                    , DynamicKey6::noUpload());
    }, [this](DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
        ASSERT_FALSE(k6.content.extra.empty());
        EXPECT_TRUE(k6.content.extra.find(DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL) != k6.content.extra.end());
        EXPECT_EQ(k6.content.extra[DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL], DynamicKey6::noUpload());
    });

    testDynamicKey(DynamicKey6::IN_CHANNEL_PERMISSION, [this]() {
        return DynamicKey6::generateInChannelPermissionKey(
                    appID
                    , appCertificate
                    , channelName
                    , uid
                    , DynamicKey6::audioVideoUpload());
    }, [this](DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_LE(k6.content.unixTs, ::time(nullptr));
        EXPECT_NE(k6.content.salt, 0);
        EXPECT_EQ(k6.content.expiredTs, 0);
        ASSERT_FALSE(k6.content.extra.empty());
        EXPECT_TRUE(k6.content.extra.find(DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL) != k6.content.extra.end());
        EXPECT_EQ(k6.content.extra[DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL], DynamicKey6::audioVideoUpload());
    });
}

void DynamicKey6_test::test_InChannelPermissionFull()
{
    auto noUpload = "006970ca35de60c44645bbae8a215061b33BAAoADgyNEQxNDE4M0FGRDkyOEQ4REFFMUU1OTg5NTg2MzA3MTEyNjRGNzSZCc2rsCg3VvW7gwOvKDdWAQABAAEAMA==";
    testDynamicKey(DynamicKey6::IN_CHANNEL_PERMISSION, [this]() {
        return DynamicKey6::generateInChannelPermissionKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs
                    , DynamicKey6::noUpload());
    }, [noUpload, this](DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(noUpload, result);
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_EQ(k6.content.unixTs, unixTs);
        EXPECT_EQ(k6.content.salt, salt);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
        ASSERT_FALSE(k6.content.extra.empty());
        EXPECT_TRUE(k6.content.extra.find(DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL) != k6.content.extra.end());
        EXPECT_EQ(k6.content.extra[DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL], DynamicKey6::noUpload());
    });

    auto audioVideoUpload = "006970ca35de60c44645bbae8a215061b33BAAoADJERDA3QThENTE2NzJGNjQwMzY5NTFBNzE0QkI5NTc0N0Q1QjZGQjOZCc2rsCg3VvW7gwOvKDdWAQABAAEAMw==";
    testDynamicKey(DynamicKey6::IN_CHANNEL_PERMISSION, [this]() {
        return DynamicKey6::generateInChannelPermissionKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs
                    , DynamicKey6::audioVideoUpload());
    }, [audioVideoUpload, this](DynamicKey6& k6, const std::string& result) {
        EXPECT_EQ(audioVideoUpload, result);
        EXPECT_EQ(k6.content.uid, uid);
        EXPECT_EQ(k6.content.unixTs, unixTs);
        EXPECT_EQ(k6.content.salt, salt);
        EXPECT_EQ(k6.content.expiredTs, expiredTs);
        ASSERT_FALSE(k6.content.extra.empty());
        EXPECT_TRUE(k6.content.extra.find(DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL) != k6.content.extra.end());
        EXPECT_EQ(k6.content.extra[DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL], DynamicKey6::audioVideoUpload());
    });
}

void DynamicKey6_test::test_rawAppId()
{
    std::string noUpload = "006970ca35de60c44645bbae8a215061b33BAAoADgyNEQxNDE4M0FGRDkyOEQ4REFFMUU1OTg5NTg2MzA3MTEyNjRGNzSZCc2rsCg3VvW7gwOvKDdWAQABAAEAMA==";
    testDynamicKey(DynamicKey6::IN_CHANNEL_PERMISSION, [this]() {
        return DynamicKey6::generateInChannelPermissionKey(
                    appID
                    , appCertificate
                    , channelName
                    , unixTs
                    , salt
                    , uid
                    , expiredTs
                    , DynamicKey6::noUpload());
    }, [noUpload, this](DynamicKey6& k6, const std::string& result) {
        ASSERT_FALSE(k6.content.extra.empty());
        EXPECT_TRUE(k6.content.extra.find(DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL) != k6.content.extra.end());
        EXPECT_EQ(k6.content.extra[DynamicKey6::ALLOW_UPLOAD_IN_CHANNEL], DynamicKey6::noUpload());
        EXPECT_NE(noUpload.find(appID), std::string::npos);
    });
}

TEST_F(DynamicKey6_test, test_MediaChannelKey)
{
    test_MediaChannelKey();
}

TEST_F(DynamicKey6_test, test_MediaChannelKeyUid)
{
    test_MediaChannelKeyUid();
}

TEST_F(DynamicKey6_test, test_MediaChannelKeyUidKick)
{
    test_MediaChannelKeyUidKick();
}

TEST_F(DynamicKey6_test, test_RecordingKey)
{
    test_RecordingKey();
}

TEST_F(DynamicKey6_test, test_RecordingKeyUid)
{
    test_RecordingKeyUid();
}

TEST_F(DynamicKey6_test, test_PublicSharingKey)
{
    test_PublicSharingKey();
}

TEST_F(DynamicKey6_test, test_InChannelPermission)
{
    test_InChannelPermission();
}

TEST_F(DynamicKey6_test, test_PublicSharingKeyFull)
{
    test_PublicSharingKeyFull();
}

TEST_F(DynamicKey6_test, test_RecordingKeyFull)
{
    test_RecordingKeyFull();
}

TEST_F(DynamicKey6_test, test_MediaChannelKeyFull)
{
    test_MediaChannelKeyFull();
}

TEST_F(DynamicKey6_test, test_InChannelPermissionFull)
{
    test_InChannelPermissionFull();
}

TEST_F(DynamicKey6_test, test_rawAppId)
{
    test_rawAppId();
}
