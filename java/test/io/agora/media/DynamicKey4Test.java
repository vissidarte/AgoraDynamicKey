package io.agora.media;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by liwei on 5/4/16.
 */
public class DynamicKey4Test {

    @Test
    public void testGenerateRecordingKey() throws Exception {
        String vendor = "970ca35de60c44645bbae84215061b33b";
        String key = "5cfd2fd1755d40ecb72977518be15d3b";
        String channel = "7d72365eb983485397e3e3f9d460bdda";
        int ts = 1446455472;
        int r = 58964981;
        long uid = 2882341273L;
        int expiredTs=1446455471;

        String expected = "0040f6609180e7967c4047f88fcc8c187e77a4d22a4970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
        String result = DynamicKey4.generateRecordingKey(vendor, key, channel, ts, r, uid, expiredTs);
        assertEquals(expected, result);
    }

    @Test
    public void testGenerateMediaChannelKey() throws Exception {
        String vendor = "970ca35de60c44645bbae84215061b33b";
        String key = "5cfd2fd1755d40ecb72977518be15d3b";
        String channel = "7d72365eb983485397e3e3f9d460bdda";
        int ts = 1446455472;
        int r = 58964981;
        long uid = 2882341273L;
        int expiredTs=1446455471;

        String expected = "004b3e0a78e01b5a07a4627b03ff1bdcc4330839c9a970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
        String result = DynamicKey4.generateMediaChannelKey(vendor, key, channel, ts, r, uid, expiredTs);
        assertEquals(expected, result);
    }
}