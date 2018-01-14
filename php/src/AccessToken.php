<?php


$Privileges = array(
    "kJoinChannel" => 1,
    "kPublishAudioStream" => 2,
    "kPublishVideoStream" => 3,
    "kPublishDataStream" => 4,
    "kPublishAudiocdn" => 5,
    "kPublishVideoCdn" => 6,
    "kRequestPublishAudioStream" => 7,
    "kRequestPublishVideoStream" => 8,
    "kRequestPublishDataStream" => 9,
    "kInvitePublishAudioStream" => 10,
    "kInvitePublishVideoStream" => 11,
    "kInvitePublishDataStream" => 12,
    "kAdministrateChannel" => 101
);

class Message
{
    private $salt;
    private $ts;
    private $msgs;
    public function __construct($salt, $ts, $msgs)
    {
        $this->salt = $salt;
        $this->ts = $ts;
        $this->msgs = $msgs;
    }

    public function packContent()
    {
        $buffer = unpack("C*", pack("I", $this->salt));
        $buffer = array_merge($buffer, unpack("C*", pack("I", $this->ts)));
        $buffer = array_merge($buffer, unpack("C*", pack("S", sizeof($this->msgs))));
        foreach ($this->msgs as $key => $value) {
            $buffer = array_merge($buffer, unpack("C*", pack("S", $key)));
            $buffer = array_merge($buffer, unpack("C*", pack("I", $value)));
        }
        return $buffer;
    }
}

class AccessToken
{
    private $msgs;
    private $appID, $appCertificate, $channelName, $uid;
    public $ts, $salt;
    public function __construct($appID, $appCertificate, $channelName, $uid)
    {
        $this->appID = $appID;
        $this->appCertificate = $appCertificate;
        $this->channelName = $channelName;
        $this->uid = $uid;
        date_default_timezone_set("UTC");
        $date = new DateTime();
        $this->ts = $date->getTimestamp() + 24 * 3600;
        $this->salt = rand(0, 100000);
        $this->msgs = array();
    }

    public function setPriviledge($key, $seconds)
    {
        $this->msgs[$key] = $seconds;
        return $this;
    }

    public function removePriviledge($key, $seconds)
    {
        unset($this->msgs[$key]);
        return $this;
    }

    public function build()
    {
        $msg = (new Message($this->salt, $this->ts, $this->msgs))->packContent();
        $val = array_merge(unpack("C*", $this->appID), unpack("C*", $this->channelName), unpack("C*", $this->uid), $msg);
        $sig = hash_hmac('sha256', implode(array_map("chr", $val)), $this->appCertificate, true);

        $crc_channel_name = crc32($this->channelName) & 0xffffffff;
        $crc_uid = crc32($this->uid) & 0xffffffff;

        $content = array_merge(unpack("C*", packString($sig)), unpack("C*", pack("I", $crc_channel_name)), unpack("C*", pack("I", $crc_uid)), unpack("C*", pack("S", count($msg))), $msg);
        $version = "006";
        $ret = $version . $this->appID . base64_encode(implode(array_map("chr", $content)));
        return $ret;
    }
}

function packString($value)
{
    return pack("S", strlen($value)) . $value;
}
