#include "iostream"
#include "fstream"
#include "sstream"
#include "optional"
#include "vector"

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};    
}; // Done this to Lex the things, or tokenize it.

/* If we see a letter, we start reading it into a buffer, and when we encounter a space, we can determine
 what the value of the buffer is, if it is a keyword, then we can use it as a keyword
 otherwise we will treat it as a identifier, like variable name, func name etc.
 */
std::vector<Token> tokenize(const std::string& str) {

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

std::string token_to_asm(const std::vector<Token>& tokens) {

    std::stringstream output;
    output << "global _start\n_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);

        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }

    }
    return output.str();
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Incorrect usage, Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return 1;
    }

    std::string contents;
    {
    std::stringstream contents_stream;
    std::fstream input(argv[1], std::ios::in); // Input the file (argv[1]) in Input mode i.e reading mode
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
    } // Destructor, as soon as the fstream will get out of scope of this, it will close

    
    std::vector<Token> tokens = tokenize(contents);
    // std::cout << token_to_asm(tokens) << std::endl; // Now we just need to output it in assembly file

    {
        std::fstream file("out.asm", std::ios::out);
        file << token_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return 0;
}