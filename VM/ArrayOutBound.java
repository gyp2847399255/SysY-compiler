public class ArrayOutBound extends MyException {

    @Override
    public void print(int line) {
        System.out.println("Array Out of Bound on Line " + (line + 1));
    }
}
