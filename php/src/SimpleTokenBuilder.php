<?php

require "AccessToken.php";

$Role = array(
    "kRoleAttendee" => 0,  // for communication
    "kRolePublisher" => 1, // for live broadcast
    "kRoleSubscriber" => 2,  // for live broadcast
    "kRoleAdmin" => 101
);

$attendeePrivileges = array(
    $Privileges["kJoinChannel"] => 0,
    $Privileges["kPublishAudioStream"] => 0,
    $Privileges["kPublishVideoStream"] => 0,
    $Privileges["kPublishDataStream"] => 0
);


$publisherPrivileges = array(
    $Privileges["kJoinChannel"] => 0,
    $Privileges["kPublishAudioStream"] => 0,
    $Privileges["kPublishVideoStream"] => 0,
    $Privileges["kPublishDataStream"] => 0,
    $Privileges["kPublishAudiocdn"] => 0,
    $Privileges["kPublishVideoCdn"] => 0,
    $Privileges["kInvitePublishAudioStream"] => 0,
    $Privileges["kInvitePublishVideoStream"] => 0,
    $Privileges["kInvitePublishDataStream"] => 0
);

$subscriberPrivileges = array(
    $Privileges["kJoinChannel"] => 0,
    $Privileges["kRequestPublishAudioStream"] => 0,
    $Privileges["kRequestPublishVideoStream"] => 0,
    $Privileges["kRequestPublishDataStream"] => 0
);

$adminPrivileges = array(
    $Privileges["kJoinChannel"] => 0,
    $Privileges["kPublishAudioStream"] => 0,
    $Privileges["kPublishVideoStream"] => 0,
    $Privileges["kPublishDataStream"] => 0,
    $Privileges["kAdministrateChannel"] => 0
);

$RolePrivileges = array(
	$Role["kRoleAttendee"] => $attendeePrivileges,
    $Role["kRolePublisher"] => $publisherPrivileges,
    $Role["kRoleSubscriber"] => $subscriberPrivileges,
    $Role["kRoleAdmin"] => $adminPrivileges
);



class SimpleTokenBuilder
{
    private $token;
    public function __construct($appID, $appCertificate, $channelName, $uid){
        $this->token = new AccessToken($appID, $appCertificate, $channelName, $uid);
    }
    public function initPriviledge($role){
        $p = $RolePrivileges[$role];
        foreach($p as $key => $value){
            $this->setPriviledge($key, $value);
        }
    }
    public function setPriviledge($privilege, $ts){
        $this->token->setPriviledge($privilege, $ts);
    }
    public function removePriviledge($privilege){
        $this->token->removePriviledge($privilege);
    }
    public function buildToken(){
        return $this->token->build();
    }
}


?>