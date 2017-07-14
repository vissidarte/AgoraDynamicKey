#pragma once

#include "Packer.h"
#include "utils.h"
#include <iostream>
#include <ctime>
#include <cstdint>

namespace agora { namespace tools {
    struct DynamicKey6 {
        enum ServiceType: uint16_t {
            MEDIA_CHANNEL_SERVICE = 1,
            RECORDING_SERVICE = 2,
            PUBLIC_SHARING_SERVICE = 3,
            IN_CHANNEL_PERMISSION = 4,
        };

        enum InChannelPermissionKey: uint16_t {
            ALLOW_UPLOAD_IN_CHANNEL = 1,
        };

        static std::string version() { return "006"; }
        static std::string noUpload() { return "0"; }
        static std::string audioVideoUpload() { return "3"; }

        typedef std::map<uint16_t, std::string> extra_map;
        TOOLS_DECLARE_PACKABLE_8(Message, uint16_t,serviceType, std::string,appID, uint32_t,unixTs, uint32_t,salt, std::string,channelName, uint32_t,uid, uint32_t,expiredTs, extra_map,extra);
        TOOLS_DECLARE_PACKABLE_7(DynamicKey6Content, uint16_t,serviceType, std::string,signature, uint32_t,uid, uint32_t,unixTs, uint32_t,salt, uint32_t,expiredTs, extra_map,extra);

        std::string signature;
        std::string appID;
        uint32_t uid;
        uint32_t unixTs ;
        uint32_t salt;
        uint32_t expiredTs;
        extra_map extra;

        bool fromString(const std::string& keyString)
        {
            if (keyString.substr(0, VERSION_LENGTH) != version()) {
                return false;
            }

            this->appID = keyString.substr(VERSION_LENGTH, APP_ID_LENGTH);
            if (! isUUID(this->appID)) {
                return false;
            }

            std::string rawContent = base64Decode(keyString.substr(VERSION_LENGTH + APP_ID_LENGTH));
            if (rawContent.empty()) {
                return false;
            }

            DynamicKey6Content content;
            unpack(rawContent, content);

            this->signature = content.signature;
            this->uid = content.uid;
            this->unixTs = content.unixTs;
            this->salt = content.salt;
            this->expiredTs = content.expiredTs;
            this->extra = content.extra;
            return true;
        }

        static std::string generateSignature(const std::string& appCertificate, ServiceType service, const std::string& appID, uint32_t unixTs, uint32_t salt, const std::string& channelName, uint32_t uid, uint32_t expiredTs, const extra_map& extra)
        {
            // decode hex to avoid case problem
            std::string rawAppID = hexDecode(appID);
            std::string rawAppCertificate = hexDecode(appCertificate);

            Message m(service, rawAppID, unixTs, salt, channelName, uid, expiredTs, extra);
            std::string toSign = pack(m);
            return stringToHEX(hmac_sign2(rawAppCertificate, toSign, HMAC_LENGTH));
        }

        static std::string generateDynamicKey(
                const std::string& appID
                , const std::string& appCertificate
                , const std::string& channelName
                , uint32_t unixTs
                , uint32_t salt
                , uint32_t uid
                , uint32_t expiredTs
                , const extra_map& extra
                , ServiceType service
                )
        {
            if (! isUUID(appID)) {
                perror("invalid appID");
                return "";
            }

            if (! isUUID(appCertificate)) {
                perror("invalid appCertificate");
                return "";
            }

            std::string signature = generateSignature(appCertificate, service, appID, unixTs, salt, channelName, uid, expiredTs, extra);
            DynamicKey6Content content(service, signature, uid, unixTs, salt, expiredTs, extra);
            return version() + appID + base64Encode(pack(content));
        }

        static std::string generateMediaChannelKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName)
        {
            return generateMediaChannelKey(appID, appCertificate, channelName, ::time(nullptr), generateSalt(), 0, 0);
        }

        static std::string generateMediaChannelKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t uid)
        {
            return generateMediaChannelKey(appID, appCertificate, channelName, ::time(nullptr), generateSalt(), uid, 0);
        }

        static std::string generateMediaChannelKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t uid, uint32_t kickTimestamp)
        {
            return generateMediaChannelKey(appID, appCertificate, channelName, ::time(nullptr), generateSalt(), uid, kickTimestamp);
        }

        static std::string generateMediaChannelKey(const std::string& appID
            , const std::string& appCertificate
            , const std::string& channelName
            , uint32_t unixTs
            , uint32_t salt
            , uint32_t uid
            , uint32_t kickTimestamp)   
        {
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, kickTimestamp, extra_map(), MEDIA_CHANNEL_SERVICE);
        }

        static std::string generateRecordingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName)
        {
            return generateRecordingKey(appID, appCertificate, channelName, ::time(NULL), generateSalt(), 0, 0);
        }

        static std::string generateRecordingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t uid)
        {
            return generateRecordingKey(appID, appCertificate, channelName, ::time(NULL), generateSalt(), uid, 0);
        }

        static std::string generateRecordingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t salt, uint32_t uid, uint32_t reserved)   
        {            
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, reserved, extra_map(), RECORDING_SERVICE);
        }

        static std::string generatePublicSharingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName)
        {
            return generatePublicSharingKey(appID, appCertificate, channelName, ::time(NULL), generateSalt(), 0, 0);
        }

        static std::string generatePublicSharingKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t salt, uint32_t uid, uint32_t reserved)   
        {
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, reserved, extra_map(), PUBLIC_SHARING_SERVICE);
        }

        static std::string generateInChannelPermissionKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t uid, const std::string& permission)
        {
            return generateInChannelPermissionKey(appID, appCertificate, channelName, ::time(NULL), generateSalt(), uid, 0, permission);
        }

        static std::string generateInChannelPermissionKey(const std::string& appID, const std::string& appCertificate, const std::string& channelName, uint32_t unixTs, uint32_t salt, uint32_t uid, uint32_t revokeTimestamp, const std::string& permission)
        {
            extra_map extra;
            extra[ALLOW_UPLOAD_IN_CHANNEL] = permission;
            return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, revokeTimestamp, extra, IN_CHANNEL_PERMISSION);
        }
    };
}}
