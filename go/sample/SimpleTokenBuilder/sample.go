package main

import (
	"../../src/SimpleTokenBuilder"
	"../../src/AccessToken"
    "fmt"
)

func main() {

	appID := "970CA35de60c44645bbae8a215061b33"
	appCertificate := "5CFd2fd1755d40ecb72977518be15d3b"
	channelName := "7d72365eb983485397e3e3f9d460bdda"
	uid := uint32(2882341273)
	expiredTs := uint32(1446455471)

	builder := SimpleTokenBuilder.CreateSimpleTokenBuilder(appID, appCertificate, channelName, uid)
	builder.Token.Salt = uint32(1)
	builder.Token.Ts = uint32(1111111)
	
	builder.Token.Message[AccessToken.KJoinChannel] = expiredTs
	fmt.Println(builder.Token.Message)
	builder.InitPriviliges(SimpleTokenBuilder.Role_Attendee)
	fmt.Println(builder.Token.Message)
	builder.SetPrivilege(AccessToken.KJoinChannel, expiredTs)
	fmt.Println(builder.Token.Message)
	builder.RemovePrivilege(AccessToken.KPublishAudioStream)
	builder.RemovePrivilege(AccessToken.KPublishVideoStream)
	builder.RemovePrivilege(AccessToken.KPublishDataStream)
	fmt.Println(builder.Token.Message)

	if result, err := builder.BuildToken(); err != nil {
		fmt.Println(err)
	} else {
		fmt.Println(result)
	}
}

