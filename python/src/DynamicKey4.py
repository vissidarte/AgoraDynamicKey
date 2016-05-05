import hmac
from hashlib import sha1
import sys
import time
import ctypes

TYPE_DYNAMIC_KEY = 0
TYPE_RECORD_TICKET = 1

def generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs):
    return generateDynamicKey4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ARS")

def generateMediaChannelKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs):
    return generateDynamicKey4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ACS")

def generateDynamicKey4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype):
    uid = ctypes.c_uint(uid).value
    key = "\x00" * (32 - len(staticKey)) + staticKey
    signature = generateSignature4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype)
    version = '{0:0>3}'.format(4)
    ret = version + str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        '{:0>10}'.format(expiredTs)
    return ret

def generateSignature4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, servicetype):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = servicetype + key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName) +\
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature
