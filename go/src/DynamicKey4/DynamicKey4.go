package DynamicKey4

import (
	"crypto/hmac"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"strings"
)

func GeneratePublicSharingKey(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32) (string) {
    return generateDynamicKey(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "APSS")
}

func GenerateRecordingKey(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32) (string) {
    return generateDynamicKey(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ARS")
}

func GenerateMediaChannelKey(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32) (string) {
    return generateDynamicKey(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, "ACS")
}

func generateDynamicKey(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32, serviceType string) (string) {
    version := "004"
	unixTsStr := fmt.Sprintf("%010d", unixTs)
	randomIntStr := fmt.Sprintf("%08x", randomInt)
	uidStr := fmt.Sprintf("%010d", uid)
	expiredTsStr := fmt.Sprintf("%010d", expiredTs)
	signature := generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr, serviceType)
	buffer := strings.Join([]string{version, signature, vendorKey, unixTsStr, randomIntStr, expiredTsStr}, "")
	return buffer
}

func generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr, serviceType string) (string) {
	buffer := strings.Join([]string{serviceType, vendorKey, unixTsStr, randomIntStr, channelName, uidStr, expiredTsStr}, "")
	signature := hmac.New(sha1.New, []byte(signKey))
	signature.Write([]byte(buffer))
	return hex.EncodeToString(signature.Sum(nil))
}
