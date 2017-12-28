var AccessToken = require('../src/AccessToken').AccessToken;
var Priviledges = require('../src/AccessToken').priviledges;
var appID  = "970CA35de60c44645bbae8a215061b33";
var appCertificate     = "5CFd2fd1755d40ecb72977518be15d3b";
var channel = "7d72365eb983485397e3e3f9d460bdda";
var uid = 2882341273;
var salt = 1;
var ts = 1111111;
var expiredTs = 1446455471;

var key = new AccessToken(appID, appCertificate, channel, uid)
key.salt = salt
key.ts = ts
key.messages[Priviledges.kJoinChannel] = expiredTs
var token = key.build();
console.log(token);
