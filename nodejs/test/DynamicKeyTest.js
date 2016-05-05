
var DynamicKey = require('../src/DynamicKey');
var DynamicKey3 = require('../src/DynamicKey3');
var DynamicKey4 = require('../src/DynamicKey4');
var vendor = "970ca35de60c44645bbae84215061b33b";
var key = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "7d72365eb983485397e3e3f9d460bdda";
var ts = 1446455472;
var r = 58964981;
//var uid=999;
var uid=2882341273;
var expiredTs=1446455471;

exports.DynamicKeyTest = function (test) {
  var expected = "70e0e5b0a137630190bfa475506de9fb56012576970ca35de60c44645bbae84215061b33b14464554720383bbf5";
  var actual = DynamicKey.generate(vendor, key, channel, ts, r);
  test.equal(expected, actual);
  test.done();
};


exports.DynamicKey3Test = function (test) {
  var expected = "00381e1588b09e6e76a0d2e8fd9e5257dbddd9c60a0970ca35de60c44645bbae84215061b33b14464554720383bbf528823412731446455471";
  var actual = DynamicKey3.generate(vendor, key, channel, ts, r, uid, expiredTs);
  test.equal(expected, actual);
  test.done();
};


exports.RecordingKeyTest = function (test) {
  var expected = "0040f6609180e7967c4047f88fcc8c187e77a4d22a4970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
  var actual = DynamicKey4.generateRecordingKey(vendor, key, channel, ts, r, uid, expiredTs);
  test.equal(expected, actual);
  test.done();
};

exports.MediaChannelKeyTest = function (test) {
  var expected = "004b3e0a78e01b5a07a4627b03ff1bdcc4330839c9a970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
  var actual = DynamicKey4.generateMediaChannelKey(vendor, key, channel, ts, r, uid, expiredTs);
  test.equal(expected, actual);
  test.done();
};
