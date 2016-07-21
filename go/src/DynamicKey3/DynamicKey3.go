package DynamicKey3

import (
	"crypto/hmac"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"strings"
)

func Generate(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32) (string) {
    return generateDynamicKey(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs)
}

func generateDynamicKey(vendorKey, signKey, channelName string, unixTs, randomInt, uid, expiredTs uint32) (string) {
    version := "003"
	unixTsStr := fmt.Sprintf("%010d", unixTs)
	randomIntStr := fmt.Sprintf("%08x", randomInt)
	uidStr := fmt.Sprintf("%010d", uid)
	expiredTsStr := fmt.Sprintf("%010d", expiredTs)
	signature := generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr)
	buffer := strings.Join([]string{version, signature, vendorKey, unixTsStr, randomIntStr, uidStr, expiredTsStr}, "")
	return buffer
}

func generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr string) (string) {
	buffer := strings.Join([]string{vendorKey, unixTsStr, randomIntStr, channelName, uidStr, expiredTsStr}, "")
	signature := hmac.New(sha1.New, []byte(signKey))
	signature.Write([]byte(buffer))
	return hex.EncodeToString(signature.Sum(nil))
}
