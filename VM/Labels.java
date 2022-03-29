import java.util.ArrayList;
import java.util.HashMap;

public class Labels {
    private static final HashMap<String, Integer> map = new HashMap<>();
    private static final HashMap<String, ArrayList<String>> params = new HashMap<>();

    public static boolean addLabel(String label, ArrayList<String> arrayList, Integer line) {
        if (map.containsKey(label)) {
            return false;
        }
        map.put(label, line);
        params.put(label, arrayList);
        return true;
    }

    public static Integer getLabel(String label) throws LabelNotExists {
        Integer res = map.get(label);
        if (res == null) {
            throw new LabelNotExists(label);
        }
        return res;
    }

    public static ArrayList<String> getParams(String label) throws LabelNotExists {
        ArrayList<String> ret = params.get(label);
        if (ret == null) {
            throw new LabelNotExists(label);
        }
        return ret;
    }
}
