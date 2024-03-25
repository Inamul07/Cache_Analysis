import java.io.FileWriter;

public class DataGenerator {

    /*
     * This method performs write operation.
     * This method gets a filename and an integer array as parameters.
     * Creates a file (if not present) or Overwrites a file (if already present).
     * Puts each value of the array in a single line of the file.
     */
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

    // Returns a random number between min and max (inclusive).
    private static int getRandomNumber(int min, int max) {
        return (int) Math.floor(Math.random() * (max - min + 1) + min);
    }

    /*
     * Generates Data with Sequential Access Pattern and writes it to a file.
     * For Every 1000 numbers, it chooses a random number from the previous 500 numbers.
     * Example: {....., 998, 999, [577], 1001, 1002, .....}
     * Here 577 is a random number between [500, 1000].
     */
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

    /*
     * Generates Data with Looping Access Pattern and writes it to a file.
     * The pattern resets loops back to 1 at either 200, 400 or 600.
     * Example: {....., 199, 200, 1, 2, ....., 599, 600, 1, 2, ....., 399, 400, 1, 2, .....}
     */
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

    /*
     * Generates Data with Random Access Pattern and writes it to a file.
     * This method generates random values between [1, 1000].
     */
    private static void generateRandomData(int count) {
        int[] arr = new int[count];
        for(int i = 0; i < count; i++) {
            arr[i] = getRandomNumber(1, 1000);
        }
        writeToFile("./data/file_rand_" + count + ".txt", arr);
    }

    public static void main(String[] args) {
        // Generate Sequential Data of count 10000, 100000, 1000000
        generateSequentialData(10000); // Ten Thousand
        generateSequentialData(100000); // Hundred Thousand
        generateSequentialData(1000000); // One Million
        
        generateLoopingData(10000); // Ten Thousand
        generateLoopingData(100000); // Hundred Thousand
        generateLoopingData(1000000); // One Million
        
        generateRandomData(10000); // Ten Thousand
        generateRandomData(100000); // Hundred Thousand
        generateRandomData(1000000); // One Million

    }
}
