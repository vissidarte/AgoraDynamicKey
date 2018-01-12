package SimpleTokenBuilder

import (
	"../AccessToken"
	"fmt"
	"time"
	"math/rand"
)

//type Role uint16
const (
	ROLE_ATTENDEE = 1
    ROLE_PUBLISHER = 2
    ROLE_SUBSCRIBER = 3
    ROLE_ADMIN = 4
)

var attendeePrivileges = map[uint16]uint32 {
	AccessToken.PRI_JOIN_CHANNEL: 0, 
	AccessToken.PRI_PUBLISH_AUDIO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_VIDEO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_DATA_STREAM: 0,
}
var publisherPrivileges = map[uint16]uint32 {
	AccessToken.PRI_JOIN_CHANNEL: 0, 
	AccessToken.PRI_PUBLISH_AUDIO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_VIDEO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_DATA_STREAM: 0,
	AccessToken.PRI_PUBLISH_AUDIO_CDN: 0,
	AccessToken.PRI_PUBLISH_VIDEO_CDN: 0,
	AccessToken.PRI_INVITE_PUBLISH_AUDIO_STREAM: 0,
	AccessToken.PRI_INVITE_PUBLISH_VIDEO_STREAM: 0,
	AccessToken.PRI_INVITE_PUBLISH_DATA_STREAM: 0,
}

var subscriberPrivileges = map[uint16]uint32 {
	AccessToken.PRI_JOIN_CHANNEL: 0, 
	AccessToken.PRI_REQUEST_PUBLISH_AUDIO_STREAM: 0, 
	AccessToken.PRI_REQUEST_PUBLISH_VIDEO_STREAM: 0, 
	AccessToken.PRI_REQUEST_PUBLISH_DATA_STREAM: 0,
}

var adminPrivileges = map[uint16]uint32 {
	AccessToken.PRI_JOIN_CHANNEL: 0, 
	AccessToken.PRI_PUBLISH_AUDIO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_VIDEO_STREAM: 0, 
	AccessToken.PRI_PUBLISH_DATA_STREAM: 0,
	AccessToken.PRI_ADMINISTRATE_CHANNEL: 0,
}

var RolePrivileges = map[uint16](map[uint16]uint32) {
	ROLE_ATTENDEE: attendeePrivileges, 
	ROLE_PUBLISHER: publisherPrivileges, 
	ROLE_SUBSCRIBER: subscriberPrivileges, 
	ROLE_ADMIN: adminPrivileges,
}


type SimpleTokenBuilder struct {
	Token AccessToken.AccessToken
}

func random(min int, max int) int {
	rand.Seed(time.Now().UnixNano())
    return rand.Intn(max-min) + min
}

func NewSimpleTokenBuilder(appID, appCertificate, channelName string, uid uint32) SimpleTokenBuilder {
	uidStr := fmt.Sprintf("%d", uid)
	ts := uint32(time.Now().Unix()) + 24 * 3600
	salt := uint32(random(1, 99999999))
	message := make(map[uint16]uint32)
	token := AccessToken.AccessToken{appID, appCertificate, channelName, uidStr, ts, salt, message}
    return SimpleTokenBuilder{token}
}

func InitPriviliges(builder *SimpleTokenBuilder, role uint16) {
	builder.Token.Message = make(map[uint16]uint32)
	for key, value := range RolePrivileges[role] {
		builder.Token.Message[key] = value
	}
}

func SetPrivilege(builder *SimpleTokenBuilder, privilege uint16, timeoutFromNow uint32) {
	builder.Token.Message[privilege] = timeoutFromNow
}

func RemovePrivilege(builder *SimpleTokenBuilder, privilege uint16) {
	delete(builder.Token.Message, privilege);
}

func (builder SimpleTokenBuilder) BuildToken() (string,error) {
	return builder.Token.Build()
}



