<?php
$version = "005";
$NO_UPLOAD = "0";
$AUDIO_VIDEO_UPLOAD = "3";

// InChannelPermissionKey
$ALLOW_UPLOAD_IN_CHANNEL = 1;

// Service Type
$MEDIA_CHANNEL_SERVICE = 1;
$RECORDING_SERVICE = 2;
$PUBLIC_SHARING_SERVICE = 3;
$IN_CHANNEL_PERMISSION = 4;

    function generateRecordingKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs)
    {
        global $RECORDING_SERVICE;

        return generateDynamicKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $RECORDING_SERVICE, null);
    }

    function generateMediaChannelKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs)
    {
        global $MEDIA_CHANNEL_SERVICE;

        return generateDynamicKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $MEDIA_CHANNEL_SERVICE, null);
    }

    function generateInChannelPermissionKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $permission)
    {
        global $ALLOW_UPLOAD_IN_CHANNEL;
        global $IN_CHANNEL_PERMISSION;

        $extra[$ALLOW_UPLOAD_IN_CHANNEL] = $permission;
        return generateDynamicKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $IN_CHANNEL_PERMISSION, $extra);
    }

    function generateDynamicKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $serviceType, $extra)
    {
        return "";
    }

    function generateSignature($appID, $appCertificate, $channelName, $ts, $randomStr, $uidStr, $expiredStr, $serviceType)
    {
        return "";
    }

?>
