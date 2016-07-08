import hmac
from hashlib import sha1
import sys
import time
import ctypes

def generatePublicSharingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs):
    return generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "APSS")

def generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs):
    return generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ARS")

def generateMediaChannelKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs):
    return generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ACS")

def generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype):
    uid = ctypes.c_uint(uid).value
    key = "\x00" * (32 - len(staticKey)) + staticKey
    signature = generateSignature(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype)
    version = '{0:0>3}'.format(4)
    ret = version + str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt) & 0xFFFFFFFF) + \
        '{:0>10}'.format(expiredTs)
    return ret

def generateSignature(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = servicetype + key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt) & 0xFFFFFFFF) + \
        str(channelName) +\
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature
