package java_runtime.java_test_sha;


public class Test {
    
    public static void main(String[] args) {
        System.out.println("hello word");

        System.out.println(Md5Utils.md5encode("source"));

        System.out.println(Sha1Utils.md5encode("source"));

    }
}
