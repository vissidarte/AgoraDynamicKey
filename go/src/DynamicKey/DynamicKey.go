package DynamicKey

import (
	"crypto/hmac"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"strings"
)

func Generate(vendorKey, signKey, channelName string, unixTs, randomInt uint32) (string) {
    return generateDynamicKey(vendorKey, signKey, channelName, unixTs, randomInt)
}

func generateDynamicKey(vendorKey, signKey, channelName string, unixTs, randomInt uint32) (string) {
	unixTsStr := fmt.Sprintf("%010d", unixTs)
	randomIntStr := fmt.Sprintf("%08x", randomInt)
	signature := generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr)
	buffer := strings.Join([]string{signature, vendorKey, unixTsStr, randomIntStr}, "")
	return buffer
}

func generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr string) (string) {
	buffer := strings.Join([]string{vendorKey, unixTsStr, randomIntStr, channelName}, "")
	signature := hmac.New(sha1.New, []byte(signKey))
	signature.Write([]byte(buffer))
	return hex.EncodeToString(signature.Sum(nil))
}
