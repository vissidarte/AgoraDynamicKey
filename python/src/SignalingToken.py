import hashlib

def getToken(account, appID, appCertificate, expiredTs):
    digest_String = account + appID + appCertificate + str(expiredTs)
    m = hashlib.md5()
    m.update(digest_String)
    token = m.hexdigest()
    return "1:" + appID + ":" + str(expiredTs) + ":" + token