#pragma once

#include <string>
#include <sstream>
#include <openssl/hmac.h>
#include <stdexcept>
#include <iomanip>
#include <cstddef>

namespace agora { namespace tools {
      static const std::string DYNAMIC_KEY_V3="003";
      struct L
      {
          static const uint32_t HMAC_LENGTH = 20;
          static const uint32_t SIGNATURE_LENGTH = 40;
          static const uint32_t STATIC_KEY_LENGTH = 32;
          static const uint32_t UNIX_TS_LENGTH = 10;
          static const uint32_t RANDOM_INT_LENGTH = 8;
          static const uint32_t UID_LENGTH = 10;
          static const uint32_t VERSION_LENGTH = 3;
          static const uint32_t DYNAMIC_KEY_LENGTH = SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH;
          static const uint32_t DYNAMIC_KEY_LENGTH2 = SIGNATURE_LENGTH + STATIC_KEY_LENGTH + UNIX_TS_LENGTH + RANDOM_INT_LENGTH + UNIX_TS_LENGTH + UID_LENGTH ;

          static const uint32_t SIGNATURE_OFFSET = 0;
          static const uint32_t STATIC_KEY_OFFSET = SIGNATURE_LENGTH;
          static const uint32_t UNIX_TS_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH;
          static const uint32_t RANDOM_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH;
          static const uint32_t UID_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH;
          static const uint32_t EXPIREDTS_INT_OFFSET = SIGNATURE_LENGTH+STATIC_KEY_LENGTH+UNIX_TS_LENGTH+RANDOM_INT_LENGTH+UID_LENGTH;
      };

    template <class T>
        class singleton
        {
            public:
                static T* instance()
                {
                    static T inst;
                    return &inst;
                }
            protected:
                singleton(){}
                virtual ~singleton(){}
            private:
                singleton(const singleton&);
                singleton& operator = (const singleton& rhs);
        };

    class crypto : public singleton<crypto>
    {
        public:
            // HMAC
            std::string hmac_sign(const std::string& message)
            {
                return hmac_sign2(hmac_key_, message, 20);
            }

            std::string hmac_sign2(const std::string& signkey, const std::string& message, uint32_t signSize)
            {
                if (signkey.empty()) {
                    /*throw std::runtime_error("empty hmac key");*/
                    return "";
                }
                return std::string((char *)HMAC(EVP_sha1()
                            , (const unsigned char*)signkey.data()
                            , signkey.length()
                            , (const unsigned char*)message.data()
                            , message.length(), NULL, NULL)
                        , signSize);
            }

            bool hmac_verify(const std::string & message, const std::string & signature)
            {
                return signature == hmac_sign(message);
            }
        private:
            std::string hmac_key_;
    };


    inline std::string stringToHex(const std::string& in)
    {
        static const char hexTable[]= "0123456789abcdef";

        if (in.empty()) {
            return std::string();
        }
        std::string out(in.size()*2, '\0');
        for (uint32_t i = 0; i < in.size(); ++i){
            out[i*2 + 0] = hexTable[(in[i] >> 4) & 0x0F];
            out[i*2 + 1] = hexTable[(in[i]     ) & 0x0F];
        }
        return out;
    }

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

    struct SignatureContent2{
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


