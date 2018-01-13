var SimpleTokenBuilder = require('../src/SimpleTokenBuilder').SimpleTokenBuilder;
var Role = require('../src/SimpleTokenBuilder').Role;
var Priviledges = require('../src/AccessToken').priviledges;
var appID  = "970CA35de60c44645bbae8a215061b33";
var appCertificate     = "5CFd2fd1755d40ecb72977518be15d3b";
var channel = "7d72365eb983485397e3e3f9d460bdda";
var uid = 2882341273;
var salt = 1;
var ts = 1111111;
var expiredTs = 1446455471;

var builder = new SimpleTokenBuilder(appID, appCertificate, channel, uid);
builder.key.salt = salt;
builder.key.ts = ts;
builder.key.messages[Priviledges.kJoinChannel] = expiredTs;

// console.log(builder.key.messages);
// builder.initPriviliges(Role.kRoleAttendee);
// console.log(builder.key.messages);
// builder.setPrivilege(Priviledges.kJoinChannel, expiredTs);
// console.log(builder.key.messages);
// builder.removePrivilege(Priviledges.kJoinChannel);
// console.log(builder.key.messages);

var token = builder.buildToken();
console.log(token);
