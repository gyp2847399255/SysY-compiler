import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Stack;

public class RunningStack {
    public static Stack<HashMap<String, Pointer>> pointerStack = new Stack<>();
    public static Stack<HashMap<String, Integer>> integerStack = new Stack<>();
    public static Stack<Integer> returnAddress = new Stack<>();
    public static HashMap<String, Pointer> globalVariable = new HashMap<>();
    public static HashMap<String, String> constString = new HashMap<>();
    public static LinkedList<Integer> input = new LinkedList<>();
    public static Stack<String> lastLabel = new Stack<>();
    public static Stack<String> curLabel = new Stack<>();
    public static HashMap<Integer, Object> params = new HashMap<>();
    public static Integer returnVal;
    public static Integer inputInteger = null;
    static {
        pointerStack.push(new HashMap<>());
        integerStack.push(new HashMap<>());
        returnAddress.push(1 << 30);
        lastLabel.push("");
        curLabel.push("");
    }

    public static void changeLabel(String label) {
        lastLabel.pop();
        lastLabel.push(curLabel.peek());
        curLabel.pop();
        curLabel.push(label);
    }

    public static void readIntegerFromInput() throws InputEmpty {
        if (input.isEmpty()) {
            throw new InputEmpty();
        }
        inputInteger = input.get(0);
        input.remove(0);
    }

    public static void addInput(String string) {
        Integer integer = Integer.parseInt(string);
        input.add(integer);
    }

    public static void addGlobalVariable(ArrayList<String> operands) {
        globalVariable.put(operands.get(0), new Pointer(operands));
    }

    public static void addConstString(ArrayList<String> operands) {
        constString.put(operands.get(0), operands.get(1).replaceAll("\\\\n", "\n"));
    }

    public static String getConstString(String name) {
        return constString.get(name);
    }

    public static void pushParam(String name, int num) throws VariableNotExists {
        Integer a;
        try {
            a = Integer.parseInt(name);
        } catch (NumberFormatException e) {
            a = integerStack.peek().get(name);
        }
        if (a != null) {
            params.put(num, a);
            return;
        }
        Pointer p = pointerStack.peek().get(name);
        if (p == null) {
            p = globalVariable.get(name);
            if (p == null) {
                throw new VariableNotExists(name);
            }
        }
        params.put(num, p);
    }

    public static void loadParam(String name, int num, int line) {
        Object o = params.get(num);
        if (o instanceof Pointer) {
            pointerStack.peek().put(name, (Pointer) o);
        } else {
            integerStack.peek().put(name, (Integer) o);
        }
    }

    public static void funcCall(int line)
            throws VariableNotExists, ParamsNumberUnmatched {
        HashMap<String, Pointer> pointerParams = new HashMap<>();
        HashMap<String, Integer> integerParams = new HashMap<>();
        pointerStack.push(pointerParams);
        integerStack.push(integerParams);
        returnAddress.push(line + 1);
        lastLabel.push("");
        curLabel.push("");
    }

    public static int funcReturn(ArrayList<String> operands, Integer line) throws VariableNotExists {
        returnVal  = getInteger(operands.get(1));
        pointerStack.pop();
        integerStack.pop();
        lastLabel.pop();
        curLabel.pop();
        return returnAddress.pop();
    }

    public static Integer loadWord(String name, Integer offset) throws ArrayOutBound, VariableNotExists {
        Pointer pointer = pointerStack.peek().get(name);
        if (pointer == null) {
            pointer = globalVariable.get(name);
            if (pointer == null) {
                throw new VariableNotExists(name);
            }
        }
        return pointer.getVal(offset);
    }

    public static void storeWord(String name, int offset, int value) throws ArrayOutBound, VariableNotExists {
        Pointer pointer = pointerStack.peek().get(name);
        if (pointer == null) {
            pointer = globalVariable.get(name);
            if (pointer == null) {
                throw new VariableNotExists(name);
            }
        }
        pointer.setVal(offset, value);
    }

    public static void createPointer(String name, int size) {
        Pointer pointer = new Pointer(size);
        pointerStack.peek().put(name, pointer);
    }

    public static Pointer loadPointer(String name, int offset) throws ArrayOutBound, VariableNotExists {
        Pointer pointer = pointerStack.peek().get(name);
        if (pointer == null) {
            pointer = globalVariable.get(name);
            if (pointer == null) {
                throw new VariableNotExists(name);
            }
        }
        Pointer ret = (Pointer) pointer.clone();
        ret.addN(offset);
        return ret;
    }

    public static Integer getInteger(String name) throws VariableNotExists {
        try {
            return Integer.parseInt(name);
        } catch (NumberFormatException e) {
            Integer ret = integerStack.peek().get(name);
            if (ret == null) {
                if (name.equals("-")) {
                    return 19260817;
                }
                throw new VariableNotExists(name);
            }
            return ret;
        }
    }

    public static void setInteger(String name, int value, Integer line) {
        if (name.equals("-")) {
            return;
        }
        integerStack.peek().put(name, value);
        pointerStack.peek().remove(name);
    }

    public static void setPointer(String name, Pointer pointer, Integer line) {
        if (name.equals("-")) {
            return;
        }
        integerStack.peek().remove(name);
        pointerStack.peek().put(name, pointer);
    }
}