    struct DynamicKey{
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        std::string toString() const
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
            this->signature = dynamicKeyString.substr(L::SIGNATURE_OFFSET, L::SIGNATURE_LENGTH);
            this->staticKey = dynamicKeyString.substr(L::STATIC_KEY_OFFSET, L::STATIC_KEY_LENGTH);
            try {
                this->unixTs =std::stoul(dynamicKeyString.substr(L::UNIX_TS_OFFSET, L::UNIX_TS_LENGTH),nullptr, 10);
                this->randomInt =std::stoul(dynamicKeyString.substr(L::RANDOM_INT_OFFSET, L::RANDOM_INT_LENGTH),nullptr, 16);
            } catch(std::exception& e) {
                return false;
            }
            return true;
        }
    };
    struct DynamicKey2{
        std::string signature;
        std::string staticKey;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t uid;
        uint32_t expiredTs;
        std::string toString() const
        {
            std::stringstream ss;
            ss << signature
                << staticKey
                << std::setfill ('0') << std::setw(10) << unixTs
                << std::setfill ('0') << std::setw(8) << std::hex << randomInt
                << std::setfill ('0') << std::setw(10) << std::dec<< uid
                << std::setfill ('0') << std::setw(10) << expiredTs;
            return ss.str();
        }
        bool fromString(const std::string& dynamicKeyString)
        {
              if (dynamicKeyString.length() != L::DYNAMIC_KEY_LENGTH2) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(L::SIGNATURE_OFFSET, L::SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(L::STATIC_KEY_OFFSET, L::STATIC_KEY_LENGTH);
              try {
                  this->unixTs =std::stoul(dynamicKeyString.substr(L::UNIX_TS_OFFSET, L::UNIX_TS_LENGTH),nullptr, 10);
                  this->randomInt =std::stoul(dynamicKeyString.substr(L::RANDOM_INT_OFFSET, L::RANDOM_INT_LENGTH),nullptr, 16);
                  this->uid =std::stoul(dynamicKeyString.substr(L::UID_INT_OFFSET, L::UID_LENGTH),nullptr, 10);
                  this->expiredTs =std::stoul(dynamicKeyString.substr(L::EXPIREDTS_INT_OFFSET, L::UNIX_TS_LENGTH),nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
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
              if (dynamicKeyString.length() != (L::DYNAMIC_KEY_LENGTH2+L::VERSION_LENGTH)) {
                  return false;
              }
              this->signature = dynamicKeyString.substr(L::VERSION_LENGTH +L::SIGNATURE_OFFSET, L::SIGNATURE_LENGTH);
              this->staticKey = dynamicKeyString.substr(L::VERSION_LENGTH +L::STATIC_KEY_OFFSET, L::STATIC_KEY_LENGTH);
              try {
                  this->unixTs = std::stoul(dynamicKeyString.substr(L::VERSION_LENGTH +L::UNIX_TS_OFFSET, L::UNIX_TS_LENGTH), nullptr, 10);
                  this->randomInt = std::stoul(dynamicKeyString.substr(L::VERSION_LENGTH +L::RANDOM_INT_OFFSET, L::RANDOM_INT_LENGTH), nullptr, 16);
                  this->uid = std::stoul(dynamicKeyString.substr(L::VERSION_LENGTH +L::UID_INT_OFFSET, L::UID_LENGTH), nullptr, 10);
                  this->expiredTs = std::stoul(dynamicKeyString.substr(L::VERSION_LENGTH +L::EXPIREDTS_INT_OFFSET, L::UNIX_TS_LENGTH), nullptr, 10);
              } catch(std::exception& e) {
                  return false;
              }
              return true;
          }
    };

    static const uint32_t HMAC_LENGTH = 20;
    std::string generateSignature(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt)
    {
        SignatureContent signContent;
        signContent.staticKey = staticKey;
        signContent.unixTs = unixTs;
        signContent.randomInt = randomInt;
        signContent.channelName = channelName;

        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, signContent.pack(), HMAC_LENGTH));
    }
    static std::string generateSignature2(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   {
        SignatureContent2 signContent;                                                                                                                                  
        signContent.staticKey = staticKey;                                                                                                                             
        signContent.unixTs = unixTs;                                                                                                                                   
        signContent.randomInt = randomInt;                                                                                                                             
        signContent.channelName = channelName;                                                                                                                         
        signContent.uid = uid;                                                                                                                         
        signContent.expiredTs = expiredTs; 

        return stringToHex(singleton<crypto>::instance()->hmac_sign2(signKey, signContent.pack(), HMAC_LENGTH));                 
    }

    std::string generateDynamicKey(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt)
    {
        DynamicKey dynamicKey;
        dynamicKey.signature = generateSignature(staticKey, signKey, channelName, unixTs, randomInt);
        dynamicKey.staticKey = staticKey;
        dynamicKey.unixTs = unixTs;
        dynamicKey.randomInt = randomInt;

        return dynamicKey.toString();
    }
    std::string generateDynamicKey2(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
        DynamicKey2 dynamicKey;                                                                                                                                         
        dynamicKey.signature = generateSignature2(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
        dynamicKey.staticKey = staticKey;                                                                                                                  
        dynamicKey.unixTs = unixTs;                                                                                                                        
        dynamicKey.randomInt = randomInt;                                                                                                                  
        dynamicKey.uid = uid;                                                                                                                  
        dynamicKey.expiredTs = expiredTs;                                                                                                                  

        return dynamicKey.toString();                                                                                                                                  
    }
    std::string generateDynamicKey3(const std::string& staticKey, const std::string& signKey, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
    {                                                                                                                                                                  
        DynamicKey3 dynamicKey;                                                                                                                                         
        dynamicKey.signature = generateSignature2(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
        dynamicKey.staticKey = staticKey;                                                                                                                  
        dynamicKey.unixTs = unixTs;                                                                                                                        
        dynamicKey.randomInt = randomInt;                                                                                                                  
        dynamicKey.uid = uid;                                                                                                                  
        dynamicKey.expiredTs = expiredTs;                                                                                                                  

        return dynamicKey.toString();                                                                                                                                  
    }
}}
