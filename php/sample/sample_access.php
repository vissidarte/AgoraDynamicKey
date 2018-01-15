<?php
require "../src/AccessToken.php";

$appID = '970CA35de60c44645bbae8a215061b33';
$appCertificate = '5CFd2fd1755d40ecb72977518be15d3b';
$channelName = "7d72365eb983485397e3e3f9d460bdda";
$ts = 1446455472;
$randomInt = 1;
$uid = 2882341273;
$ts = 1111111;
$expiredTs = 1446455471;


$builder = new AccessToken($appID, $appCertificate, $channelName, $uid);
$builder->setPriviledge($Privileges["kJoinChannel"], $expiredTs);
$builder->salt = $randomInt;
$builder->ts = $ts;
echo $builder->build();

?>