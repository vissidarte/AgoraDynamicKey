<?php

$kJoinChannel = 1;
$kPublishAudioStream = 2;
$kPublishVideoStream = 3;
$kPublishDataStream = 4;
$kPublishAudiocdn = 5;
$kPublishVideoCdn = 6;
$kRequestPublishAudioStream = 7;
$kRequestPublishVideoStream = 8;
$kRequestPublishDataStream = 9;
$kInvitePublishAudioStream = 10;
$kInvitePublishVideoStream = 11;
$kInvitePublishDataStream = 12;
$kAdministrateChannel = 101;

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
        $this->ts = $date->getTimestamp();
        $this->salt = rand(0, 100000);
        $this->msgs = array();
    }

    public function setPriviledge($key, $seconds)
    {
        $this->msgs[$key] = $seconds;
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

function generateDynamicKey($appID, $appCertificate, $channelName, $ts, $uid, $randomInt)
{
    $signature = generateSignature($appID, $appCertificate, $channelName, $ts, $uid, $randomInt);
    // $content = packContent($serviceType, $signature, hex2bin($appID), $ts, $randomInt, $expiredTs, $extra);
    // echo bin2hex($content);
    global $version;
    return $version . base64_encode("");
}

function generateSignature($appID, $appCertificate, $channelName, $ts, $uid, $salt)
{
    $msg = new Message($salt, $ts);
    $msg->packContent();

    return "";
    // $rawAppID = hex2bin($appID);
    // $rawAppCertificate = hex2bin($appCertificate);

    // $buffer = pack("S", $serviceType);
    // $buffer .= pack("S", strlen($rawAppID)) . $rawAppID;
    // $buffer .= pack("I", $ts);
    // $buffer .= pack("I", $salt);
    // $buffer .= pack("S", strlen($channelName)) . $channelName;
    // $buffer .= pack("I", $uid);
    // $buffer .= pack("I", $expiredTs);

    // $buffer .= pack("S", count($extra));
    // foreach ($extra as $key => $value) {
    //     $buffer .= pack("S", $key);
    //     $buffer .= pack("S", strlen($value)) . $value;
    // }

    // return strtoupper(hash_hmac('sha1', $buffer, $rawAppCertificate));
}

function packString($value)
{
    return pack("S", strlen($value)) . $value;
}

// function packContent($serviceType, $signature, $appID, $ts, $salt, $expiredTs, $extra)
// {
//     $buffer = pack("S", $serviceType);
//     $buffer .= packString($signature);
//     $buffer .= packString($appID);
//     $buffer .= pack("I", $ts);
//     $buffer .= pack("I", $salt);
//     $buffer .= pack("I", $expiredTs);

//     $buffer .= pack("S", count($extra));
//     foreach ($extra as $key => $value) {
//         $buffer .= pack("S", $key);
//         $buffer .= packString($value);
//     }

//     return $buffer;
// }
