/**
 * build with command:
 * g++ -o ChannelKeyVerifier6 ChannelKeyVerifier6.cpp  -lcrypto -std=c++0x
 */
#include "../src/DynamicKey6.h"
#include <iostream>
#include <cstdint>
using namespace agora::tools;

template<typename T>
inline std::string map_to_string(const T & c, const std::string& separator)
{
    std::ostringstream oss;
    std::string s = "";
    for (const auto & v: c) {
        oss << s << v.first << ":" << v.second;
        s = separator;
    }

    return oss.str();
}

bool isTimeout(uint32_t unixTs, uint32_t timeoutSecond, uint32_t nowSecond) {
  uint32_t minAllowedTs = nowSecond - 300 - timeoutSecond;
  uint32_t maxAllowedTs = nowSecond + 300;
  return (unixTs < minAllowedTs) || (unixTs > maxAllowedTs);
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        std::cout << argv[0] << "channelName uid key" << std::endl;
        return 0;
    }

    std::string channelName = argv[1];
    uint32_t uid = strtoul(argv[2], NULL, 10);
    std::string keyString = argv[3];

    DynamicKey6 key;
    if (! key.fromString(keyString)) {
        std::cout << "The key is not a valid DynamicKey6" << std::endl;
        return 0;
    }

    std::cout << std::endl;
    std::cout << "App ID:     " << key.appID << std::endl;
    std::cout << "UID:        " << key.content.uid << std::endl;
    std::cout << "Timestamp:  " << key.content.unixTs << std::endl;
    std::cout << "Expired:    " << key.content.expiredTs << std::endl;
    std::cout << "Extra:      [" << map_to_string(key.content.extra, ", ") << "]" << std::endl;
    std::cout << "Salt:       " << key.content.salt << std::endl;
    std::cout << "Signature:  " << key.content.signature << std::endl;
    std::cout << std::endl;

    std::string appID = "970ca35de60c44645bbae8a215061b33";
    std::string appCertificate = "5cfd2fd1755d40ecb72977518be15d3b";
    uint32_t now = ::time(NULL);

    if (key.content.serviceType != DynamicKey6::MEDIA_CHANNEL_SERVICE) {
        printf("INVALID_SERVICE_TYPE Key type is %u, expecting a channel key %u. \n\n", key.content.serviceType, DynamicKey6::MEDIA_CHANNEL_SERVICE);
    }

    if (key.content.expiredTs > 0 && key.content.expiredTs < now) {
        printf("DYNAMIC_KEY_EXPIRED Service is expired with ts %u, now %u \n\n", key.content.expiredTs, now);
    }

    if (isTimeout(key.content.unixTs, 0, now)) {
        printf("DYNAMIC_KEY_TIMEOUT Key is timeout with ts %u, now %u \n\n", key.content.unixTs, now);
    }

    if (appID != key.appID) {
        printf("INVALID_APP_ID %s, Expecting %s \n\n", key.appID.c_str(), appID.c_str());
    }

    std::string signatureWithUid = DynamicKey6::generateSignature(
        appCertificate
        , DynamicKey6::MEDIA_CHANNEL_SERVICE
        , appID
        , key.content.unixTs
        , key.content.salt
        , channelName
        , uid
        , key.content.expiredTs
        , key.content.extra);
    if (signatureWithUid != key.content.signature) {
        printf("Verify Key failed! Try not to verify UID \n\n");
        std::string signatureWithoutUid = DynamicKey6::generateSignature(
            appCertificate
            , DynamicKey6::MEDIA_CHANNEL_SERVICE
            , appID
            , key.content.unixTs
            , key.content.salt
            , channelName
            , 0
            , key.content.expiredTs
            , key.content.extra);
        if (signatureWithoutUid != key.content.signature) {
            printf("Fail to veriy signature %s. Expecting %s or %s . \n\n",
                key.content.signature.c_str()
                , signatureWithUid.c_str()
                , signatureWithoutUid.c_str()
            );
        } else {
            printf("Signature without UID matched. \n\n");
        }
    } else {
        printf("Signature with UID %u matched. \n\n", uid);
    }

    return 0;
}
