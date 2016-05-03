import hmac
from hashlib import sha1
import sys
import time

TYPE_DYNAMIC_KEY = 0
TYPE_RECORD_TICKET = 1


def generateRecordSignature(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = "ARS" + key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName) +\
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature


def generateSignature4(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = "ACS" + key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName) +\
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature


def generateDynamicKey4(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs,
        type):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    if type == TYPE_DYNAMIC_KEY:
        signature_fun = generateSignature4
    elif type == TYPE_RECORD_TICKET:
        signature_fun = generateRecordSignature
    else:
        return
    signature = signature_fun(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs)
    version = '{0:0>3}'.format(4)
    ret = version + str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        '{:0>10}'.format(expiredTs)
    return ret
