#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <arquivo de origem> <arquivo de destino>\n";
    return 1;
  }

  std::ifstream origem(argv[1]);
  if (!origem.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo de origem.\n";
    return 1;
  }

  std::ofstream destino(argv[2]);
  if (!destino.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo de destino.\n";
    return 1;
  }

  char c;
  while (origem.get(c)) {
    destino.put(c);
  }

  origem.close();
  destino.close();

  std::cout << "Arquivo copiado com sucesso.\n";
  return 0;
}
