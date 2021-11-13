#include <iostream>
#include "early.cpp"


int main() {
    int rules_number;
    std::cout << "Введите количество правил:\n";
    std::cin >> rules_number;

    char start;
    std::cout << "Введите стартовый нетерминал:\n";
    std::cin >> start;

    auto grammar = Grammar(start, {});
    std::cout << "Введите правила в грамматике. Для пустого слова введите empty\n";
    for (int i = 0; i < rules_number; ++i) {
        Rule rule;
        std::cin >> rule;
        grammar.add_rule(rule);
    }

    std::string word;
    std::cout << "Введите слово, которое хотите проверить:\n";
    std::cin >> word;

    auto early = EarlyAlgorithm(grammar, word);
    auto answer = early.iterate();

    if (answer)
        std::cout << "Слово лежит в грамматике\n";
    else
        std::cout << "Слово не лежит в грамматике\n";
}