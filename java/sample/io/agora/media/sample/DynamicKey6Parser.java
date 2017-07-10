package io.agora.media.sample;

import io.agora.media.DynamicKey6;

import java.util.Map;

/**
 * Created by liwei on 7/9/17.
 */
public class DynamicKey6Parser {

    static String toString(Map<Short, String> c, String separator)
    {
        String oss = "";
        String s = "";
        for (Map.Entry<Short, String> v : c.entrySet()) {
            oss += s + v.getKey() + ":" + v.getValue();
            s = separator;
        }

        return oss;
    }
    public static void main(String[] args) throws Exception {
        String key = "006970ca35de60c44645bbae8a215061b33BAAoADJERDA3QThENTE2NzJGNjQwMzY5NTFBNzE0QkI5NTc0N0Q1QjZGQjOZCc2rsCg3VvW7gwOvKDdWAQABAAEAMw==";
        DynamicKey6 k6 = new DynamicKey6();
        if (! k6.fromString(key)) {
            System.out.println("Cannot parse DynamicKey6");
            return ;
        }


        System.out.println("App ID:     " + k6.appID);
        System.out.println("UID:        " + k6.uid);
        System.out.println("Timestamp:  " + k6.unixTs);
        System.out.println("Expired:    " + k6.expiredTs);
        System.out.println("Extra:      [" + toString(k6.extra, ", ") + "]");
        System.out.println("Salt:       " + k6.salt);
        System.out.println("Signature:  " + k6.signature);


    }
}
