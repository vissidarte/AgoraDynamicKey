import hashlib
import time

def generateSignalingToken(
        account,
        appID,
        appCertificate,
        validTimeInSeconds
        ):
	version = "1"
	expired = str( int(time.time()) + validTimeInSeconds)
	content = account + appID + appCertificate + expired
	md5sum  = hashlib.md5(content).hexdigest()
	result  = "%s:%s:%s:%s"%(version, appID, expired, md5sum)
	return result
