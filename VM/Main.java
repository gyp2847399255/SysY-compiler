import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        String irFile = "ir.txt";
        String dataFile = "data.txt";
        String inputFile = "input.txt";
        getData(dataFile);
        getInput(inputFile);
        startRunning(irFile);
    }

    private static ArrayList<String> getContent(String file) throws IOException {
        ArrayList<String> content = new ArrayList<>();
        BufferedReader bufferedReader = new BufferedReader(new FileReader(file));
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            content.add(line);
        }
        return content;
    }

    private static void getInput(String file) {
        ArrayList<String> data;
        try {
            data = getContent(file);
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        for (String i : data) {
            String[] tmp = i.split(" ");
            for (String j : tmp) {
                if (j.length() > 0) {
                    RunningStack.addInput(j);
                }
            }
        }
    }

    private static void getData(String dataFile) {
        ArrayList<String> data;
        try {
            data = getContent(dataFile);
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        for (String i : data) {
            if (i.contains("\"")) {
                int firstSpace = i.indexOf(" ");
                int firstQuotation = i.indexOf("\"");
                int lastQuotation = i.lastIndexOf("\"");
                ArrayList<String> constString = new ArrayList<>();
                constString.add(i.substring(0, firstSpace));
                constString.add(i.substring(firstQuotation + 1, lastQuotation));
                RunningStack.addConstString(constString);
            } else {
                String[] tmp = i.split(" ");
                ArrayList<String> globalVariable = new ArrayList<>();
                for (String j : tmp) {
                    if (j.length() > 0) {
                        globalVariable.add(j);
                    }
                }
                RunningStack.addGlobalVariable(globalVariable);
            }
        }
    }

    private static void startRunning(String testFile) {
        ArrayList<String> content;
        try {
            content = getContent(testFile);
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        int entrance = 0;
        for (int i = 0; i < content.size(); i++) {
            String lineContent = content.get(i);
            if (lineContent.contains(":")) {
                String[] s = lineContent.split("[ :]");
                String label = null;
                ArrayList<String> arrayList = new ArrayList<>();
                for (String value : s) {
                    if (value.length() > 0) {
                        if (label == null) {
                            label = value;
                        } else {
                            arrayList.add(value);
                        }
                    }
                }
                if (!Labels.addLabel(label, arrayList, i)) {
                    System.out.println("label duplicated");
                    return;
                }
                if ("func_main".equals(label)) {
                    entrance = i;
                }
            }
        }

        long start = System.currentTimeMillis();
        for (int i = entrance; i < content.size(); i++) {
            if (System.currentTimeMillis() - start > 5000) {
                return;
            }
            if (content.get(i).contains(":")) {
                String[] s = content.get(i).split("[ :]");
                String label = null;
                for (String value : s) {
                    if (value.length() > 0) {
                        label = value;
                        break;
                    }
                }
                RunningStack.changeLabel(label);
                continue;
            } else if (content.get(i).length() == 0) {
                continue;
            }
            String[] strings = content.get(i).split(" ");
            ArrayList<String> operands = new ArrayList<>();
            for (String j : strings) {
                if (j.length() > 0) {
                    operands.add(j);
                }
            }
            int k =  Handler.handle(operands, i);
            if (k >= 0) {
                i = k - 1;
            }
        }
    }
}