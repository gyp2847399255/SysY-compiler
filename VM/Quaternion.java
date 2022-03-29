import java.util.ArrayList;

public class Quaternion {
    // assign assigned integer
    public static void assign(ArrayList<String> operands, Integer line) {
        int a;
        try {
            a = RunningStack.getInteger(operands.get(2));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a, line);
    }

    // multiply assigned multiply1, multiply2
    public static void multiply(ArrayList<String> operands, Integer line){
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a * b, line);
    }

    // divide assigned divide1, divide2
    public static void divide(ArrayList<String> operands, Integer line){
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a / b, line);
    }

    // mod assigned mod1, mod2
    public static void mod(ArrayList<String> operands, Integer line){
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a % b, line);
    }

    // plus assigned plus1 plus2
    public static void plus(ArrayList<String> operands, Integer line) {
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a + b, line);
    }

    // minus assigned integer integer
    public static void minus(ArrayList<String> operands, Integer line) {
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a - b, line);
    }

    // eq assigned integer integer
    public static void equal(ArrayList<String> operands, Integer line) {
        Integer a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a.equals(b) ? 1 : 0, line);
    }

    // notEqual assigned integer integer
    public static void notEqual(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a != b ? 1 : 0, line);
    }

    // ge assigned integer integer
    public static void greatEqual(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a >= b ? 1 : 0, line);
    }

    public static void greatThan(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a > b ? 1 : 0, line);
    }

    public static void lessEqual(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a <= b ? 1 : 0, line);
    }

    public static void lessThan(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = RunningStack.getInteger(operands.get(3));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a < b ? 1 : 0, line);
    }

    // negate assigned negate1
    public static void negate(ArrayList<String> operands, Integer line) {
        int a;
        try {
            a = RunningStack.getInteger(operands.get(2));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), -a, line);
    }

    // not assigned not1
    public static void not(ArrayList<String> operands, Integer line) {
        int a;
        try {
            a = RunningStack.getInteger(operands.get(2));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a == 0 ? 1 : 0, line);
    }

    public static void createPointer(ArrayList<String> operands, Integer line) {
        RunningStack.createPointer(operands.get(1), Integer.parseInt(operands.get(2)));
    }

    // loadWord assigned pointer offset
    public static void loadWord(ArrayList<String> operands, Integer line) {
        int a, offset;
        try {
            offset = RunningStack.getInteger(operands.get(3));
            a = RunningStack.loadWord(operands.get(2), offset);
        } catch (MyException e) {
            e.print(line);
            return;
        }
        RunningStack.setInteger(operands.get(1), a, line);
    }

    // storeWord integer pointer offset
    public static void storeWord(ArrayList<String> operands, Integer line) {
        try {
            int a = RunningStack.getInteger(operands.get(1));
            int offset = RunningStack.getInteger(operands.get(3));
            RunningStack.storeWord(operands.get(2), offset, a);
        } catch (MyException e) {
            e.print(line);
        }
    }

    // loadPointer assigned pointer offset
    public static void loadPointer(ArrayList<String> operands, Integer line) {
        Pointer pointer;
        int offset;
        try {
            offset = RunningStack.getInteger(operands.get(3));
            pointer = RunningStack.loadPointer(operands.get(2), offset);
        } catch (MyException e) {
            e.print(line);
            return;
        }
        RunningStack.setPointer(operands.get(1), pointer, line);
    }

    public static void pushParam(ArrayList<String> operands, Integer line) {
        try {
            RunningStack.pushParam(operands.get(1), Integer.parseInt(operands.get(2)));
        } catch (VariableNotExists variableNotExists) {
            variableNotExists.print(line);
        }
    }

    public static void loadParam(ArrayList<String> operands, Integer line) {
        RunningStack.loadParam(operands.get(1), Integer.parseInt(operands.get(2)), line);
    }

    // call label
    public static int funcCall(ArrayList<String> operands, Integer line) {
        ArrayList<String> arrayList = new ArrayList<>();
        arrayList.add("jump");
        arrayList.add(operands.get(1));
        int ret = jump(arrayList, line);
        try {
            RunningStack.funcCall(line);
        } catch (MyException e) {
            e.print(line);
            return -1;
        }
        return ret;
    }

    // return res
    public static int funcRet(ArrayList<String> operands, Integer line) {
        try {
            return RunningStack.funcReturn(operands, line);
        } catch (VariableNotExists variableNotExists) {
            variableNotExists.print(line);
            return 1 << 30;
        }
    }

    // jump label
    public static int jump(ArrayList<String> operands, Integer line) {
        int a;
        try {
            a = Labels.getLabel(operands.get(1));
        } catch (MyException e) {
            e.print(line);
            return 1 << 30;
        }
        return a;
    }

    // jumpWhenTrue label condition
    public static int jumpWhenTrue(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = Labels.getLabel(operands.get(1));
        } catch (MyException e) {
            e.print(line);
            return 1 << 30;
        }
        if (a != 0) {
            return b;
        }
        return line + 1;
    }

    // jumpWhenFalse label condition
    public static int jumpWhenFalse(ArrayList<String> operands, Integer line) {
        int a, b;
        try {
            a = RunningStack.getInteger(operands.get(2));
            b = Labels.getLabel(operands.get(1));
        } catch (MyException e) {
            e.print(line);
            return 1 << 30;
        }
        if (a == 0) {
            return b;
        }
        return line + 1;
    }


    // printString content
    public static void printString(ArrayList<String> operands, Integer line) {
        if (operands.size() < 2) {
            System.out.println("ERROR" + line);
            return;
        }
        String content = RunningStack.getConstString(operands.get(1));
        System.out.print(content);
    }

    // printInteger integer
    public static void printInteger(ArrayList<String> operands, Integer line) {
        int a;
        try {
            a = RunningStack.getInteger(operands.get(1));
        } catch (VariableNotExists e) {
            e.print(line);
            return;
        }
        System.out.print(a);
    }

    public static void readInt(ArrayList<String> operands, Integer line) {
        try {
            RunningStack.readIntegerFromInput();
        } catch (InputEmpty inputEmpty) {
            inputEmpty.print(line);
        }
    }

    // getInt assigned
    public static void getInt(ArrayList<String> operands, Integer line) {
        Integer a = RunningStack.inputInteger;
        if (a == null) {
            System.out.println("get integer before input " + (line + 1));
            return;
        }
        RunningStack.setInteger(operands.get(1), a, line);
        a = null;
    }

    public static void getReturn(ArrayList<String> operands, Integer line) {
        RunningStack.setInteger(operands.get(1), RunningStack.returnVal, line);
    }

    // phiNode assigned integer label ...
    public static void phiNode(ArrayList<String> operands, Integer line) {
        String label = RunningStack.lastLabel.peek();
        Integer a = null;
        for (int i = 3; i < operands.size(); i += 2) {
            if (label.equals(operands.get(i))) {
                try {
                    a = RunningStack.getInteger(operands.get(i - 1));
                } catch (VariableNotExists variableNotExists) {
                    variableNotExists.print(line);
                }
            }
        }
        if (a == null) {
            System.out.println("Unmatched label in phi node " + (line + 1));
            return;
        }
        RunningStack.setInteger(operands.get(1), a, line);
    }
}
