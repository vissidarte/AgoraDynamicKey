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

  typedef std::map<uint16_t, uint32_t> PrevilidgeMessageMap;

  TOOLS_DECLARE_PACKABLE_3(PrevilidgeMessage, uint32_t, salt, uint32_t, ts,
                           PrevilidgeMessageMap, messages);

  static std::string Version() { return "006"; }

  static std::string GenerateSignature(const std::string& appCertificate,
                                       const std::string& appID,
                                       const std::string& channelName,
                                       const std::string& uid,
                                       const std::string& message) {
    
    std::stringstream ss;
    ss << appID << channelName << uid << message;
    return stringToHEX(hmac_sign2(appCertificate, ss.str(), HMAC_LENGTH));
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
    uidStr << uid;
    uid_ = uidStr.str();
    uint32_t now = time(NULL);
    std::srand(now);
    message_.salt = rand();
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
    message_.salt = rand();
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
    try {
      std::stoul(uid_);
    } catch (...) {
      perror("invalid uid");
      return "";
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

    std::stringstream ss;
    ss << DynamicKey6::Version() << signature_ << app_id_ << std::setfill('0')
       << std::setw(10) << crc_channel_name_ << std::setfill('0')
       << std::setw(10) << crc_uid_ << base64Encode(message_raw_content_);
    return ss.str();
  }

  int AddPrivilege(Privileges privilege, uint32_t timeoutFromNow = 0) {
    message_.messages.insert(std::make_pair(privilege, timeoutFromNow));
  }

  int SetDynamicKeyTimeout(uint32_t timeoutFromNow) {
    message_.ts = time(NULL) + timeoutFromNow;
  }

  bool FromString(const std::string& channelKeyString) {
    if (channelKeyString.substr(0, VERSION_LENGTH) != Version()) {
      return false;
    }
    int index = 0;
    index += VERSION_LENGTH;
    signature_ = channelKeyString.substr(index, SIGNATURE_LENGTH);
    index += SIGNATURE_LENGTH;
    app_id_ = channelKeyString.substr(index, APP_ID_LENGTH);
    index += APP_ID_LENGTH;
    try {
      crc_channel_name_ = std::stoul(
          channelKeyString.substr(index, UNIX_TS_LENGTH), nullptr, 10);
      index += UNIX_TS_LENGTH;
      crc_uid_ = std::stoul(channelKeyString.substr(index, UNIX_TS_LENGTH),
                            nullptr, 10);
      index += UNIX_TS_LENGTH;
      message_raw_content_ =
          base64Decode(channelKeyString.substr(index, channelKeyString.size()));
     Unpack(message_raw_content_, message_); 

    } catch (std::exception& e) {
      return false;
    }
    return true;
  }

  std::string channel_name_;
  std::string uid_;
  std::string signature_;
  std::string app_id_;
  std::string app_cert_;
  PrevilidgeMessage message_;
  std::string message_raw_content_;
  uint32_t crc_channel_name_;
  uint32_t crc_uid_;
};
}  // namespace tools
}  // namespace agora
