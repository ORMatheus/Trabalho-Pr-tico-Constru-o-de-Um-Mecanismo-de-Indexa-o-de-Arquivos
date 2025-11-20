#include <iostream>
#include <string>
#include <filesystem>
using namespace std;

class Index{
    
    public:
    void lerArquivo();
    string processarTexto(){
        string removerPonto();
        string ConvertMinusculo();
        string stopWords();
    }
    void saveIndece();
    void loadIndice();
    string Buscar();
    string multiplaBusca();

    private:
    string removerPonto();
    string ConvertMinusculo();
    string stopWords();
    
};

class TableHash{

};




