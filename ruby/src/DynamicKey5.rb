module DynamicKey5
  module_function

  # def generate_dynamic_key(servicetype, appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, extra)
  #   uid = ctypes.c_uint(uid).value
  #   randomInt = ctypes.c_uint(randomInt).value
  #   signature = generateSignature(
  #       servicetype,
  #       appID,
  #       appCertificate,
  #       channelName,
  #       unixTs,
  #       randomInt,
  #       uid,
  #       expiredTs,
  #       extra
  #   )
  #   version = '{0:0>3}'.format(5)
  #   content = packUint16(servicetype) \
  #       + packString(signature)\
  #       + packString(appID.decode('hex'))\
  #       + packUint32(unixTs) \
  #       + packUint32(randomInt) \
  #       + packUint32(expiredTs)\
  #       + packMap(extra)
  #   return version + base64.b64encode(content)
  # end
  #
  # def generate_public_sharing_key(app_id, app_cert, channel_name, uid, valid_time)
  # end
  appCertificate = '5cfd2fd1755d40ecb72977518be15d3b'
  appID = '970ca35de60c44645bbae8a215061b33'

  rawAppId = [appID].pack("H*")
  rawAppCertificate = [appCertificate].pack("H*")

  service = 2
  unixTs = 1446455472
  salt = 58964981
  channelName = '7d72365eb983485397e3e3f9d460bdda'
  uid = 2882341273
  expiredTs = 1446455471
  extra = {}
  # string = "#{service}#{appID}#{unixTs}#{salt}#{channelName}#{uid}#{expiredTs}#{extra}"
  string = [service, rawAppId, unixTs, salt, channelName, uid, expiredTs].join("\n")
  digest = OpenSSL::Digest.new("sha1")
  puts OpenSSL::HMAC.hexdigest(digest, rawAppCertificate, string)

  # '970ca35de60c44645bbae8a215061b33'.bytes.every(2){|m1,m2|  f=m1.to_s.to_i(16)<<4;f|= m2.to_s.to_i(16);puts f&255 }

  # p "MNS #{KEYID}:#{Base64.encode64(sha1).chop}"
  # HMAC::MD5.hexdigest(appCertificate,string)
end
# import hmac
# from hashlib import sha1
# import ctypes
# import base64
# import struct

# # service type
# MEDIA_CHANNEL_SERVICE = 1
# RECORDING_SERVICE = 2
# PUBLIC_SHARING_SERVICE = 3
# IN_CHANNEL_PERMISSION = 4

# # extra key
# ALLOW_UPLOAD_IN_CHANNEL = 1

# # permision
# NoUpload = "0"
# AudioVideoUpload= "3"

# def generatePublicSharingKey(
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs
#         ):
#     return generateDynamicKey(
#         PUBLIC_SHARING_SERVICE,
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs,{}
#         )


# def generateRecordingKey(
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs):
#     return generateDynamicKey(
#         RECORDING_SERVICE,
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs,
#         {})


# def generateMediaChannelKey(
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs):
#     return generateDynamicKey(
#         MEDIA_CHANNEL_SERVICE,
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs,
#         {})


# def generateInChannelPermissionKey(
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs,permission ):
#     extra={}
#     extra[ALLOW_UPLOAD_IN_CHANNEL]=permission
#     return generateDynamicKey(
#         IN_CHANNEL_PERMISSION,
#         appID,
#         appCertificate,
#         channelName,
#         unixTs,
#         randomInt,
#         uid,
#         expiredTs,
#         extra)

# def packUint16(x)
#   return struct.pack('<H', int(x))
# end
#
# def packUint32(x)
#   return struct.pack('<I', int(x))
# end
#
# def packInt32(x)
#   return struct.pack('<i', int(x))
# end
#
# def packString(string)
#   return packUint16(len(string)) + string
# end
#
# def packMap(m)
#   ret = packUint16(len(m.items()))
#   for k, v in m.items() :
#     ret += packUint16(k) + packString(v)
#     return ret
#   end
# end
#
# def generate_signature(
#     servicetype,
#     appID,
#     appCertificate,
#     channelName,
#     unixTs,
#     randomInt,
#     uid,
#     expiredTs,
#     extra,)
#   $rawAppID = [appID].pack("H*")
#   $rawAppCertificate = [appCertificate].pack("H*")
#
#   $buffer = pack("S", $serviceType);
#   $buffer.= pack("S", strlen($rawAppID)).$rawAppID;
#   $buffer.= pack("I", $ts);
#   $buffer.= pack("I", $salt);
#   $buffer.= pack("S", strlen($channelName)).$channelName;
#   $buffer.= pack("I", $uid);
#   $buffer.= pack("I", $expiredTs);
#
#   $buffer.= pack("S", count($extra));
#   foreach ($extra as $key => $value) {
#       $buffer.= pack("S", $key);
#   $buffer.= pack("S", strlen($value)).$value;
#   }
#
#   return strtoupper(hash_hmac('sha1', $buffer, $rawAppCertificate));
#   # content = packUint16(servicetype) \
#   #       + packString(appID.decode('hex'))\
#   #       + packUint32(unixTs)  \
#   #       + packInt32(randomInt)  \
#   #       + packString(channelName) \
#   #       + packUint32(uid)\
#   #       + packUint32(expiredTs)\
#   #       + packMap(extra)
#   # signature = hmac.new(appCertificate.decode('hex'), content, sha1).hexdigest()
#   # return signature.upper()
# end
