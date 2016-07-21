<?php

    function generateRecordingKey($staticKey, $signKey, $channelName, $ts, $randomInt, $uid, $expiredTs ,$serviceType='ARS')
    {
        return generateDynamicKey($staticKey, $signKey, $channelName, $ts, $randomInt, $uid, $expiredTs ,$serviceType);
    }

    function generateMediaChannelKey($staticKey, $signKey, $channelName, $ts, $randomInt, $uid, $expiredTs ,$serviceType='ACS')
    {
        return generateDynamicKey($staticKey, $signKey, $channelName, $ts, $randomInt, $uid, $expiredTs ,$serviceType);
    }

    function generateDynamicKey($staticKey, $signKey, $channelName, $ts, $randomInt, $uid, $expiredTs ,$serviceType)
    {
        $version = "004";

        $randomStr = "00000000" . dechex($randomInt);
        $randomStr = substr($randomStr,-8);

        $uidStr = "0000000000" . $uid;
	    $uidStr = substr($uidStr,-10);
        
        $expiredStr = "0000000000" . $expiredTs;
        $expiredStr = substr($expiredStr,-10);

        $signature = generateSignature($staticKey, $signKey, $channelName, $ts, $randomStr, $uidStr, $expiredStr ,$serviceType);

        return $version . $signature . $staticKey . $ts . $randomStr . $expiredStr;
    }

    function generateSignature($staticKey, $signKey, $channelName, $ts, $randomStr, $uidStr, $expiredStr ,$serviceType)
    {
        $concat = $serviceType . $staticKey . $ts . $randomStr . $channelName . $uidStr . $expiredStr;
        return hash_hmac('sha1', $concat, $signKey);
    }
?>