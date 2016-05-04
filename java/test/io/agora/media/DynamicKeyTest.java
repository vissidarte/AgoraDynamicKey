package io.agora.media;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by liwei on 5/4/16.
 */
public class DynamicKeyTest {

    @Test
    public void testGenerate() throws Exception {
        String vendor = "970ca35de60c44645bbae84215061b33b";
        String key = "5cfd2fd1755d40ecb72977518be15d3b";
        String channel = "7d72365eb983485397e3e3f9d460bdda";
        int ts = 1446455472;
        int r = 58964981;
        int uid = 999;
        int expiredTs = 1446455471;
        String result = DynamicKey.generate(vendor, key, channel, ts, r);

        String expected = "70e0e5b0a137630190bfa475506de9fb56012576970ca35de60c44645bbae84215061b33b14464554720383bbf5";
        assertEquals(expected, result);
    }
}