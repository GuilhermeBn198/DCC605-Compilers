/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package algumalex;

/**
 *
 * @author Guilherme
 */
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
public class LeitorDeArquivosTexto {
    InputStream is;
    public LeitorDeArquivosTexto(String arquivo) {
        try {
            is = new FileInputStream(new File(arquivo));
        } catch (FileNotFoundException ex) {
            ex.printStackTrace(System.err);
        }
    }
    public int lerProximoCaractere() {
        try {
            int ret = is.read();
            System.out.print((char)ret);
            return ret;
        } catch (IOException ex) {
            ex.printStackTrace(System.err);
            return -1;
        }
    }
}