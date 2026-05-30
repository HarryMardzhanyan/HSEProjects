// clang++ -std=c++20 -o TextAnalyzer TextAnalyzer.cpp
// ./TextAnalyzer

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <functional>

class TFIDFAnalyze {
private:
    // Внутренняя структура: вся стата документа
    struct DocumentInfo {
        // Всего слов в документе
        int total_words = 0;

        // Частота каждого слова
        std::unordered_map<std::string, int> word_counts;
    };

    // ХЭШ-ТАБЛИЦЫ ДЛЯ ХРАНЕНИЯ ДАННЫХ
    // Хэш-таблица: имя файла и его статистика
    std::unordered_map<std::string, DocumentInfo> documents;

    // Хэш-таблица: слово и сколько документов содержит данное слово
    std::unordered_map<std::string, int> df;

    // Хэш-таблица: слово и список документов, где оно встречается
    std::unordered_map<std::string, std::vector<std::string>> word_to_docs;

    // Количество документов
    int N = 0;

    // Токенизация файла
    std::vector<std::string> tokenize_file(const std::string& filename);

    // Считаем IDF
    double calc_idf(const std::string& word) const;
    double calc_tf(int word_count, int total_words) const;

    // ОБРАБОТКА ЗАПРОСОВ
    void handle_word(const std::string& w) const;
    void handle_word_in_document(const std::string& w, const std::string& doc) const;
    void handle_doc(const std::string& doc) const;
    void handle_query(const std::vector<std::string>& words) const;
public:
    // ИНТЕРФЕЙС
    void initialize_corpus(const std::string& list_filename);
    void process_request(const std::string& line);
};

std::vector<std::string> TFIDFAnalyze::tokenize_file(const std::string &filename){
    std::ifstream file(filename);
    if (!file.is_open()) return {};

    // Читаем весь файл
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Обрабатываем все символы, чтобы заменить пунктуацию на пробелы, и приводим все к нижнему регистру
    std::set<char> punctuation = {'!', '"', '#', '$', '%', '&', '(', ')', '*', '+', ',',
        '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '{', '|', '}', '~'};
    std::transform(text.begin(), text.end(), text.begin(), 
        [&punctuation](unsigned char c){ return (punctuation.find(c) != punctuation.end()) ? ' ' : std::tolower(c); });
    
    // Токенизация текстового файла
    std::istringstream iss(text);
    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

double TFIDFAnalyze::calc_idf(const std::string &word) const {
    auto it = df.find(word);
    if (it == df.end() || it->second == 0) {
        std::cout << "WARNING: there is no such a document that contains word '" << word << "': ";
        return 0.0;
    }
    return std::log(static_cast<double>(N) / it->second);
}

double TFIDFAnalyze::calc_tf(int word_count, int total_words) const {
    return total_words ? static_cast<double>(word_count) / total_words : 0.0;
}

void TFIDFAnalyze::initialize_corpus(const std::string &list_filename) {
    std::ifstream list(list_filename);
    if (!list.is_open()) {
        std::cerr << "ERROR: impossible to open" << list_filename << "\n";
        return;
    }

    // Читаем имена файлов из списка
    std::vector<std::string> filenames{std::istream_iterator<std::string>(list), {}};

    // Обрабатываем каждый документ из documents.txt 
    std::for_each(filenames.begin(), filenames.end(), [this](const std::string& fname){
        auto words = tokenize_file(fname);
        if (words.empty()) return;

        DocumentInfo& doc = documents[fname];
        doc.total_words = words.size();

        // Считаем частоту слов в текущем документе
        std::for_each(words.begin(), words.end(), [&doc](const std::string& w){
            doc.word_counts[w]++;
        });

        // Обновляем всю статистику конкретного документа
        std::for_each(doc.word_counts.begin(), doc.word_counts.end(), [this, &fname](const auto& p){
            // Увеличиваем счётчик документов для каждого уникального слова
            df[p.first]++;

            // Добавляем имя текущего файла в список документов, где было данное слово
            word_to_docs[p.first].push_back(fname);
        });
    });

    // Обновляем значение числа документов
    N = documents.size();
}

void TFIDFAnalyze::process_request(const std::string &line) {
    std::istringstream iss(line);

    // Разбираем строку (из терминала) на токены
    std::vector<std::string> tokens{std::istream_iterator<std::string>(iss), {}};
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];

    if (cmd == "WORD" && tokens.size() == 2) {
        handle_word(tokens[1]);
    } else if (cmd == "WORD_IN_DOC" && tokens.size() == 3) {
        handle_word_in_document(tokens[1], tokens[2]);
    } else if (cmd == "DOC" && tokens.size() == 2) {
        handle_doc(tokens[1]);
    } else if (cmd == "QUERY" && tokens.size() >= 2) {
        handle_query({tokens.begin() + 1, tokens.end()});
    } else {
        std::cout << "ERROR: eto schlyapa, invalid command format" << "\n";
    }
}

void TFIDFAnalyze::handle_word(const std::string &w) const {
    auto it = word_to_docs.find(w);
    int doc_count = (it != word_to_docs.end()) ? it->second.size() : 0;

    std::cout << "Word: " << w << "\n";
    std::cout << "Documents total: " << N << "\n";
    std::cout << "Documents with word: " << doc_count << "\n";
    std::cout << "IDF: " << calc_idf(w) << "\n";

    std::cout << "Appears in:\n";
    if (it != word_to_docs.end()) {
        std::for_each(it->second.begin(), it->second.end(), [](const std::string& f){
            std::cout << "- " << f << "\n";
        });
    } else  {
        std::cout << "Not found in any document" << "\n";
    }
}

void TFIDFAnalyze::handle_word_in_document(const std::string &w, const std::string &doc) const {
    auto doc_it = documents.find(doc);
    if (doc_it == documents.end()) {
        std::cout << "ERROR: The document '" << doc << "' hasn't been found" << "\n";
        return;
    }

    const auto& d = doc_it->second;
    int counter = 0;
    auto wc_it = d.word_counts.find(w);
    if (wc_it != d.word_counts.end()) counter = wc_it->second;

    double tf = calc_tf(counter, d.total_words);
    double tf_idf = tf * calc_idf(w);

    std::cout << "Word: " << w << "\n";
    std::cout << "Document: " << doc << "\n";
    std::cout << "Count: " << counter << "\n";
    std::cout << "TF: " << tf << "\n";
    std::cout << "TF-IDF: " << tf_idf << "\n";
}

void TFIDFAnalyze::handle_doc(const std::string &doc) const {
    auto doc_it = documents.find(doc);
    if (doc_it == documents.end()) {
        std::cout << "ERROR: The document '" << doc << "' hasn't been found" << "\n";
        return;
    }

    const auto& d = doc_it->second;
    std::vector<std::pair<std::string, double>> scores;
    scores.reserve(d.word_counts.size());

    std::transform(d.word_counts.begin(), d.word_counts.end(), std::back_inserter(scores), [this, &d](const auto& p){
        double tf = calc_tf(p.second, d.total_words);
        double idf = calc_idf(p.first);
        return std::make_pair(p.first, tf * idf);
    });

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });

    if (scores.size() > 5) scores.resize(5);

    std::cout << "Document: " << doc << "\n";
    std::cout << "Total words: " << d.total_words << "\n";
    std::cout << "Unique words: " << d.word_counts.size() << "\n";

    std::cout << "Top words: " << "\n";
    int c = 1;
    for_each(scores.begin(), scores.end(), [&c](const auto &p){
        std::cout << c++ << ". " << p.first << " (" << p.second << ")" << "\n";
    });
}

