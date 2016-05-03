#pragma once

#include "utils.h"

namespace agora { namespace tools {
      static const std::string DYNAMIC_KEY_V3="003";
      struct L2
      {
          static const uint32_t DYNAMIC_KEY_LENGTH = VERSION_LENGTH+SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH + UNIX_TS_LENGTH + UID_LENGTH ;
          static const uint32_t SIGNATURE_OFFSET = 0;
          static const uint32_t STATIC_KEY_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH;
          static const uint32_t UNIX_TS_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
          static const uint32_t RANDOM_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
          static const uint32_t UID_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
          static const uint32_t EXPIREDTS_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
      };

    struct SignatureContent3{
        std::string staticKey;
        uint32_t unixTs;
        uint32_t randomInt ;
        uint32_t uid;
        uint32_t expiredTs;
        std::string channelName;                                                                                                                         
        std::string pack(){
            std::stringstream ss;
            ss << std::setfill ('\0') << std::setw(32) << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                << channelName
                << std::setfill ('0') << std::setw(10) << std::dec<<uid
                << std::setfill ('0') << std::setw(10) << expiredTs;
            return ss.str();
        }
    };

    struct DynamicKey3{
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t uid;
        uint32_t expiredTs;
        std::string toString() const
        {
            std::stringstream ss;
            ss  << DYNAMIC_KEY_V3
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
              if (dynamicKeyString.length() != L2::DYNAMIC_KEY_LENGTH) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(L2::SIGNATURE_OFFSET, SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(L2::STATIC_KEY_OFFSET, STATIC_KEY_LENGTH);
              try {
                  this->unixTs = std::stoul(dynamicKeyString.substr(L2::UNIX_TS_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
                  this->randomInt = std::stoul(dynamicKeyString.substr(L2::RANDOM_INT_OFFSET, RANDOM_INT_LENGTH), nullptr, 16);
                  this->uid = std::stoul(dynamicKeyString.substr(L2::UID_INT_OFFSET, UID_LENGTH), nullptr, 10);
                  this->expiredTs = std::stoul(dynamicKeyString.substr(L2::EXPIREDTS_INT_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
          }
    };

    static std::string generateSignature3(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   {
        SignatureContent3 signContent;                                                                                                                                  
        signContent.staticKey = staticKey;                                                                                                                             
        signContent.unixTs = unixTs;                                                                                                                                   
        signContent.randomInt = randomInt;                                                                                                                             
        signContent.channelName = channelName;                                                                                                                         
        signContent.uid = uid;                                                                                                                         
        signContent.expiredTs = expiredTs; 

        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, signContent.pack(), HMAC_LENGTH));                 
    }

    std::string generateDynamicKey3(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
        DynamicKey3 dynamicKey;                                                                                                                                         
        dynamicKey.signature = generateSignature3(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
        dynamicKey.staticKey = staticKey;                                                                                                                  
        dynamicKey.unixTs = unixTs;                                                                                                                        
        dynamicKey.randomInt = randomInt;                                                                                                                  
        dynamicKey.uid = uid;                                                                                                                  
        dynamicKey.expiredTs = expiredTs;                                                                                                                  

        return dynamicKey.toString();                                                                                                                                  
    }
}}
