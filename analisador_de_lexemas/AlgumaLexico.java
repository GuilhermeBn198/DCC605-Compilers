package analisador_de_lexemas;

/**
 * Lex
 */
public class AlgumaLexico {
	LeitorDeArquivosTexto ldat;

	public AlgumaLexico(String arquivo) {
		ldat = new LeitorDeArquivosTexto(arquivo);
	}

	public Token proximoToken() {
		int caractereLido = -1;
		
		while ((caractereLido = ldat.lerProximoCaractere())!=-1){
			char c = (char)caractereLido;
			if(c == ' ' || c == '\n')continue;

			//operadores matematicos
			if (c == ':') {
				return new Token(TipoToken.DELim, ":");
			} else if (c =='*') {
				return new Token (TipoToken. OPAritMult, "");
			} else if (c == '/'){
				return new Token (TipoToken.OPAritDiv, "/");
			} else if (c == '+') {
				return new Token (TipoToken. OPAritSoma, "+");
			}else if (c == '-') {
				return new Token (TipoToken. OPAritSub, "-");
			} else if (c == '(') {
				return new Token (TipoToken. ABrePar, "(");
			}else if (c == ')'){
				return new Token (TipoToken. FechaPar, ")");
			}
			//caracteres lógicos
			else if(c == '<'){
				c = (char) ldat.lerProximoCaractere();
				if (c == '>') {
					return new Token(TipoToken.OPRelDif, "<>");
				} else if (c == '='){
					return new Token(TipoToken.OPRelMenorIgual, "<=");
				} else {
					ldat.retroceder();
					return new Token(TipoToken.OPRelMenor, "<");
				}
			}
		}
		return null;
	} 
}