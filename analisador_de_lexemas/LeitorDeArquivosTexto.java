package analisador_de_lexemas;

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

	private void recarregarBuffer2(){
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

	private int lerCaractereDoBuffer(){
		int ret = bufferDeLeitura[ponteiro];
		incrementarPonteiro();
		return ret;
	}

	public int lerProximoCaractere(){
		int c = lerCaractereDoBuffer();
		System.out.print((char)c);
		return c;
	}

	public void retroceder(){
		//serve para resetar o ponteiro
		ponteiro--;
		if (ponteiro < 0) {
			ponteiro = TAMANHO_BUFFER * 2 - 1;
		}
	}
}
