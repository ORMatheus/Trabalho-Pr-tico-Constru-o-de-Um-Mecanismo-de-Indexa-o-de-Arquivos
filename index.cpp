#include "index.h"

// ==========================================
// IMPLEMENTAÇÃO DO TEXTPROCESSOR
// ==========================================

TextProcessor::TextProcessor(const std::string& stopwords_path) {
    std::ifstream file(stopwords_path);
    if (file.is_open()) {
        std::string word;
        while (std::getline(file, word)) {
            // Remove espaços em branco extras
            word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
            if (!word.empty()) {
                stopwords.insert(word);
            }
        }
    }
}

std::vector<std::string> TextProcessor::process(const std::string& text) const {
    std::string cleaned = to_lowercase(text);
    cleaned = remove_punctuation(cleaned);
    std::vector<std::string> words = split_into_words(cleaned);

    std::vector<std::string> result;
    result.reserve(words.size());
    for (const auto& w : words) {
        if (!w.empty() && stopwords.find(w) == stopwords.end()) {
            result.push_back(w);
        }
    }
    return result;
}

std::string TextProcessor::to_lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string TextProcessor::remove_punctuation(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return std::ispunct(c) ? ' ' : c;
    });
    return s;
}

std::vector<std::string> TextProcessor::split_into_words(const std::string& s) {
    std::vector<std::string> words;
    std::stringstream ss(s);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// ==========================================
// IMPLEMENTAÇÃO DO INDEXER
// ==========================================

Indexer::Indexer() : contadorId(1), processador("stopwords.txt") {}

int Indexer::getId(string nomedoArquivo) {
    if (nomeParaId.find(nomedoArquivo) == nomeParaId.end()) {
        nomeParaId[nomedoArquivo] = contadorId++;
    }
    return nomeParaId[nomedoArquivo];
}

void Indexer::saveOnHash(int id, const vector<string>& palavras) {
    tabelaHash[id].insert(tabelaHash[id].end(), palavras.begin(), palavras.end());
}

void Indexer::lerArquivos(string caminhoPasta) {
    if (!fs::exists(caminhoPasta)) {
        cout << "Erro: Pasta '" << caminhoPasta << "' nao encontrada!" << endl;
        return;
    }

    cout << "Lendo pasta: " << caminhoPasta << "..." << endl;

    for (const auto& entry : fs::directory_iterator(caminhoPasta)) {
        if(entry.is_regular_file()) {
            string caminhoCompleto = entry.path().string();
            string nomeArquivo = entry.path().filename().string();
            
            int idAtual = getId(nomeArquivo);
            
            ifstream arquivo(caminhoCompleto);
            if (arquivo.is_open()) {
                string linha;
                vector<string> conteudoArquivoTratado;

                while (getline(arquivo, linha)) {
                    vector<string> palavrasDaLinha = processador.process(linha);
                    conteudoArquivoTratado.insert(conteudoArquivoTratado.end(), 
                                                  palavrasDaLinha.begin(), 
                                                  palavrasDaLinha.end());
                }
                saveOnHash(idAtual, conteudoArquivoTratado);
                cout << " - Indexado: " << nomeArquivo << " (ID: " << idAtual << ")" << endl;
                arquivo.close();
            }
        }
    }
}

void Indexer::imprimirEstado() {
    cout << "\n--- ESTADO DO INDEXADOR ---" << endl;
    cout << "Arquivos mapeados: " << nomeParaId.size() << endl;
    for(auto const& [nome, id] : nomeParaId) {
        cout << " [" << id << "] " << nome << endl;
    }
    cout << "---------------------------" << endl;
}

// ==========================================
// IMPLEMENTAÇÃO DO SERIALIZER
// ==========================================

void Serializer::writeString(ofstream& out, const string& s) {
    size_t len = s.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(s.c_str(), len);
}

string Serializer::readString(ifstream& in) {
    size_t len;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    string s(len, '\0');
    in.read(&s[0], len);
    return s;
}

void Serializer::saveIndex(const Indexer& indexer, const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Erro ao criar arquivo de salvamento!" << endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&indexer.contadorId), sizeof(indexer.contadorId));

    size_t mapSize1 = indexer.nomeParaId.size();
    outFile.write(reinterpret_cast<const char*>(&mapSize1), sizeof(mapSize1));
    for (const auto& pair : indexer.nomeParaId) {
        writeString(outFile, pair.first);
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

    size_t mapSize2 = indexer.tabelaHash.size();
    outFile.write(reinterpret_cast<const char*>(&mapSize2), sizeof(mapSize2));
    for (const auto& pair : indexer.tabelaHash) {
        int id = pair.first;
        outFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        
        size_t vectorSize = pair.second.size();
        outFile.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));
        
        for (const string& s : pair.second) {
            writeString(outFile, s);
        }
    }
    outFile.close();
    cout << "Dados salvos em '" << filename << "' com sucesso." << endl;
}

void Serializer::loadIndex(Indexer& indexer, const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "Nenhum indice anterior encontrado. Comecando do zero." << endl;
        return;
    }

    indexer.nomeParaId.clear();
    indexer.tabelaHash.clear();

    inFile.read(reinterpret_cast<char*>(&indexer.contadorId), sizeof(indexer.contadorId));

    size_t mapSize1;
    inFile.read(reinterpret_cast<char*>(&mapSize1), sizeof(mapSize1));
    for (size_t i = 0; i < mapSize1; i++) {
        string nome = readString(inFile);
        int id;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
        indexer.nomeParaId[nome] = id;
    }

    size_t mapSize2;
    inFile.read(reinterpret_cast<char*>(&mapSize2), sizeof(mapSize2));
    for (size_t i = 0; i < mapSize2; i++) {
        int id;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
        
        size_t vectorSize;
        inFile.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));
        
        vector<string> palavras;
        palavras.reserve(vectorSize);
        for (size_t j = 0; j < vectorSize; j++) {
            palavras.push_back(readString(inFile));
        }
        indexer.tabelaHash[id] = palavras;
    }
    inFile.close();
    cout << "Indice carregado com sucesso de '" << filename << "'." << endl;
}
