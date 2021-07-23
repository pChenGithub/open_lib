package java_runtime.java_test_sha;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

public class UrlUtils {
    static public String encode(String source) {
        String ret;
        try {
            ret = URLEncoder.encode(source, "utf-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return null;
        }

        return ret;
    }
}
