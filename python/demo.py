from DynamicKey import generateDynamicKey
from DynamicKey3 import generateDynamicKey3
from DynamicKey4 import generateDynamicKey4
import sys


def main():
    if len(sys.argv) != 8:
        statickey = "970ca35de60c44645bbae84215061b33b"
        signkey = "5cfd2fd1755d40ecb72977518be15d3b"
        channelname = "7d72365eb983485397e3e3f9d460bdda"
        unixts = 1446455472
        uid = 999
        randomint = 58964981
        expiredts = 1446455471
    else:
        statickey = sys.argv[1]
        signkey = sys.argv[2]
        channelname = sys.argv[3]
        unixts = sys.argv[4]
        randomint = sys.argv[5]
        uid = sys.argv[6]
        expiredts = sys.argv[7]
    print """    Usage: ./dynamicKey.exe staticKey signKey channelName unixTs randomInt
Input parameters:
    staticKey: %s
    signKey:%s
    channelName:%s
    unixTs:%s
    randomint:%s
    uid:%s
    expiredTs:%s
    """ % (statickey, signkey, channelname, unixts,  randomint, uid, expiredts)
    print "generateDynamicKey:"
    print generateDynamicKey(statickey, signkey, channelname, unixts, randomint)
    print "generateDynamicKey3:"
    print generateDynamicKey3(statickey, signkey, channelname, unixts, randomint, uid, expiredts)
    print "generateDynamicKey4:"
    print generateDynamicKey4(statickey, signkey, channelname, unixts, randomint, uid, expiredts, 0)
    print "generateRecordTicket:"
    print generateDynamicKey4(statickey, signkey, channelname, unixts, randomint, uid, expiredts, 1)


if __name__ == "__main__":
    main()
