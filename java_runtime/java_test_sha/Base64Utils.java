package java_runtime.java_test_sha;


import java.util.Base64;

public class Base64Utils {
    
    static public String encode(byte[] src) {
        return Base64.getEncoder().encodeToString(src);
    }
}
