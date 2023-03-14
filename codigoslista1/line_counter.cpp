#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <nome do arquivo>\n";
    return 1;
  }

  std::ifstream arquivo(argv[1]);
  if (!arquivo.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo.\n";
    return 1;
  }

  int num_caracteres = 0;
  int num_palavras = 0;
  int num_linhas = 0;

  std::string linha;
  while (std::getline(arquivo, linha)) {
    num_caracteres += linha.length();
    num_palavras += std::count_if(linha.begin(), linha.end(),
                                  [](char c) { return std::isspace(c); });
    num_linhas++;
  }

  arquivo.close();

  std::cout << "Numero de caracteres: " << num_caracteres << '\n';
  std::cout << "Numero de palavras: " << num_palavras << '\n';
  std::cout << "Numero de linhas: " << num_linhas << '\n';

  return 0;
}