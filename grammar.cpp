#include <string>
#include <vector>
#include <iostream>


struct Rule {
    char _non_terminal;
    std::string _product;

public:
    Rule(char _non_terminal, std::string& _product):
        _non_terminal(_non_terminal),
        _product(_product) {}
    Rule(char _non_terminal, std::string&& _product):
        _non_terminal(_non_terminal),
        _product(std::move(_product)) {}

    explicit Rule(): _non_terminal('S') {}

    bool operator==(const Rule& other) const {
        return _non_terminal == other._non_terminal &&
               _product == other._product;
    }

    friend std::istream& operator>>(std::istream& in, Rule& rule) {
        in >> rule._non_terminal >> rule._product;
        if (rule._product == "empty") {
            rule._product = "";
        }
        return in;
    }
};


struct Grammar {
    char _start;
    std::vector<Rule> _rules;

public:
    Grammar(char _start, std::vector<Rule>& _rules):
        _start(_start),
        _rules(_rules) {}
    Grammar(char _start, std::vector<Rule>&& _rules):
            _start(_start),
            _rules(std::move(_rules)) {}

    bool operator==(const Grammar& other) const {
        return _start == other._start &&
               _rules == other._rules;
    }

    void add_rule(Rule& rule) {
        _rules.push_back(rule);
    }

    void add_rule(Rule&& rule) {
        _rules.push_back(std::move(rule));
    }

    std::vector<Rule> products(char non_terminal) {
        std::vector<Rule> rules(0);
        for (auto& rule: _rules) {
            if (rule._non_terminal == non_terminal) {
                rules.push_back(rule);
            }
        }
        return rules;
    }
};


struct Situation {
    Rule _rule;
    int _position;
    int _to;

public:
    Situation(Rule& rule, int pos, int to):
        _rule(rule),
        _position(pos),
        _to(to) {}

    Situation(Rule&& rule, int pos, int to):
            _rule(std::move(rule)),
            _position(pos),
            _to(to) {}

    bool operator==(const Situation& other) const {
        return _rule == other._rule &&
        _position == other._position &&
        _to == other._to;
    }

    bool operator<(const Situation& other) const {
        return _position < other._position &&
               _to < other._to;
    }

    [[nodiscard]] bool is_complete() const {
        return _rule._product.size() == _position;
    }
};

template <class T>
inline void hash_combine(std::size_t & s, const T & v)
{
    std::hash<T> h;
    s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}


struct SituationHash
{
    std::size_t operator()(Situation const& s) const
    {
        std::size_t res = 0;
        hash_combine(res,s._position);
        hash_combine(res,s._to);
        hash_combine(res,s._rule._non_terminal);
        hash_combine(res,s._rule._product);
        return res;
    }
};