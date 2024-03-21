// Code Review: Write comments
import java.io.FileWriter;

public class DataGenerator {

    private static void writeToFile(String filename, int[] arr) {
        try {
            FileWriter fileWriter = new FileWriter(filename);
            for(int i: arr) {
                fileWriter.write(i + "\n");
            }
            fileWriter.close();
        } catch(Exception e) {
            System.out.println("Error" + e.toString());
        }
    }

    private static int getRandomNumber(int min, int max) {
        return (int) Math.floor(Math.random() * (max - min + 1) + min);
    }

    private static void generateSequentialData(int count) {
        int[] arr = new int[count];
        for(int i = 1; i <= count; i++) {
            if(i%1000 == 0) {
                arr[i-1] = getRandomNumber(i-500, i);
            } else {
                arr[i-1] = i;
            }
        }
        writeToFile("./data/file_seq_" + count + ".txt", arr);
    }

    private static void generateLoopingData(int count) {
        int[] arr = new int[count];
        int[] loopParam = {200, 400, 600};
        int loopParamIdx = getRandomNumber(0, loopParam.length - 1), val = 1;
        for(int i = 0; i < count; i++) {
            arr[i] = val++;
            if(i % loopParam[loopParamIdx] == 0) {
                val = 1;
                loopParamIdx = getRandomNumber(0, loopParam.length - 1);
            }
        }
        writeToFile("./data/file_loop_" + count + ".txt", arr);
    }

    private static void generateRandomData(int count) {
        int[] arr = new int[count];
        for(int i = 0; i < count; i++) {
            arr[i] = getRandomNumber(1, 1000);
        }
        writeToFile("./data/file_rand_" + count + ".txt", arr);
    }

    public static void main(String[] args) {
        generateSequentialData(10000);
        generateSequentialData(100000);
        generateSequentialData(1000000);
        
        generateLoopingData(10000);
        generateLoopingData(100000);
        generateLoopingData(1000000);
        
        generateRandomData(10000);
        generateRandomData(100000);
        generateRandomData(1000000);

    }
}
