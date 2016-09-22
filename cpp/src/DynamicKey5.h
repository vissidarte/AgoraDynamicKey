#pragma once

#include "Packer.h"
#include "utils.h"

namespace agora { namespace tools {
    struct DynamicKey5 {
        enum ServiceType: uint16_t {
            MEDIA_CHANNEL_SERVICE = 1,
            RECORDING_SERVICE = 2,
            PUBLIC_SHARING_SERVICE = 3,
            IN_CHANNEL_PERMISSION = 4,
        };

        enum InChannelPermissionKey: uint16_t {
            ALLOW_UPLOAD_IN_CHANNEL = 1,
        };

        const static std::string NO_UPLOAD;
        const static std::string AUDIO_VIDEO_UPLOAD;

        typedef std::map<uint16_t, std::string> extra_map;
        DECLARE_PACKABLE_8(Message, uint16_t,serviceType, std::string,appId, uint32_t,unixTs, uint32_t,salt, std::string,channelName, uint32_t,uid, uint32_t,expiredTs, extra_map,extra);
        DECLARE_PACKABLE_7(DynamicKey5Content, uint16_t,serviceType, std::string,signature, std::string,appId, uint32_t,unixTs, uint32_t,randomInt, uint32_t,expiredTs, extra_map,extra);

        static const std::string VERSION;
        std::string signature;
        std::string appID;
        uint32_t unixTs ;
        uint32_t randomInt;
        uint32_t expiredTs;
        extra_map extra;

        template<typename T>
        static std::string pack(const T& x)
        {
            packer p;
            p << x;
            return p.pack().body();
        }

        template<typename T>
        static void unpack(const std::string& data, T& x)
        {
            unpacker u(data.data(), data.length());
            u >> x;
        }

        bool fromString(const std::string& channelKeyString)
        {
            if (channelKeyString.substr(0, VERSION_LENGTH) != "005") {
                return false;
            }

            std::string rawContent = base64Decode(channelKeyString.substr(VERSION_LENGTH));
            if (rawContent.empty()) {
                return false;
            }

            DynamicKey5Content content;
            unpack(rawContent, content);

            this->signature = stringToHex(content.signature);
            this->appID = stringToHex(content.appId);
            this->unixTs = content.unixTs;
            this->randomInt = content.randomInt;
            this->expiredTs = content.expiredTs;
            this->extra = content.extra;
            return true;
        }

        static std::string generateSignature(const std::string& appCertificate, ServiceType service, const std::string& appId, uint32_t unixTs, uint32_t salt, const std::string& channelName, uint32_t uid, uint32_t expiredTs, const extra_map& extra)
        {
            std::string rawAppId = hexDecode(appId);
            Message m(service, rawAppId, unixTs, salt, channelName, uid, expiredTs, extra);
            std::string toSign = pack(m);
            return singleton<crypto>::instance()->hmac_sign2(appCertificate, toSign, HMAC_LENGTH);
        }

        static std::string generateDynamicKey(
                const std::string& appID
                , const std::string& appCertificate
                , const std::string& channelName
                , uint32_t unixTs
                , uint32_t randomInt
                , uint32_t uid
                , uint32_t expiredTs
                , const extra_map& extra
                , ServiceType service
                )
        {
            std::string  signature = generateSignature(appCertificate, service, appID, unixTs, randomInt, channelName, uid, expiredTs, extra);
            DynamicKey5Content content(service, signature, hexDecode(appID), unixTs, randomInt, expiredTs, extra);
            return VERSION + base64Encode(pack(content));
        }

        static std::string generateMediaChannelKey(const std::string& appID
            , const std::string& appCertificate
            , const std::string& channelName
            , uint32_t unixTs
            , uint32_t randomInt
            , uint32_t uid
            , uint32_t expiredTs)   
        {
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, extra_map(), MEDIA_CHANNEL_SERVICE);
        }
        
        static std::string generateRecordingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
        {            
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, extra_map(), RECORDING_SERVICE);
        }

        static std::string generatePublicSharingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs)   
        {
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, extra_map(), PUBLIC_SHARING_SERVICE);
        }

        static std::string generateInChannelPermissionKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t randomInt, uint32_t uid, uint32_t expiredTs, const std::string& permission)
        {
            extra_map extra;
            extra[ALLOW_UPLOAD_IN_CHANNEL] = permission;
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, extra, IN_CHANNEL_PERMISSION);
        }

    };

    const std::string DynamicKey5::VERSION="005";
    const std::string DynamicKey5::NO_UPLOAD = "0";
    const std::string DynamicKey5::AUDIO_VIDEO_UPLOAD = "3";

}}
