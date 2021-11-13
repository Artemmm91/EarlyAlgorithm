#include "grammar.cpp"
#include <unordered_set>

class EarlyAlgorithm {
public:
    static const char start_symbol = '$';

private:
    std::vector<std::unordered_set<Situation, SituationHash>> situations;
    Grammar grammar;
    std::string word;
    Rule initial_rule;

    void initialize() {
        size_t n = word.size();
        situations.resize(n + 1);
        grammar._start = start_symbol;
        situations[0].emplace(initial_rule, 0, 0);
    }

    void scan(int j, char letter) {
        for (auto& situation : situations[j]) {
            if (situation.is_complete())
                continue;
            auto position = situation._position;
            if (situation._rule._product[position] == letter) {
                auto new_situation = situation;
                ++new_situation._position;
                situations[j + 1].insert(std::move(new_situation));
            }
        }
    }
    void predict(int j) {
        auto new_situations = std::unordered_set<Situation, SituationHash>();
        for (auto& situation: situations[j]) {
            if (situation.is_complete())
                continue;
            auto non_terminal = situation._rule._product[situation._position];
            for (auto& rule: grammar.products(non_terminal)) {
                auto new_situation = Situation(rule, 0, j);
                new_situations.insert(new_situation);
            }
        }
        situations[j].merge(new_situations);
    }
    void complete(int j) {
        auto new_situations = std::unordered_set<Situation, SituationHash>();
        for (auto& situation: situations[j]) {
            if (!situation.is_complete())
                continue;

            for (auto next_situation: situations[situation._to]) {
                if (next_situation.is_complete())
                    continue;
                if (next_situation._rule._product[next_situation._position] ==
                    situation._rule._non_terminal) {
                    next_situation._position += 1;
                    new_situations.insert(next_situation);
                }
            }
        }
        situations[j].merge(new_situations);
    }

    bool check_finish() {
        auto end_situation = Situation(initial_rule, static_cast<int>(initial_rule._product.size()), 0);
        return situations[word.size()].contains(end_situation);
    }

public:
    EarlyAlgorithm(Grammar& grammar, std::string& word):
            grammar(grammar), word(word),
            initial_rule(start_symbol, std::string(1, grammar._start)) {
        initialize();
    }

    EarlyAlgorithm(Grammar&& grammar, std::string&& word):
            grammar(std::move(grammar)), word(std::move(word)),
            initial_rule(start_symbol, std::string(1, grammar._start)) {
        initialize();
    }

    bool iterate() {
        size_t n = word.size();

        for (int j = 0; j <= n; ++j) {
            if (j > 0) scan(j - 1, word[j - 1]);

            size_t situations_number = situations[j].size();
            size_t previous_number = -1;
            while (previous_number != situations_number) {
                complete(j);
                predict(j);
                previous_number = situations_number;
                situations_number = situations[j].size();
            }
        }

        return check_finish();
    }
};
