package io.agora.media;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by liwei on 5/4/16.
 */
public class DynamicKey3Test {

    @Test
    public void testGenerate() throws Exception {
        String vendor = "970ca35de60c44645bbae84215061b33b";
        String key = "5cfd2fd1755d40ecb72977518be15d3b";
        String channel = "7d72365eb983485397e3e3f9d460bdda";
        int ts = 1446455472;
        int r = 58964981;
        long uid = 2882341273L;
        int expiredTs = 1446455471;

        String result = DynamicKey3.generate(vendor, key, channel, ts, r, uid, expiredTs);

        String expected = "00381e1588b09e6e76a0d2e8fd9e5257dbddd9c60a0970ca35de60c44645bbae84215061b33b14464554720383bbf528823412731446455471";
        assertEquals(expected, result);
    }
}
