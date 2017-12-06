#pragma once

#include <zlib.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include "AgoraDynamicKey/cpp/src/utils.h"

namespace agora {
namespace tools {

struct DynamicKey6 {
  enum Privileges {
    kJoinChannel = 1,
    kPublishAudioStream = 2,
    kPublishVideoStream = 3,
    kPublishDataStream = 4,
    kPublishAudiocdn = 5,

    kPublishVideoCdn = 6,
    kHostinAudioStream = 7,
    kHostinVideoStream = 8,
    kHostinDataStream = 9,
    kInviteAudioStream = 10,
    kInviteVideoStream = 11,
    kInviteDataStream = 12,

    kAdministrateChannel = 101,
  };

  typedef std::map<uint16_t, uint32_t> PrivilegeMessageMap;

  TOOLS_DECLARE_PACKABLE_3(PrivilegeMessage, uint32_t, salt, uint32_t, ts,
                           PrivilegeMessageMap, messages);
  TOOLS_DECLARE_PACKABLE_4(PackContent, std::string, signature, uint32_t,
                           crcChannelName, uint32_t, crcUid, std::string,
                           rawMessage);

  static std::string Version() { return "006"; }

  static std::string GenerateSignature(const std::string& appCertificate,
                                       const std::string& appID,
                                       const std::string& channelName,
                                       const std::string& uid,
                                       const std::string& message) {
    std::stringstream ss;
    ss << appID << channelName << uid << message;
    return (HmacSign2(appCertificate, ss.str(), HMAC_LENGTH));
  }

  DynamicKey6() : crc_channel_name_(0), crc_uid_(0) {}

  DynamicKey6(const std::string& appId, const std::string& appCertificate,
              const std::string& channelName, uint32_t uid = 0)
      : app_id_(appId),
        app_cert_(appCertificate),
        channel_name_(channelName),
        crc_channel_name_(0),
        crc_uid_(0) {
    std::stringstream uidStr;
    if (uid == 0) {
      uidStr << "";
    } else {
      uidStr << uid;
    }
    uid_ = uidStr.str();
    uint32_t now = time(NULL);
    std::srand(now);
    message_.salt = GenerateSalt();
    message_.ts = now + 34 * 3600;
  }

  DynamicKey6(const std::string& appId, const std::string& appCertificate,
              const std::string& channelName, const std::string& uid = "")
      : app_id_(appId),
        app_cert_(appCertificate),
        channel_name_(channelName),
        uid_(uid),
        crc_channel_name_(0),
        crc_uid_(0) {
    uint32_t now = time(NULL);
    std::srand(now);
    message_.salt = GenerateSalt();
    message_.ts = now + 34 * 3600;
  }

  std::string Build() {
    if (!IsUUID(app_id_)) {
      perror("invalid appID");
      return "";
    }

    if (!IsUUID(app_cert_)) {
      perror("invalid appCertificate");
      return "";
    }
    if (uid_ != "") {
      try {
        std::stoul(uid_);
      } catch (...) {
        perror("invalid uid");
        return "";
      }
    }
    message_raw_content_ = Pack(message_);
    signature_ = GenerateSignature(app_cert_, app_id_, channel_name_, uid_,
                                   message_raw_content_);
    crc_channel_name_ = crc32(
        0, reinterpret_cast<Bytef*>(const_cast<char*>(channel_name_.c_str())),
        channel_name_.length());
    crc_uid_ =
        crc32(0, reinterpret_cast<Bytef*>(const_cast<char*>(uid_.c_str())),
              uid_.length());
    PackContent content;
    content.signature = signature_;
    content.crcChannelName = crc_channel_name_;
    content.crcUid = crc_uid_;
    content.rawMessage = message_raw_content_;
    std::stringstream ss;
    ss << DynamicKey6::Version() << app_id_ << base64Encode(Pack(content));
    return ss.str();
  }

  void AddPrivilege(Privileges privilege, uint32_t timeoutFromNow = 0) {
    message_.messages[privilege] = timeoutFromNow;
  }

  bool FromString(const std::string& channelKeyString) {
    if (channelKeyString.substr(0, VERSION_LENGTH) != Version()) {
      return false;
    }
    try {
      app_id_ = channelKeyString.substr(VERSION_LENGTH, APP_ID_LENGTH);
      PackContent content;
      Unpack(base64Decode(channelKeyString.substr(
                 VERSION_LENGTH + APP_ID_LENGTH, channelKeyString.size())),
             content);
      signature_ = content.signature;
      crc_channel_name_ = content.crcChannelName;
      crc_uid_ = content.crcUid;
      message_raw_content_ = content.rawMessage;
      Unpack(message_raw_content_, message_);
    } catch (std::exception& e) {
      return false;
    }
    return true;
  }

  std::string app_id_;
  std::string app_cert_;
  std::string channel_name_;
  std::string uid_;
  std::string signature_;
  PrivilegeMessage message_;
  std::string message_raw_content_;
  uint32_t crc_channel_name_;
  uint32_t crc_uid_;
};
}  // namespace tools
}  // namespace agora