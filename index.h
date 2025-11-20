#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>
namespace fs = std::filesystem;
using namespace std;

class helper{
    
    public:
    void lerArquivo();
    string processarTexto(){
        string removerPonto();
        string ConvertMinusculo();
        string stopWords();
    }

    string Buscar();
    string multiplaBusca();
    
    int txtToId(); 

    private:
    string removerPonto();
    string ConvertMinusculo();
    string stopWords();
    
};

class index{
    int convertToId();//manter o indice principal (Converter o nome"texto" para o ID "int")
    void includeID(); //inclui o id na tabela
    string convertToName(); //converter o id para o nome do arquivo 
    
}; 

//Fernando
class textProcessor{
    string processarTexto(){
        string removerPonto();
        string ConvertMinusculo();
        string stopWords();
    }
};


//Matheus
class indexer{
    private:
    unordered_map<int, vector<string>> tabelaHash;

    unordered_map<string,int> nomeParaId;
    int contadorId=1;

};

class Serializer{
    void saveIndece();
    void loadIndice();
};

class queryProcessor{
    string Buscar(string palavra); //retorna os nomes dos arquivos que possuem a palavra buscada
    string multiplaBusca(string palavra1,string palavra2); //retorna os nomes dos arquivos que possuem a palavra buscada
    
};

class comandLineInterface{
    void funcoesDaMain(); // funções da main Dependendo dos argumentos, ele deve criar e utilizar os objetos Indexer, Serializer e QueryProcessor para realizar as operações de indexação ou busca
};