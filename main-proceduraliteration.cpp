#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <istream>
#include <locale>
#include <array>

using namespace std;

void readFile()
{
	string text;
	string line;
	string s;
	string word;
	char prev = ' ';

	ifstream input;
	int state = 0;
	int newstate = 0;
	int flag = 0;
	int check = 0;

	int fsm[3][2] = { {2,3},{1, 1},{1,1}};
	string keywords[18] = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "function" };\
    
	input.open("input.txt");
	if (input.is_open())
	{
		while (getline(input, s))
		{

			for (char c : s)
			{
					switch (c)
					{
					case '*':
					case '+':
					case '-':
					case '=':
					case '/':
					case '>':
					case '<':
					case '%':
						state = fsm[0][0];
						if (isalpha(prev) || isdigit(prev) || prev == '$')
						{
							cout << "IDENTIFIER     " << "=     " << word << " " << endl;
							word = "";
						}
						if (flag == 1 && word.find('.') != string::npos && isdigit(prev))
						{
							cout << "real found " << word << " " << endl;
							word == "";
							flag = 0;
						}
						//cout << "test";
						break;
					case '\'':
					case '(':
					case ')':
					case '{':
					case '}':
					case '[':
					case ']':
					case ',':
					case '.':
						if (flag == 1)
						{
							break;
						}
					case ':':
					case ';':
					case '!':
						state = fsm[0][1];
						if (isalpha(prev) || isdigit(prev) || prev == '$')
						{
							cout << "IDENTIFIER     " << "=     " << word << " " << endl;
							word = "";
						}

						break;
					case ' ':
						if (flag == 1 && word.find('.') != string::npos && isdigit(prev))
						{
							cout << "real found " << word << " " << endl;
							word == "";
							flag = 0;
						}
					}

			

				

				if (isalpha(c) || c == '$')
				{
					word += c;
					for (int i = 0; i < 18; i++)
					{
						if (word == keywords[i])
						{
							state = 4;
						}
					}
				}


				if (prev == ' ' && isdigit(c))
				{
					word += c;
					flag = 1;
				}
				if (isdigit(c) && isdigit(prev) && flag == 1)
				{
					word += c;
				}
				if (c == '.' && isdigit(prev) && flag == 1)
				{
					word += c;
				}
				if (prev == '.' && isdigit(c) && flag == 1)
				{
					word += c;
				}

			

				else if(isdigit(c) && isalpha(prev))
				{
					word += c;
					//newstate = 5;
				}
				else if (c == ' ' && (isalpha(prev) || isdigit(prev) || prev == '$') && word != "" && flag == 0)
				{
					newstate = 5;
				}

				

				if (state == 2)
				{
					cout << "OPERATOR       " << "=     " << c << " " << endl;
					state = 0;
				}
				if (state == 3)
				{
					cout << "SEPERATOR      " << "=     " << c << " " << endl;
					state = 0;
				}
				if (state == 4)
				{
					cout << "KEYWORD        " << "=     " << word << " " << endl;
					word = "";
					state = 0;
				}
				if (newstate == 5)
				{
					cout << "IDENTIFIER     " << "=     " << word << " " << endl;
					newstate = 0;
					word = "";
				}


				prev = c;
			}

		}

	}

	input.close();
}


int main() {
	readFile();


}