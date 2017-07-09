package io.agora.media;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by liwei on 7/9/17.
 */
public class DynamicKey6Test {
    private String appID = "970ca35de60c44645bbae8a215061b33";
    private String appCertificate = "5cfd2fd1755d40ecb72977518be15d3b";
    private String channel = "7d72365eb983485397e3e3f9d460bdda";
    private int ts = 1446455472;
    private int r = 58964981;
    private long uid = 2882341273L;
    private int expiredTs=1446455471;

    @Test
    public void testGeneratePublicSharingKeyFull() throws Exception {
        String expected = "006970ca35de60c44645bbae8a215061b33AwAoADc0QTk5RTVEQjI4MDk0NUI0NzUwNTk0MUFDMjM4MDU2NzIwREY3QjCZCc2rsCg3VvW7gwOvKDdWAAA=";
        String result = DynamicKey6.generatePublicSharingKey(appID, appCertificate, channel, ts, r, uid, expiredTs);
        assertEquals(expected, result);
    }

    @Test
    public void testGenerateRecordingKeyFull() throws Exception {
        String expected = "006970ca35de60c44645bbae8a215061b33AgAoADkyOUM5RTQ2MTg3QTAyMkJBQUIyNkI3QkYwMTg0MzhDNjc1Q0ZFMUGZCc2rsCg3VvW7gwOvKDdWAAA=";
        String result = DynamicKey6.generateRecordingKey(appID, appCertificate, channel, ts, r, uid, expiredTs);
        assertEquals(expected, result);
    }

    DynamicKey6 testDynamicKey(short service, String result) throws Exception
    {
        DynamicKey6 k6 = new DynamicKey6();
        boolean parsed = k6.fromString(result);
        assertTrue(parsed);

        assertEquals(k6.appID, appID);

        String signature = DynamicKey6.generateSignature(
            appCertificate
            , service
            , k6.appID
            , k6.unixTs
            , k6.salt
            , channel
            , k6.uid
            , k6.expiredTs
            , k6.extra);
        assertEquals(k6.signature, signature);
        return k6;
    }

    @Test
    public void testMediaChannelKey() throws Exception {
        String result = DynamicKey6.generateMediaChannelKey(appID, appCertificate, channel);
        DynamicKey6 k6 = testDynamicKey(DynamicKey6.MEDIA_CHANNEL_SERVICE, result);
        assertEquals(k6.uid, 0);
        assertTrue(k6.unixTs <= DynamicKey6.getTimestamp());
        assertNotEquals(k6.salt, 0);
        assertEquals(k6.expiredTs, 0);
    }

    @Test
    public void testMediaChannelKeyUid() throws Exception {
        String result = DynamicKey6.generateMediaChannelKey(appID, appCertificate, channel, uid);
        DynamicKey6 k6 = testDynamicKey(DynamicKey6.MEDIA_CHANNEL_SERVICE, result);
        assertEquals(k6.uid, uid);
        assertTrue(k6.unixTs <= DynamicKey6.getTimestamp());
        assertNotEquals(k6.salt, 0);
        assertEquals(k6.expiredTs, 0);
    }

    @Test
    public void testMediaChannelKeyUidKick() throws Exception {
        String result = DynamicKey6.generateMediaChannelKey(appID, appCertificate, channel, uid, expiredTs);
        DynamicKey6 k6 = testDynamicKey(DynamicKey6.MEDIA_CHANNEL_SERVICE, result);
        assertEquals(k6.uid, uid);
        assertTrue(k6.unixTs <= DynamicKey6.getTimestamp());
        assertNotEquals(k6.salt, 0);
        assertEquals(k6.expiredTs, expiredTs);
    }

    @Test
    public void testGenerateMediaChannelKeyFull() throws Exception {
        String expected = "006970ca35de60c44645bbae8a215061b33AQAoAEJERTJDRDdFNkZDNkU0ODYxNkYxQTYwOUVFNTM1M0U5ODNCQjFDNDSZCc2rsCg3VvW7gwOvKDdWAAA=";
        String result = DynamicKey6.generateMediaChannelKey(appID, appCertificate, channel, ts, r, uid, expiredTs);
        assertEquals(expected, result);
    }

    @Test
    public void testInChannelPermission() throws Exception {
        String result = DynamicKey6.generateInChannelPermissionKey(
            appID
            , appCertificate
            , channel
            , uid
            , DynamicKey6.noUpload);
        DynamicKey6 k6 = testDynamicKey(DynamicKey6.IN_CHANNEL_PERMISSION, result);
        assertEquals(k6.uid, uid);
        assertTrue(k6.unixTs <=DynamicKey6.getTimestamp());
        assertNotEquals(k6.salt, 0);
        assertEquals(k6.expiredTs, 0);
        assertFalse(k6.extra.isEmpty());
        assertTrue(k6.extra.containsKey(DynamicKey6.ALLOW_UPLOAD_IN_CHANNEL));
        assertEquals(k6.extra.get(DynamicKey6.ALLOW_UPLOAD_IN_CHANNEL), DynamicKey6.noUpload);

        result = DynamicKey6.generateInChannelPermissionKey(
                appID
                , appCertificate
                , channel
                , uid
                , DynamicKey6.audioVideoUpload);
        k6 = testDynamicKey(DynamicKey6.IN_CHANNEL_PERMISSION, result);
        assertEquals(k6.uid, uid);
        assertTrue(k6.unixTs <=DynamicKey6.getTimestamp());
        assertNotEquals(k6.salt, 0);
        assertEquals(k6.expiredTs, 0);
        assertFalse(k6.extra.isEmpty());
        assertTrue(k6.extra.containsKey(DynamicKey6.ALLOW_UPLOAD_IN_CHANNEL));
        assertEquals(k6.extra.get(DynamicKey6.ALLOW_UPLOAD_IN_CHANNEL), DynamicKey6.audioVideoUpload);
    }

    @Test
    public void testInChannelPermissionFull() throws Exception {
        String noUpload = "006970ca35de60c44645bbae8a215061b33BAAoADgyNEQxNDE4M0FGRDkyOEQ4REFFMUU1OTg5NTg2MzA3MTEyNjRGNzSZCc2rsCg3VvW7gwOvKDdWAQABAAEAMA==";
        String generatedNoUpload = DynamicKey6.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.noUpload);
        assertEquals(noUpload, generatedNoUpload);

        String audioVideoUpload = "006970ca35de60c44645bbae8a215061b33BAAoADJERDA3QThENTE2NzJGNjQwMzY5NTFBNzE0QkI5NTc0N0Q1QjZGQjOZCc2rsCg3VvW7gwOvKDdWAQABAAEAMw==";
        String generatedAudioVideoUpload = DynamicKey6.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.audioVideoUpload);
        assertEquals(audioVideoUpload, generatedAudioVideoUpload);
    }

    @Test
    public void testRawAppId() throws Exception
    {
        String noUpload = DynamicKey6.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.noUpload);
        assertNotEquals(-1, noUpload.indexOf(appID));
    }

}
