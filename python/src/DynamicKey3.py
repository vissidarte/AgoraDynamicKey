import hmac
from hashlib import sha1
import sys

import ctypes

def generateSignaure(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName) +\
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature


def generate(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    uid = ctypes.c_uint(uid).value
    key = "\x00" * (32 - len(staticKey)) + staticKey
    signature = generateSignaure(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs)
    version = '{0:0>3}'.format(3)
    ret = version + str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    return ret
