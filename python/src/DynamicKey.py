import hmac
from hashlib import sha1
import sys


def generateSignaure(staticKey, signKey, channelName, unixTs, randomInt):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature


def generate(staticKey, signKey, channelName, unixTs, randomInt):
    signature = generateSignaure(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt)
    ret = str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt))
    return ret
