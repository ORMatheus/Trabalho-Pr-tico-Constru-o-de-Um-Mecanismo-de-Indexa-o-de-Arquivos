#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>

// Facilita o uso do filesystem
namespace fs = std::filesystem;
using namespace std;

// Declaração antecipada para que o Serializer saiba que o Indexer existe
class Indexer;

// --- CLASSE SERIALIZER ---
class Serializer {
public:
    void saveIndex(const Indexer& indexer, const string& filename);
    void loadIndex(Indexer& indexer, const string& filename);

private:
    void writeString(ofstream& out, const string& s);
    string readString(ifstream& in);
};

// --- CLASSE TEXTPROCESSOR ---
class TextProcessor {
public:
    explicit TextProcessor(const std::string& stopwords_path = "stopwords.txt");
    std::vector<std::string> process(const std::string& text) const;

private:
    std::unordered_set<std::string> stopwords;
    static std::string to_lowercase(std::string s);
    static std::string remove_punctuation(std::string s);
    static std::vector<std::string> split_into_words(const std::string& s);
};

// --- CLASSE INDEXER ---
class Indexer {
    // Permite que o Serializer acesse os dados privados (tabelaHash, etc.)
    friend class Serializer;

private:
    unordered_map<int, vector<string>> tabelaHash;
    unordered_map<string, int> nomeParaId;
    int contadorId;
    TextProcessor processador; 

public:
    Indexer();
    int getId(string nomedoArquivo);
    void saveOnHash(int id, const vector<string>& palavras);
    void lerArquivos(string caminhoPasta);
    void imprimirEstado();
};

#endif // INDEX_H