void TFIDFAnalyze::handle_query(const std::vector<std::string> &words) const {
    std::vector<std::pair<std::string, double>> doc_scores;
    doc_scores.reserve(documents.size());

    // Вычисляем для каждого документа, степень соответствия к нашему query
    std::transform(documents.begin(), documents.end(), std::back_inserter(doc_scores), [this, &words](const auto& doc_pair){
        double score = 0.0;
        std::for_each(words.begin(), words.end(), [this, &doc_pair, &score](const std::string& w){
            auto wc_it = doc_pair.second.word_counts.find(w);
            if (wc_it != doc_pair.second.word_counts.end()) {
                double tf = calc_tf(wc_it->second, doc_pair.second.total_words);
                score += tf * calc_idf(w);
            }
        });
        return std::make_pair(doc_pair.first, score);
    });

    // Фильтруем документы, где tf-idf нулевой
    std::vector<std::pair<std::string, double>> results;
    std::copy_if(doc_scores.begin(), doc_scores.end(), std::back_inserter(results), [](const auto& p){
        return p.second > 1e-12;
    });

    // Сортируем по убыванию значений (TF-IDF)
    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });

    // Вывод результата
    std::cout << "Query: ";
    for_each(words.begin(), words.end(), [](const std::string& w){
        std::cout << w << " ";
    }); std::cout << "\n";

    std::cout << "Results:" << "\n";
    int c = 1;
    for_each(results.begin(), results.end(), [&c](const auto& p){
        std::cout << c++ << ". " << p.first << " (" << p.second << ")" << "\n";
    });
}

int main() {
    std::cout << std::fixed << std::setprecision(4);
    TFIDFAnalyze analyzer;
    analyzer.initialize_corpus("documents.txt");

    std::string line;
    while(std::getline(std::cin, line)) if (!line.empty()) analyzer.process_request(line);
    
    return 0;
}