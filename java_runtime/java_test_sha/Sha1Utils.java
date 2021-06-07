package java_runtime.java_test_sha;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Sha1Utils {
    static public String md5encode(String source) {
        MessageDigest md;
        try {
            md = MessageDigest.getInstance("SHA");
        } catch (NoSuchAlgorithmException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        }
        md.update(source.getBytes());
        return new String(md.digest());
    }
}
