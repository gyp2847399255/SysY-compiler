public class ParamsNumberUnmatched extends MyException {
    @Override
    public void print(int line) {
        System.out.println("ParamsNumberUnmatched " + line);
    }
}
