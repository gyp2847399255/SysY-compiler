public class VariableNotExists extends MyException {
    String variable;

    public VariableNotExists(String variable) {
        this.variable = variable;
    }

    public void print(int line) {
        System.out.println("Read nonexistent variable '" + variable + "' on line " + (line + 1));
    }
}
