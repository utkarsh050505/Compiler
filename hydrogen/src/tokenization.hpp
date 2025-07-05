#pragma once

#include "vector"
#include "string"
#include "optional"

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};    
}; // Done this to Lex the things, or tokenize it.

class Tokenizer {
public:
    inline Tokenizer(std::string src)
        : m_src(std::move(src))
    {

    }

    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        std::string buf;

        for (int i = 0; i < str.length(); i++) {
            char c = str.at(i);

            if (std::isalpha(c)) {
                buf.push_back(c);
                i++;

                while (i < str.length() && std::isalnum(str.at(i))) {
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;

                if (buf == "ret") {
                    tokens.push_back({.type = TokenType::_return});
                    buf.clear();
                    continue;
                }
                else {
                    std::cerr << "You messed up!" << std::endl;
                    exit(1);
                }
            }

            else if (std::isdigit(c)) {
                buf.push_back(c);
                i++;

                while (i < str.length() && std::isdigit(str.at(i))) {
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;

                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
            }


            else if (c == ';') {
                tokens.push_back({.type = TokenType::semi});
            }

            else if (std::isspace(c)) {
                continue;
            }

            else {
                std::cerr << "You messed up!" << std::endl;
                exit(1);
            }
        }
        return tokens;
    }

private:

    [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
        if (m_index + ahead >= m_src.length) {
            return {};
        } else {
            return m_src.at(m_index);
        }
    }

    const std::string m_src;
    int m_index; // Current Index
}