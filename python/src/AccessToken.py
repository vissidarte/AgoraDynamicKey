import hmac
from hashlib import sha256
import ctypes
import base64
import struct
from binascii import crc32


kJoinChannel = 1
kPublishAudioStream = 2
kPublishVideoStream = 3
kPublishDataStream = 4
kPublishAudiocdn = 5
kPublishVideoCdn = 6
kRequestPublishAudioStream = 7
kRequestPublishVideoStream = 8
kRequestPublishDataStream = 9
kInvitePublishAudioStream = 10
kInvitePublishVideoStream = 11
kInvitePublishDataStream = 12
kAdministrateChannel = 101


def packUint16(x):
    return struct.pack('<H',int(x))

def packUint32(x):
    return struct.pack('<I',int(x))

def packInt32(x):
    return struct.pack('<i', int(x))

def packString(string):
    return packUint16(len(string)) + string

def packMap(m):
    ret = packUint16(len(m.items()))
    for k, v in m.items():
        ret += packUint16(k) + packString(v)
    return ret

def packMapUint32(m):
    ret = packUint16(len(m.items()))
    for k, v in m.items():
        ret += packUint16(k) + packUint32(v)
    return ret

class AccessToken:

    def __init__(self, appID, appCertificate, channelName, uid, salt, ts, msgType, expiredTs):
        extra = {}
        extra[msgType] = expiredTs
        self.appID = appID
        self.appCertificate = appCertificate
        self.channelName = channelName
        self.uid = uid
        self.message = packUint32(salt) \
            + packUint32(ts) \
            + packMapUint32(extra)

    def build(self):
        val = self.appID + self.channelName + self.uid + self.message
        signature = hmac.new(self.appCertificate, val, sha256).digest()
        crc_channel_name = crc32(self.channelName) & 0xffffffff
        crc_uid = crc32(self.uid) & 0xffffffff

        content = packString(signature) \
            + packUint32(crc_channel_name) \
            + packUint32(crc_uid) \
            + packString(self.message)

        version = '{0:0>3}'.format(6)
        ret = version + self.appID + base64.b64encode(content)
        return ret 





