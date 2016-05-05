#pragma once

#include "utils.h"

namespace agora { namespace tools {

    struct DynamicKey3{
        static const uint32_t DYNAMIC_KEY_LENGTH = VERSION_LENGTH+SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH + UNIX_TS_LENGTH + UID_LENGTH ;
        static const uint32_t SIGNATURE_OFFSET = 0;
        static const uint32_t STATIC_KEY_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH;
        static const uint32_t UNIX_TS_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
        static const uint32_t RANDOM_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
        static const uint32_t UID_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
        static const uint32_t EXPIREDTS_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;

        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t uid;
        uint32_t expiredTs;
        static std::string toString(const std::string& staticKey, const std::string& signature,  uint32_t unixTs, uint32_t randomInt,uint32_t uid, uint32_t expiredTs) 
        {
            std::stringstream ss;
            ss  << "003" 
                << signature
                << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                << std::setfill ('0') << std::setw(10) << std::dec<< uid
                << std::setfill ('0') << std::setw(10) << expiredTs;
            return ss.str();
        }
        bool fromString(const std::string& dynamicKeyString)
        {
              if (dynamicKeyString.length() != DYNAMIC_KEY_LENGTH) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(SIGNATURE_OFFSET, SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(STATIC_KEY_OFFSET, STATIC_KEY_LENGTH);
              try {
                  this->unixTs = std::stoul(dynamicKeyString.substr(UNIX_TS_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
                  this->randomInt = std::stoul(dynamicKeyString.substr(RANDOM_INT_OFFSET, RANDOM_INT_LENGTH), nullptr, 16);
                  this->uid = std::stoul(dynamicKeyString.substr(UID_INT_OFFSET, UID_LENGTH), nullptr, 10);
                  this->expiredTs = std::stoul(dynamicKeyString.substr(EXPIREDTS_INT_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
          }

    static std::string generateSignature(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   {
        std::stringstream ss;
        ss << std::setfill ('\0') << std::setw(32) << staticKey
            << std::setfill ('0') << std::setw(10) << unixTs
            << std::setfill ('0') << std::setw(8) << std::hex << randomInt
            << channelName
            << std::setfill ('0') << std::setw(10) << std::dec<<uid
            << std::setfill ('0') << std::setw(10) << expiredTs;
        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, ss.str(), HMAC_LENGTH));                 
    }

    static std::string generate(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
        std::string signature = generateSignature(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
        return toString(staticKey, signature,  unixTs, randomInt, uid, expiredTs);                                                                                                                                  
    }

    };
}}
