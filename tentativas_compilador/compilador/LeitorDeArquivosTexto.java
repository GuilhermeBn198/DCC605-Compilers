import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
public class LeitorDeArquivosTexto {
    InputStream is;
    public LeitorDeArquivosTexto(String arquivo) {
        try {
            is = new FileInputStream(new File(arquivo));
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
    }
    public int lerProximoCaractere() {
        try {
            int ret = is.read();
            System.out.print((char)ret);
            return ret;
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
            return -1;
        }
    }
}