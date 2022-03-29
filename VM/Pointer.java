import java.util.ArrayList;

public class Pointer {
    private ArrayList<Integer> arrayList;
    private int n;

    public Pointer() {}

    public Pointer(int size) {
        arrayList = new ArrayList<>();
        this.n = 0;
        for (int i = 0; i < size; i++) {
            arrayList.add(0);
        }
    }

    public Pointer(ArrayList<String> global) {
        this.n = 0;
        arrayList = new ArrayList<>();
        int size = Integer.parseInt(global.get(1));
        if (global.size() == 2) {
            for (int i = 0; i < size; i++) {
                arrayList.add(0);
            }
        } else {
            assert global.size() == size + 2;
            for (int i = 2; i < global.size(); i++) {
                arrayList.add(Integer.parseInt(global.get(i)));
            }
        }
    }

    public int getVal(int offset) throws ArrayOutBound {
        if (offset + n >= arrayList.size() || offset + n < 0) {
            throw new ArrayOutBound();
        }
        return arrayList.get(offset + n);
    }

    public void setVal(int offset, int res) throws ArrayOutBound {
        if (offset + n >= arrayList.size() || offset + n < 0) {
            throw new ArrayOutBound();
        }
        arrayList.set(offset + n, res);
    }

    public void addN(int offset) throws ArrayOutBound {
        if (offset + n >= arrayList.size() || offset + n < 0) {
            throw new ArrayOutBound();
        }
        this.n += offset;
    }

    @Override
    public Object clone() {
        Pointer ret = new Pointer();
        ret.n = this.n;
        ret.arrayList = this.arrayList;
        return ret;
    }
}
