import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;

public class JarLoader {

    public static void load(String jarPath)
            throws MalformedURLException, NoSuchMethodException, InvocationTargetException,
            IllegalAccessException, ClassNotFoundException, InstantiationException {
        File jarFile = new File(jarPath);
        URL url = jarFile.toURI().toURL();

        if (null != url) {
            Method method = null;
            method = URLClassLoader.class.getDeclaredMethod("addURL", URL.class);

            boolean accessible = method.isAccessible();
            method.setAccessible(true);

            URLClassLoader classLoader = new URLClassLoader(
                    new URL[]{url}, Thread.currentThread().getContextClassLoader());
            method.invoke(classLoader, url);
            method.setAccessible(accessible);

            Class clazz = classLoader.loadClass("coomy.A");
            if (null != clazz) {
                Object instance = clazz.getDeclaredConstructor().newInstance();
                Method m = clazz.getDeclaredMethod("getValue");
                Integer ret = (Integer) m.invoke(instance);
                System.out.println(ret);
            }

        }
    }
}
