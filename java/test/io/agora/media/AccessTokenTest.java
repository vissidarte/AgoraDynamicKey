package io.agora.media;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class AccessTokenTest {
    private String appId = "970CA35de60c44645bbae8a215061b33";
    private String appCertificate = "5CFd2fd1755d40ecb72977518be15d3b";
    private String channelName = "7d72365eb983485397e3e3f9d460bdda";
    private String uid = "2882341273";
    private int ts = 1111111;
    private int salt = 1;
    private int expiredTs = 1446455471;

    @Test
    public void testGenerateDynamicKey() throws Exception {
        String expected = "006970CA35de60c44645bbae8a215061b33IACV0fZUBw+72cVoL9eyGGh3Q6Poi8bgjwVLnyKSJyOXR7dIfRBXoFHlEAABAAAAR/QQAAEAAQCvKDdW";
        AccessToken token = new AccessToken(appId, appCertificate, channelName, uid, ts, salt);
        token.addPrivilege(AccessToken.Privileges.kJoinChannel, expiredTs);
        String result = token.build();
        assertEquals(expected, result);
    }
}
