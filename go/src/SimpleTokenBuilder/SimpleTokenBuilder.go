package SimpleTokenBuilder

import (
	"../AccessToken"
	"fmt"
	"time"
	"math/rand"
)

type Role uint16
const (
	Role_Attendee = 1
    Role_Publisher = 2
    Role_Subscriber = 3
    Role_Admin = 4
)

var attendeePrivileges = map[uint16]uint32 {
	AccessToken.KJoinChannel: 0, 
	AccessToken.KPublishAudioStream: 0, 
	AccessToken.KPublishVideoStream: 0, 
	AccessToken.KPublishDataStream: 0,
}
var publisherPrivileges = map[uint16]uint32 {
	AccessToken.KJoinChannel: 0, 
	AccessToken.KPublishAudioStream: 0, 
	AccessToken.KPublishVideoStream: 0, 
	AccessToken.KPublishDataStream: 0,
	AccessToken.KPublishAudiocdn: 0,
	AccessToken.KPublishVideoCdn: 0,
	AccessToken.KInvitePublishAudioStream: 0,
	AccessToken.KInvitePublishVideoStream: 0,
	AccessToken.KInvitePublishDataStream: 0,
}

var subscriberPrivileges = map[uint16]uint32 {
	AccessToken.KJoinChannel: 0, 
	AccessToken.KRequestPublishAudioStream: 0, 
	AccessToken.KRequestPublishVideoStream: 0, 
	AccessToken.KRequestPublishDataStream: 0,
}

var adminPrivileges = map[uint16]uint32 {
	AccessToken.KJoinChannel: 0, 
	AccessToken.KPublishAudioStream: 0, 
	AccessToken.KPublishVideoStream: 0, 
	AccessToken.KPublishDataStream: 0,
	AccessToken.KAdministrateChannel: 0,
}

var RolePrivileges = map[uint16](map[uint16]uint32) {
	Role_Attendee: attendeePrivileges, 
	Role_Publisher: publisherPrivileges, 
	Role_Subscriber: subscriberPrivileges, 
	Role_Admin: adminPrivileges,
}


type SimpleTokenBuilder struct {
	Token AccessToken.AccessToken
}

func random(min int, max int) int {
	rand.Seed(time.Now().UnixNano())
    return rand.Intn(max-min) + min
}

func CreateSimpleTokenBuilder(appID, appCertificate, channelName string, uid uint32) SimpleTokenBuilder {
	uidStr := fmt.Sprintf("%d", uid)
	ts := uint32(time.Now().Unix()) + 24 * 3600
	salt := uint32(random(1, 99999999))
	message := make(map[uint16]uint32)
	token := AccessToken.AccessToken{appID, appCertificate, channelName, uidStr, ts, salt, message}
    return SimpleTokenBuilder{token}
}

func InitPriviliges(builder *SimpleTokenBuilder, role Role) {
	rolepri := uint16(role)
	builder.Token.Message = make(map[uint16]uint32)
	for key, value := range RolePrivileges[rolepri] {
		builder.Token.Message[key] = value
	}
}

func (builder SimpleTokenBuilder) SetPrivilege(privilege AccessToken.Privileges, timeoutFromNow uint32) {
	pri := uint16(privilege)
	builder.Token.Message[pri] = timeoutFromNow
}

func (builder SimpleTokenBuilder) RemovePrivilege(privilege AccessToken.Privileges) {
	pri := uint16(privilege)
	delete(builder.Token.Message, pri)
}

func (builder SimpleTokenBuilder) BuildToken() (string,error) {
	return builder.Token.Build()
}



