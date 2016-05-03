#pragma once

#include "utils.h"

namespace agora { namespace tools {

    enum{
        TYPE_DYNAMIC_KEY = 0,
        TYPE_RECORD_TICKET = 1,
    };

    struct L4
    {
        static const uint32_t DYNAMIC_KEY_LENGTH = VERSION_LENGTH + SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH + UNIX_TS_LENGTH ;
        static const uint32_t SIGNATURE_OFFSET = VERSION_LENGTH;
        static const uint32_t STATIC_KEY_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH;
        static const uint32_t UNIX_TS_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
        static const uint32_t RANDOM_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
        static const uint32_t EXPIREDTS_INT_OFFSET = VERSION_LENGTH+SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
    };

    struct DynamicKey4{
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t expiredTs;
        std::string toString() const
        {
            std::stringstream ss;
            ss  << "004" 
                << signature
                << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                << std::setfill ('0') << std::setw(10) << std::dec<< expiredTs;
            return ss.str();
        }

        bool fromString(const std::string& dynamicKeyString)
        {
              if (dynamicKeyString.length() != (L4::DYNAMIC_KEY_LENGTH)) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(L4::SIGNATURE_OFFSET, SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(L4::STATIC_KEY_OFFSET, STATIC_KEY_LENGTH);
              try {
                  this->unixTs = std::stoul(dynamicKeyString.substr(L4::UNIX_TS_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
                  this->randomInt = std::stoul(dynamicKeyString.substr(L4::RANDOM_INT_OFFSET, RANDOM_INT_LENGTH), nullptr, 16);
                  this->expiredTs = std::stoul(dynamicKeyString.substr(L4::EXPIREDTS_INT_OFFSET, UNIX_TS_LENGTH), nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
          }
    };

    static std::string generateSignature4(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   {
        std::stringstream ss;
        ss<<"ACS"
            << std::setfill ('\0') << std::setw(32) << staticKey
            << std::setfill ('0') << std::setw(10) << unixTs
            << std::setfill ('0') << std::setw(8) << std::hex << randomInt
            << channelName
            << std::setfill ('0') << std::setw(10) << std::dec<<uid
            << std::setfill ('0') << std::setw(10) << expiredTs;
        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, ss.str(), HMAC_LENGTH));                 
    }

    static std::string generateRecordSignature(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   {
        std::stringstream ss;
        ss<< "ARS"
            << std::setfill ('\0') << std::setw(32) << staticKey
            << std::setfill ('0') << std::setw(10) << unixTs
            << std::setfill ('0') << std::setw(8) << std::hex << randomInt
            << channelName
            << std::setfill ('0') << std::setw(10) << std::dec<<uid
            << std::setfill ('0') << std::setw(10) << expiredTs;
        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, ss.str(), HMAC_LENGTH));                 
    }

    std::string generateDynamicKey4(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs, uint8_t type)   
    {                                                                                                                                                                  
        DynamicKey4 dynamicKey;                                                                                                                                         
        switch(type){
            case TYPE_DYNAMIC_KEY:
                dynamicKey.signature = generateSignature4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
                break;
            case TYPE_RECORD_TICKET:
                dynamicKey.signature = generateRecordSignature(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
                break;
            default:
                return "";
        }
        dynamicKey.staticKey = staticKey;                                                                                                                  
        dynamicKey.unixTs = unixTs;                                                                                                                        
        dynamicKey.randomInt = randomInt;                                                                                                                  
        dynamicKey.expiredTs = expiredTs;                                                                                                                  

        return dynamicKey.toString();                                                                                                                                  
    }

}}
