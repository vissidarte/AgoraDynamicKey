package test

import (
	"../src/SignalingToken"
	 "testing"
	 //"fmt"
)

func Test_SignalingToken(t *testing.T) {
	account := "2882341273"
	appID := "970CA35de60c44645bbae8a215061b33"
	appCertificate := "5CFd2fd1755d40ecb72977518be15d3b"
	validTimeInSeconds := uint32(3600*24)

	SignalingToken.GenerateSignalingToken(account, appID, appCertificate, validTimeInSeconds)
	//key := SignalingToken.GenerateSignalingToken(account, appID, appCertificate, validTimeInSeconds)
	//fmt.Println(key)
}
