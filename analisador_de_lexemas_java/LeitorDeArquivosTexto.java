package analisador_de_lexemas_java;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

public class LeitorDeArquivosTexto {
	private final static int TAMANHO_BUFFER = 5;
	int[] bufferDeLeitura;
	int ponteiro;
	int bufferAtual;
	int inicioLexema;
	private String lexema;

	InputStream is;

	public LeitorDeArquivosTexto(String arquivo){ //vai ler os caracteres e inicilizar o buffer
		try {
			is = new FileInputStream(new File(arquivo));	
			inicializarBuffer();
		}
		catch (FileNotFoundException ex) {
			Logger.getLogger(LeitorDeArquivosTexto.class.getName()).log(Level.SEVERE, null, ex);
		}
	}

	private void inicializarBuffer(){
		bufferAtual = 2;
		inicioLexema = 0;
		lexema = "";
		bufferDeLeitura = new int[TAMANHO_BUFFER * 2];
		ponteiro = 0;
		recarregarBuffer1();
	}

	private void incrementarPonteiro(){
		ponteiro++;
		if (ponteiro == TAMANHO_BUFFER) {
			recarregarBuffer2();
		} else if(ponteiro == TAMANHO_BUFFER * 2){
			recarregarBuffer1();
			ponteiro = 0;
		}
		//sempre que passarmos da metade 1 pra metade 2 temos que carregar o buffer da direita, quando tivermos na metade 2 teremos que voltar e carregar o buffer da esquerda
	}

	private void recarregarBuffer1(){
		if (bufferAtual == 2) { 
			bufferAtual = 1; //logica circular sem bug de carregamento duplo
			for (int i = 0; i < TAMANHO_BUFFER; i++) {
				try {
					bufferDeLeitura[i] = is.read();
					if (bufferDeLeitura[i] == -1) {
						break;
					}	
				} catch (IOException ex) {
				Logger.getLogger(LeitorDeArquivosTexto.class.getName()).log(Level.SEVERE, null, ex);
				}
			}
		}
	}

	private void recarregarBuffer2(){
		if (bufferAtual == 1) {//logica circular sem bug de carregamento duplo
			bufferAtual = 2;
			for (int i = 0; i < TAMANHO_BUFFER * 2; i++) {
				try {
					bufferDeLeitura[i] = is.read();
					if (bufferDeLeitura[i] == -1) {
						break;
					}	
				} catch (IOException ex) {
				Logger.getLogger(LeitorDeArquivosTexto.class.getName()).log(Level.SEVERE, null, ex);
				}
			}
		}
	}

	private int lerCaractereDoBuffer(){
		int ret = bufferDeLeitura[ponteiro];
		System.out.println(this); //kd vez q é lido 1 caracter do buffer, o mesmo é imprimido no console
		incrementarPonteiro();
		return ret;
	}

	public int lerProximoCaractere(){
		int c = lerCaractereDoBuffer();
		lexema += (char)c;
		return c;
	}

	public void retroceder(){
		//serve para resetar o ponteiro
		ponteiro--;
		lexema = lexema.substring(0, lexema.length() - 1);
		if (ponteiro < 0) {
			ponteiro = TAMANHO_BUFFER * 2 - 1;
		}
	}

	public void zerar(){ //sempre que se tentar um padrao e não conseguir, se zera o lexema
		ponteiro = inicioLexema;
		lexema = "";
	}

	public void confirmar(){ //confirma se um padrão é um lexema msm
		inicioLexema = ponteiro;
		lexema = "";
	}

	public String getLexema(){
		return lexema;
	}
	
	@Override
	public String toString(){
		String ret = "Buffer:[";
		for (int i : bufferDeLeitura) {
			char c = (char) i;
			if (Character.isWhitespace(c)) {
				ret += ' ';
			} else {
				ret += (char) i;
			}
		}
		ret += "]\n";
		ret += "	";
		for (int i = 0; i < TAMANHO_BUFFER * 2; i++) {
			if (i == inicioLexema && i == ponteiro) {
				ret += "%";
			} else if(i == inicioLexema){
				ret += "^";
			} else if(i == ponteiro){
				ret += "*";
			}else {
				ret += " ";
			}
		}
		return ret;
	}
}
