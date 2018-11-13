import javax.crypto.*;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.SecretKeySpec;
import java.security.*;
import java.security.spec.InvalidKeySpecException;
import java.util.Base64;

public class Main {

    private static String key = "12345678";
    private static String key2 = "1234567812345678";
    private static String val = "what's your name?";

    public static void des() {
        try {
            DESKeySpec keySpec = new DESKeySpec(key.getBytes());
            SecretKeyFactory keyFac = SecretKeyFactory.getInstance("DES");
            SecretKey secretKey = keyFac.generateSecret(keySpec);

            Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey, new SecureRandom());
            byte[] cipherData = cipher.doFinal(val.getBytes());

            System.out.println(Base64.getEncoder().encodeToString(cipherData));

            cipher.init(Cipher.DECRYPT_MODE, secretKey, new SecureRandom());
            System.out.println(new String(cipher.doFinal(cipherData)));

        } catch (InvalidKeySpecException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        }
    }

    public static void aes() {
        try {
            SecretKeySpec keySpec = new SecretKeySpec(key2.getBytes(), "AES");

            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, keySpec, new SecureRandom());
            byte[] cipherData = cipher.doFinal(val.getBytes());

            System.out.println(Base64.getEncoder().encodeToString(cipherData));

            cipher.init(Cipher.DECRYPT_MODE, keySpec, new SecureRandom());
            System.out.println(new String(cipher.doFinal(cipherData)));

        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        }
    }

    public static void rsa() {
        try {
            KeyPairGenerator keyPairGen = KeyPairGenerator.getInstance("rsa");
            keyPairGen.initialize(1024);
            KeyPair keyPair = keyPairGen.generateKeyPair();

            PublicKey pub = keyPair.getPublic();
            PrivateKey pri = keyPair.getPrivate();

            Cipher cipher = Cipher.getInstance("rsa");
            cipher.init(Cipher.ENCRYPT_MODE, pri, new SecureRandom());
            byte[] cipherData = cipher.doFinal(val.getBytes());

            Signature sign = Signature.getInstance("SHA256withRSA");
            sign.initSign(pri);
            sign.update(cipherData);
            byte[] signData = sign.sign();

            System.out.println(Base64.getEncoder().encodeToString(cipherData));
            System.out.println(Base64.getEncoder().encodeToString(signData));

            cipher.init(Cipher.DECRYPT_MODE, pub, new SecureRandom());
            byte[] plainData = cipher.doFinal(cipherData);

            sign.initVerify(pub);
            sign.update(cipherData);

            System.out.println(sign.verify(signData));
            System.out.println(new String(plainData));


        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        } catch (SignatureException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        des();
        aes();
        rsa();
        System.out.println();
    }
}

