/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Main.java to edit this template
 */
package algumalex;

/**
 *
 * @author Guilherme
 */
public class AlgumaLex {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        AlgumaLexico lex = new AlgumaLexico(args[0]);
        Token t = null;
        while((t = lex.proximoToken()) != null) {
            System.out.print(t);
        }
    }
}
