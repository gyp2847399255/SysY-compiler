public class LabelNotExists extends MyException {
    private String label;

    public LabelNotExists(String label) {
        this.label = label;
    }

    public void print(int line) {
        System.out.println("Read nonexistent label '" + label + "' on line " + (line + 1));
    }
}
