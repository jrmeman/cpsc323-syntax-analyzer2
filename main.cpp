/*  Justin Meman
    CPSC 323

    Assignment 2 - Syntax Analyzer
    Write a syntax analyzer using any top-down parser. The program output displays the analysis
    of each "input" from the file and determines its syntax.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <istream>
#include <locale>
#include <array>
#include <stack>
#include <vector>

using namespace std;

struct tokens {
	string token;
	string lexeme;
	string actualInput;
};

tokens number;
tokens dollar;
string types[6] = {"bool", "char", "int", "float", "double" , "void"};

vector <tokens> lexered;

// Converts strings to table values
int To_Table(string input)
{
	if (input == "id" || input == "Identifier")
		return 0;
	else if (input == "num" || input == "number")
		return 1;
	else if (input == "=")
		return 2;
	else if (input == "+")
		return 3;
	else if (input == "-")
		return 4;
	else if (input == "*")
		return 5;
	else if (input == "/")
		return 6;
	else if (input == "(")
		return 7;
	else if (input == ")")
		return 8;
	else if (input == "key")
		return 9;
	else if (input == "$")
		return 10;
	else if (input == "S")
		return 0;
	else if (input == "A")
		return 1;
	else if (input == "D")
		return 2;
	else if (input == "E")
		return 3;
	else if (input == "E'")
		return 4;
	else if (input == "T")
		return 5;
	else if (input == "T'")
		return 6;
	else if (input == "F")
		return 7;
}

// Analyzes "lexered" vector if it's correct with our defined grammar
void Syntax_Analyzer(vector <tokens> lexered)
{
	string table[8][11];

	table[0][0] = "Rule 1";
	table[0][1] = "Rule 1";
	table[0][7] = "Rule 1";
	table[0][9] = "Rule 3";
	table[1][0] = "Rule 15";
	table[2][9] = "Rule 16";
	table[3][0] = "Rule 4";
	table[3][1] = "Rule 4";
	table[3][7] = "Rule 4";
	table[4][3] = "Rule 5";
	table[4][4] = "Rule 6";
	table[4][8] = "Rule 7";
	table[4][10] = "Rule 7";
	table[5][0] = "Rule 8";
	table[5][1] = "Rule 8";
	table[5][7] = "Rule 8";
	table[6][3] = "Rule 11";
	table[6][4] = "Rule 11";
	table[6][5] = "Rule 9";
	table[6][6] = "Rule 10";
	table[6][8] = "Rule 11";
	table[6][10] = "Rule 11";
	table[7][0] = "Rule 13";
	table[7][1] = "Rule 14";
	table[7][7] = "Rule 12";

	int position = 0;
	int flag = 0;
	stack <string> mystack;
	lexered.push_back(dollar);
	mystack.push(dollar.lexeme);
	mystack.push("S");

	ofstream out;
	out.open("output.txt");
	out << "\tTOKENS\t\t\t     LEXEMES\n\n";

	while (mystack.top() != "$") {
		string t = mystack.top();
		tokens i = lexered.at(position);

		if(i.actualInput == "$")
			break;
		
		if (flag == 0) {
			cout << "Token: " << i.token << " " << "\t\t    Lexeme: " << i.actualInput << endl;
			out << "Token: " << i.token << " " << "\t\t    Lexeme: " << i.actualInput << endl;
			flag = 1;
		}

		// Validity checks to see if top of stack is a terminal
		if (t == "id" || t == "num" || t == "type" || t == "=" || t == "+" || t == "-" || t == "/" || t == "*" || t == "(" || t == ")") {
			// if the top of stack is a terminal, checks if it matches the current position of the input and increments position variable
			if (t == i.lexeme || (t == "id" && i.token == "Identifier") || (t == "num" && i.token == "Integer") || (t == "type" && i.token == "Keyword")) {
				mystack.pop();
				position++;
				flag = 0;
			}
			else {
				cout << "Error Found" << endl;
				out << "Error Found" << endl;
				return;
			}
		}
		
		// Implementation for each rule corresponding to the table
		// Rule 1 is a special case because it can either be sent to Rule 1, 2, or 3
		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 1") {
			if (lexered.at(position + 1).actualInput != "=") {
				cout << "     <Statement> -> <Expression>";
				out << "     <Statement> -> <Expression>";
				mystack.pop();
				mystack.push("E");
			}

			else if(lexered.at(position + 1).actualInput == "=") {
				cout << "     <Statement> -> <Assign>";
				out << "     <Statement> -> <Assign>";
				mystack.pop();
				mystack.push("A");
			}
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 2") {
			cout << "     <Statement> -> <Assign>";
			out << "     <Statement> -> <Assign>";
			mystack.pop();
			mystack.push("A");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 3") {
			cout << "     <Statement> -> <Declaration>";
			out << "     <Statement> -> <Declaration>";
			mystack.pop();
			mystack.push("D");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 4") {
			cout << "     <Expression> -> <Term><E'>";
			out << "     <Expression> -> <Term><E'>";
			mystack.pop();
			mystack.push("E'");
			mystack.push("T");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 5") {
			cout << "     <E'> -> +<Term><E'>";
			out << "     <E'> -> +<Term><E'>";
			mystack.pop();
			mystack.push("E'");
			mystack.push("T");
			mystack.push("+");
		}
		
		else if ((table[To_Table(t)][To_Table(i.lexeme)] == "Rule 6")) {
			cout << "     <E'> -> -<Term><E'>";
			out << "     <E'> -> -<Term><E'>";
			mystack.pop();
			mystack.push("E'");
			mystack.push("T");
			mystack.push("-");
		}

		else if ((table[To_Table(t)][To_Table(i.lexeme)] == "Rule 7")) {
			cout << "     <E'> -> epsilon";
			out << "     <E'> -> epsilon";
			mystack.pop();
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 8") {
			cout << "     <Term> -> <Factor><T'>";
			out << "     <Term> -> <Factor><T'>";
			mystack.pop();
			mystack.push("T'");
			mystack.push("F");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 9") {
			cout << "     <T'> -> *<Factor><T'>";
			out << "     <T'> -> *<Factor><T'>";
			mystack.pop();
			mystack.push("T'");
			mystack.push("F");
			mystack.push("*");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 10") {
			cout << "     <T'> -> /<Factor><T'>";
			out << "     <T'> -> /<Factor><T'>";
			mystack.pop();
			mystack.push("T'");
			mystack.push("F");
			mystack.push("/");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 11") {
			cout << "     <T'> -> epsilon";
			out << "     <T'> -> epsilon";
			mystack.pop();
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 12") {
			cout << "     <Factor> -> ( <Expression> )";
			out << "     <Factor> -> ( <Expression> )";
			mystack.pop();
			mystack.push(")");
			mystack.push("E");
			mystack.push("(");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 13") {
			cout << "     <Factor> -> id";
			out << "     <Factor> -> id";
			mystack.pop();
			mystack.push("id");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 14") {
			cout << "     <Factor> -> num";
			out << "     <Factor> -> num";
			mystack.pop();
			mystack.push("num");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 15") {
			cout << "     <Assign> -> id = <Expression>";
			out << "     <Assign> -> id = <Expression>";
			mystack.pop();
			mystack.push("E");
			mystack.push("=");
			mystack.push("id");
		}

		else if (table[To_Table(t)][To_Table(i.lexeme)] == "Rule 16") {
			cout << "     <Declaration> -> type id";
			out << "     <Declaration> -> type id";
			mystack.pop();
			mystack.push("id");
			mystack.push("type");
		}

		// Special case rule to fix stack in order to syntactically analyze a new line
		else if(i.lexeme == ";") {
			cout << "     --New Line--" << endl;
			out << "     --New Line--" << endl;
			int j = mystack.size() - 1;
			for(int i = 0; i < j; i++){
				mystack.pop();
			}
			mystack.push("S");
			position++;
			flag = 0;
		}

		else {
			cout << "Error Found" << endl;
			out << "Error Found" << endl;
			return;
		}

		cout << endl;
		out << endl;
	}

	cout << "Syntactically Correct" << endl;
	out << "Syntactically Correct" << endl;

	out.close();
}

// Function to push structs to vector and help with the syntax analyzer
void lexer_output(string token, string lexeme, string actual) {
	tokens temp {token, lexeme, actual};

	if (token == "Identifier") {
		lexered.push_back(temp);
	}

	else if (token == "Real" || token == "Integer") {
		lexered.push_back(temp);
	}

	else if (token == "Keyword") {
		lexered.push_back(temp);
	}

	else if (token == "Operator" && lexeme == "=") {
		lexered.push_back(temp);
	}

	else if (token == "Operator" && lexeme == "+") {
		lexered.push_back(temp);
	}

	else if (token == "Operator" && lexeme == "-") {
		lexered.push_back(temp);
	}

	else if (token == "Operator" && lexeme == "/") {
		lexered.push_back(temp);
	}

	else if (token == "Operator" && lexeme == "*") {
		lexered.push_back(temp);
	}

	else if (token == "Separator" && lexeme == "(") {
		lexered.push_back(temp);
	}

	else if (token == "Separator" && lexeme == ")") {
		lexered.push_back(temp);
	}
	
	else if (token == "Separator" && lexeme == ";") {
		lexered.push_back(temp);
	}
}

// Lexer copy and pasted from assignment 1
// Small adjustments made to help with syntax analyzer
void Lexer() {
    ifstream in;

    string s, w, num;
    int state = 0;
    int column;
    bool flag = true;

    // Defining FSM - mimics the FSM defined in the documentation
    int fsm[10][8] = {  {1, 3, 7, 8, 0, 0, 8, 9}, 
                        {1, 1, 2, 2, 1, 2, 2, 2}, 
                        {0, 0, 0, 0, 0, 0, 0, 0}, 
                        {5, 3, 5, 5, 5, 5, 4, 5}, 
                        {6, 4, 6, 6, 6, 6, 6, 6}, 
                        {0, 0, 0, 0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0, 0, 0, 0}, 
                        {9, 9, 9, 9, 9, 9, 9, 0} };
    
    // Array of operators
    char op[8] = {'+', '-', '*', '/', '=', '%', '<', '>'};

    // Array of separators
    char sep[10] = {'[', ']', '(', ')', '{', '}', ',', '.', ';', ':'};

    // Array of keywords
    string keywords[21] = {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", 
        "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};

    in.open("input.txt");

    if(!in.is_open()) {
        cout << "ERROR: File cannot be opened/found\n";
        system("pause");
        exit(1);
    }

    cout << endl;
    cout << "\tTOKENS\t\t\t     LEXEMES\n\n";

    while(getline(in, s)) {
        for(char c : s) {
            // Operator check
            if(state == 0) {
                for(int i = 0; i < 8; i++) {
                    // Operator column
                    column = 2;
                    if(c == op[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "OPERATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Operator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;          
                }
            }

            // Separator check
            if(state == 0){
                for(int i = 0; i < 10; i++) {
                    // Separator column
                    column = 3;
                    if(c == sep[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "SEPARATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Separator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;
                }
            }

            // Checks for comment blocks in txt file
            if(c == '!' && state == 0) {
                // Changing states
                state = 9;
            }
            else if(c == '!' && state == 9) {
                // Revert back to state 0 after end of comment block
                state = 0;
            }

            // Character check
            column = 0;
            if(isalpha(c) && state == 0) {
                state = fsm[state][column];
                // Concatonate char to w string
                w += c;
            }

            else if((isalpha(c) || isdigit(c) || c == '$' || c == '_') && state == 1) {
                // Concatonate char to w string
                w += c;
            }

            else if(!isalpha(c) && !isdigit(c) && c != '$' && c != '_' && state == 1) {
                // Keyword check
                for(int i = 0; i < 21; i++) {
                    if(w == keywords[i]) {
                        // cout << "KEYWORD\t\t\t=\t\t" << w << endl;
						lexer_output("Keyword", "key", w);
                        flag = false;
                    }
                }

                // If not keyword, w is an identifier
                if(flag != false) {
                    // cout << "IDENTIFIER\t\t=\t\t" << w << endl;
					lexer_output("Identifier", "id", w);
                }

                // Next 2 for loops checks to see if !isalpha(c) && !isdigit(c) && c != '$' && c != '_' 
                // is an operator or separator
                for(int i = 0; i < 8; i++) {
                    // Operator column
                    column = 2;
                    if(c == op[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "OPERATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Operator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;          
                }

                for(int i = 0; i < 10; i++) {
                    // Separator column
                    column = 3;
                    if(c == sep[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "SEPARATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Separator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;
                }

                state = 0;
                w = "";
                flag = true;
            }

            // Integer check
            column = 1;
            if(isdigit(c) && state == 0) {
                state = fsm[state][column];
                // Concatonate char to num string
                num += c;
            }

            else if(isdigit(c) && state == 3) {
                // Concatonate char to string
                num += c;
            }

            else if(!isdigit(c) && c != '.' && state == 3) {
                // cout << "INTEGER\t\t\t=\t\t" << num << endl;
				lexer_output("Integer", "num", num);

                // Next 2 for loops checks to see if !isdigit(c) && c != '.' is an operator or separator
                for(int i = 0; i < 8; i++) {
                    // Operator column
                    column = 2;
                    if(c == op[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "OPERATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Operator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;          
                }

                for(int i = 0; i < 10; i++) {
                    // Separator column
                    column = 3;
                    if(c == sep[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "SEPARATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Separator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;
                }

                state = 0;
                num = "";
            }

            // Real number check
            else if(c == '.' && state == 3) {
                column = 6;
                state = fsm[state][column];
                num += c;
            }

            else if(isdigit(c) && state == 4) {
                num += c;
            }

            else if(!isdigit(c) && state == 4) {
                // cout << "REAL NUMBER\t\t=\t\t" << num << endl;
				lexer_output("Real", "num", num);

                // Next 2 for loops checks to see if !isdigit(c) is an operator or separator
                for(int i = 0; i < 8; i++) {
                    // Operator column
                    column = 2;
                    if(c == op[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "OPERATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Operator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;          
                }

                for(int i = 0; i < 10; i++) {
                    // Separator column
                    column = 3;
                    if(c == sep[i]) {
                        // Changing states
                        state = fsm[state][column];
                        // cout << "SEPARATOR\t\t=\t\t" << c << endl;
						string s(1, c);
						lexer_output("Separator", s, s);
						s = "";
                    }
                    // Revert back to state 0 after output
                    state = 0;
                }

                state = 0;
                num = "";
            }
        }
    }

    in.close();
}


int main() {
	// Initialized dollar token
	dollar.token = "EndSymbol";
	dollar.lexeme = "$";
	dollar.actualInput = "$";

	Lexer();
	Syntax_Analyzer(lexered);
}