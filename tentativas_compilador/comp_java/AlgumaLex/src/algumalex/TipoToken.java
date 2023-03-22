/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package algumalex;

/**
 *
 * @author Guilherme
 */
public enum TipoToken {
    PCDeclaracoes/**/, PCAlgoritmo/**/, 

    PCInteiro/**/, PCReal/**/,

    PCAtribuir/**/, PCA/**/, PCLer/**/, 
    PCImprimir/**/, PCSe/**/, PCEntao/**/, 
    PCEnquanto/**/,

    PCInicio/**/, PCFim/**/,

    OpAritMult/**/, OpAritDiv/**/, OpAritSoma/**/,OpAritSub/**/,

    OpRelMenor/**/, OpRelMenorIgual/**/, OpRelMaiorIgual/**/,
    OpRelMaior/**/, OpRelIgual/**/, OpRelDif/**/,


    OpBoolE/**/, OpBoolOu/**/,

    Delim/**/, 
    
    AbrePar/**/, FechaPar/**/, 
    
    Var/**/, NumInt/**/, NumReal/**/, Cadeia/**/, Fim/**/
} //enum para identificar os tipos de Tokens