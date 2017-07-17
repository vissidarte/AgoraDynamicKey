var DynamicKey6 = require('../src/DynamicKey6');

var key = process.argv[2];
var k6 = DynamicKey6.fromString(key);
if (!k6.parsed) {
    console.log("Cannot parse key");
    process.exit();
}

function toString(map, separator)
{
    var oss = "";
    var s = "";
    for (var key in map) {
        oss += s + key + ":" + map[key];
        s = separator;
    }

    return oss;
}

console.log("App ID:     " + k6.appID);
console.log("UID:        " + k6.content.uid);
console.log("Timestamp:  " + k6.content.unixTs);
console.log("Expired:    " + k6.content.expiredTs);
console.log("Extra:      [" + toString(k6.extra, ", ") + "]");
console.log("Salt:       " + k6.content.salt);
console.log("Signature:  " + k6.content.signature);
