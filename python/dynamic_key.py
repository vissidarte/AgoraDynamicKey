import hmac
from hashlib import sha1
import sys


def generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt):
    key = "\x00" * (32 - len(staticKey)) + staticKey
    content = key +\
        '{:0>10}'.format(unixTs) + \
        "%.8x" % (int(randomInt)) + \
        str(channelName)
    signature = hmac.new(signKey, content, sha1).hexdigest()
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
    print generateDynamicKey("770ca35e60c44645bbae84215061b33b",
                              "5cfd2f1755d40ecb72977518be15d3b",
                              "7d7236eb983485397e3e3f9d460bdda",
                              1446455472,
                              58964981
                              )

if __name__ == "__main__":
    main()
