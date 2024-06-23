import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class PPMWriter {

    public static void intarrToPPM(File file, int[] arr, int width, int height) throws IOException {
        file.delete();

        try (var os = new FileOutputStream(file, true);
             var bw = new BufferedOutputStream(os)) {
            var header = String.format("P6\n%d %d\n255\n",
                    width, height);

            bw.write(header.getBytes(StandardCharsets.US_ASCII));

            for (var y = 0; y < height; y++) {
                for (var x = 0; x < width; x++) {
                    bw.write(arr[y * width + x]*255);
                    bw.write(arr[y * width + x]*255);
                    bw.write(arr[y * width + x]*255);
                }
            }
        }
    }
}

