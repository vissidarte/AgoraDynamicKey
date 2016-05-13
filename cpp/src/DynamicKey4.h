#pragma once

#include "utils.h"

namespace agora { namespace tools {
    struct DynamicKey4{
        static const uint32_t DYNAMIC_KEY_LENGTH = VERSION_LENGTH + SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH + UNIX_TS_LENGTH ;
        static const uint32_t SIGNATURE_OFFSET = VERSION_LENGTH;
        static const uint32_t STATIC_KEY_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH;
        static const uint32_t UNIX_TS_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
        static const uint32_t RANDOM_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
        static const uint32_t EXPIREDTS_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
        static const std::string VERSION;
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t expiredTs;

        static std::string toString(const std::string& staticKey, const std::string& signature,  uint32_t unixTs, uint32_t randomInt, uint32_t expiredTs) 
        {
            std::stringstream ss;
            ss  << DynamicKey4::VERSION
                << signature
                << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                << std::setfill ('0') << std::setw(10) << std::dec<< expiredTs;
            return ss.str();
        }

        bool fromString(const std::string& dynamicKeyString)
        {
              if (dynamicKeyString.length() != (DYNAMIC_KEY_LENGTH)) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(SIGNATURE_OFFSET, SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(STATIC_KEY_OFFSET, STATIC_KEY_LENGTH);
              try {
                  this->unixTs = std::stoul(dynamicKeyString.substr(UNIX_TS_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
                  this->randomInt = std::stoul(dynamicKeyString.substr(RANDOM_INT_OFFSET, RANDOM_INT_LENGTH), nullptr, 16);
                  this->expiredTs = std::stoul(dynamicKeyString.substr(EXPIREDTS_INT_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
          }

        static std::string generateSignature4(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs, std::string service)   {
        std::stringstream ss;
        ss<< service
            << std::setfill ('\0') << std::setw(32) << staticKey
            << std::setfill ('0') << std::setw(10) << unixTs
            << std::setfill ('0') << std::setw(8) << std::hex << randomInt
            << channelName
            << std::setfill ('0') << std::setw(10) << std::dec<<uid
            << std::setfill ('0') << std::setw(10) << expiredTs;
        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, ss.str(), HMAC_LENGTH));                 
    }

        static std::string generateMediaChannelKey(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
       std::string  signature = generateSignature4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, MEDIA_CHANNEL_SERVICE);

        return toString(staticKey,signature,  unixTs, randomInt,  expiredTs);                                                                                                                                  
    }
    
    static std::string generateRecordingKey(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
        std::string signature = generateSignature4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, RECORDING_SERVICE);
        
        return toString(staticKey,signature, unixTs, randomInt, expiredTs);                                                                                                                                  
    }

    };
    const std::string DynamicKey4::VERSION="004";

}}
