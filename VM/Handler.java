import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;

public class Handler {
    private static final HashMap<String, Method> methodMap;
    static {
        methodMap = new HashMap<>();
        try {
            methodMap.put("assign", Quaternion.class.getMethod("assign", ArrayList.class, Integer.class));
            methodMap.put("multiply", Quaternion.class.getMethod("multiply", ArrayList.class, Integer.class));
            methodMap.put("divide", Quaternion.class.getMethod("divide", ArrayList.class, Integer.class));
            methodMap.put("mod", Quaternion.class.getMethod("mod", ArrayList.class, Integer.class));
            methodMap.put("plus", Quaternion.class.getMethod("plus", ArrayList.class, Integer.class));
            methodMap.put("minus", Quaternion.class.getMethod("minus", ArrayList.class, Integer.class));
            methodMap.put("equal", Quaternion.class.getMethod("equal", ArrayList.class, Integer.class));
            methodMap.put("notEqual", Quaternion.class.getMethod("notEqual", ArrayList.class, Integer.class));
            methodMap.put("greatEqual", Quaternion.class.getMethod("greatEqual", ArrayList.class, Integer.class));
            methodMap.put("greatThan", Quaternion.class.getMethod("greatThan", ArrayList.class, Integer.class));
            methodMap.put("lessEqual", Quaternion.class.getMethod("lessEqual", ArrayList.class, Integer.class));
            methodMap.put("lessThan", Quaternion.class.getMethod("lessThan", ArrayList.class, Integer.class));
            methodMap.put("not", Quaternion.class.getMethod("not", ArrayList.class, Integer.class));
            methodMap.put("createPointer", Quaternion.class.getMethod("createPointer", ArrayList.class, Integer.class));
            methodMap.put("loadWord", Quaternion.class.getMethod("loadWord", ArrayList.class, Integer.class));
            methodMap.put("storeWord", Quaternion.class.getMethod("storeWord", ArrayList.class, Integer.class));
            methodMap.put("loadPointer", Quaternion.class.getMethod("loadPointer", ArrayList.class, Integer.class));
            methodMap.put("negate", Quaternion.class.getMethod("negate", ArrayList.class, Integer.class));
            methodMap.put("funcCall", Quaternion.class.getMethod("funcCall", ArrayList.class, Integer.class));
            methodMap.put("return", Quaternion.class.getMethod("funcRet", ArrayList.class, Integer.class));
            methodMap.put("jump", Quaternion.class.getMethod("jump", ArrayList.class, Integer.class));
            methodMap.put("jumpWhenTrue", Quaternion.class.getMethod("jumpWhenTrue", ArrayList.class, Integer.class));
            methodMap.put("jumpWhenFalse", Quaternion.class.getMethod("jumpWhenFalse", ArrayList.class, Integer.class));
            methodMap.put("printString", Quaternion.class.getMethod("printString", ArrayList.class, Integer.class));
            methodMap.put("printInteger", Quaternion.class.getMethod("printInteger", ArrayList.class, Integer.class));
            methodMap.put("getInt", Quaternion.class.getMethod("getInt", ArrayList.class, Integer.class));
            methodMap.put("phi", Quaternion.class.getMethod("phiNode", ArrayList.class, Integer.class));
            methodMap.put("push", Quaternion.class.getMethod("pushParam", ArrayList.class, Integer.class));
            methodMap.put("pop", Quaternion.class.getMethod("loadParam", ArrayList.class, Integer.class));
            methodMap.put("getReturn", Quaternion.class.getMethod("getReturn", ArrayList.class, Integer.class));
            methodMap.put("readInt", Quaternion.class.getMethod("readInt", ArrayList.class, Integer.class));
        } catch (NoSuchMethodException | SecurityException e) {
            e.printStackTrace();
        }
    }

    public static int handle(ArrayList<String> operands, Integer line) {
        Method method = methodMap.get(operands.get(0));
        if (method == null) {
            System.out.println("UNDEFINED OP " + operands.get(0));
        }
        Integer ret = null;
        try {
            ret = (Integer) method.invoke(Quaternion.class, operands, line);
        } catch (IllegalAccessException | InvocationTargetException | IllegalArgumentException e) {
            e.printStackTrace();
        }
        if (ret != null) {
            return ret;
        }
        return -1;
    }
}
