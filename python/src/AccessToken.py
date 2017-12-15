import hmac
from hashlib import sha256
import ctypes
import base64
import struct
from zlib import crc32
import random
import time

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
    return struct.pack('<H', int(x))


def packUint32(x):
    return struct.pack('<I', int(x))


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

    def __init__(self, appID, appCertificate, channelName, uid):
        random.seed(time.time())
        self.appID = appID
        self.appCertificate = appCertificate
        self.channelName = channelName
        self.uidStr = str(uid)
        self.ts = int(time.time())
        self.salt = random.randint(1, 99999999)
        self.messages = {}

    def AddPrivilege(self, key, secondsFromNow):
        self.messages[key] = int(time.time()) + secondsFromNow

    def build(self):
        m = packUint32(self.salt)  + packUint32(self.ts) \
             + packMapUint32(self.messages)

        val = self.appID + self.channelName + self.uidStr + m
        signature = hmac.new(self.appCertificate, val, sha256).digest()
        crc_channel_name = crc32(self.channelName) & 0xffffffff
        crc_uid = crc32(self.uidStr) & 0xffffffff

        content = packString(signature) \
            + packUint32(crc_channel_name) \
            + packUint32(crc_uid) \
            + packString(m)

        version = '{0:0>3}'.format(6)
        ret = version + self.appID + base64.b64encode(content)
        return ret
