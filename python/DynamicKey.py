import hmac
from hashlib import sha1
import sys


def generateSignaure2(
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


def generateSignaure(staticKey, signKey, channelName, unixTs, randomInt):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName)
    signature = hmac.new(signKey, content, sha1).hexdigest()
    return signature

def generateDynamicKey3(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    signature = generateSignaure2(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs)
    version= '{0:0>3}'.format(3)
    ret = version+str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    return ret


def generateDynamicKey2(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    signature = generateSignaure2(
        staticKey,
        signKey,
        channelName,
        unixTs,
        randomInt,
        uid,
        expiredTs)
    ret = str(signature) + \
        staticKey + \
        '{0:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        '{:0>10}'.format(uid) + \
        '{:0>10}'.format(expiredTs)
    return ret


def generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt):
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


def main():
    # print constructDynamicKey(
        # str(sys.argv[1]),
        # str(sys.argv[2]),
        # str(sys.argv[3]),
        # sys.argv[4],
        # sys.argv[5]
    # )
    print generateDynamicKey("970ca35de60c44645bbae84215061b33b",
                             "5cfd2fd1755d40ecb72977518be15d3b",
                             "7d72365eb983485397e3e3f9d460bdda",
                             1446455472,
                             58964981
                             )
    print generateDynamicKey2("970ca35de60c44645bbae84215061b33b",
                              "5cfd2fd1755d40ecb72977518be15d3b",
                              "7d72365eb983485397e3e3f9d460bdda",
                              1446455472,
                              58964981,
                              999,
                              1446455471
                              )
    print generateDynamicKey3("970ca35de60c44645bbae84215061b33b",
                              "5cfd2fd1755d40ecb72977518be15d3b",
                              "7d72365eb983485397e3e3f9d460bdda",
                              1446455472,
                              58964981,
                              999,
                              1446455471
                              )



if __name__ == "__main__":
    main()
