public class InputEmpty extends MyException {
    @Override
    public void print(int line) {
        System.out.println("Input has empty " + (line + 1));
    }
}
