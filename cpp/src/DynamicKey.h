#pragma once

#include "utils.h"

namespace agora { namespace tools {
    struct DynamicKey{
            struct SignatureContent{
                std::string staticKey;
                uint32_t unixTs;
                uint32_t randomInt ;
                std::string channelName;
                std::string pack(){
                    std::stringstream ss;
                    ss << std::setfill ('\0') << std::setw(32) << staticKey
                        << std::setfill ('0') << std::setw(10) << unixTs
                        << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                        << channelName;
                    return ss.str();
                }
            };
        static const uint32_t DYNAMIC_KEY_LENGTH = SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH;
        static const uint32_t SIGNATURE_OFFSET = 0;
        static const uint32_t STATIC_KEY_OFFSET = SIGNATURE_LENGTH;
        static const uint32_t UNIX_TS_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
        static const uint32_t RANDOM_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
        static const uint32_t UID_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
        static const uint32_t EXPIREDTS_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
        static const std::string VERSION;
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        static std::string toString(const std::string& staticKey, const std::string& signature, uint32_t unixTs, uint32_t randomInt) 
        {
            std::stringstream ss;
            ss << signature
                << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt;
            return ss.str();
        }

        bool fromString(const std::string& dynamicKeyString)
        {
            if (dynamicKeyString.length() != 90) {
                return false;
            }
            this->signature = dynamicKeyString.substr(SIGNATURE_OFFSET, SIGNATURE_LENGTH);
            this->staticKey = dynamicKeyString.substr(STATIC_KEY_OFFSET, STATIC_KEY_LENGTH);
            try {
                this->unixTs =std::stoul(dynamicKeyString.substr(UNIX_TS_OFFSET, UNIX_TS_LENGTH),nullptr, 10);
                this->randomInt =std::stoul(dynamicKeyString.substr(RANDOM_INT_OFFSET, RANDOM_INT_LENGTH),nullptr, 16);
            } catch(std::exception& e) {
                return false;
            }
            return true;
        }

    static std::string generateSignature(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt)
    {
        SignatureContent signContent;
        signContent.staticKey = staticKey;
        signContent.unixTs = unixTs;
        signContent.randomInt = randomInt;
        signContent.channelName = channelName;
        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, signContent.pack(), HMAC_LENGTH));
    }

    static std::string generate(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt)   
    {                                                                                                                                                                  
        std::string signature = generateSignature(staticKey, signKey, channelName, unixTs, randomInt);

        return toString(staticKey, signature, unixTs, randomInt);                                                                                                                                  
    }
    };
    const std::string DynamicKey3::VERSION="001";
}}
