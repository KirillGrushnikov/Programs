#include "Calculate.h"
#include <stack>
#include <math.h>

int GetPrecision(double value, unsigned int Precision)
{
	int sch = 1;
	if (value < 0) sch++;
	while ((int)value)
	{
		value /= 10;
		sch++;
	}
	return sch + Precision;
}

int GetPriority(char sing)
{
	if (sing == '+' || sing == '-') return 1;
	if (sing == '*' || sing == '/' || sing == '%' || sing == '^') return 2;
	else return 0;
}

bool Math(std::stack <double>& numbers, std::stack <char>& sings)
{
	const double PI = acos(-1);

	while (!sings.empty() && numbers.size() != 1 && sings.top() != '(')
	{
		double first_num = numbers.top();
		numbers.pop();
		double second_num = numbers.top();
		numbers.pop();
		char sing = sings.top();
		sings.pop();
		switch (sing)
		{
		case '+':
			numbers.push(first_num + second_num);
			break;
		case '-':
			numbers.push(second_num - first_num);
			break;
		case '*':
			numbers.push(first_num * second_num);
			break;
		case '/':
			if (first_num == 0)
				return false;
			numbers.push(second_num / first_num);
			break;
		case '^':
			numbers.push(pow(second_num, first_num));
			break;
		case '%':
			numbers.push(int(second_num) % int(first_num));
			break;
		default:
			return false;
			break;
		}
	}
	return true;
}

std::string DeleteSpace(std::string example)
{
	std::string str;
	for (char c : example)
	{
		if (c == ' ') 
			continue;

		str += c;
	}

	return str;
}

std::string Calculate(std::string example, unsigned int Precision)
{
	if (example.empty())
		return "Error";
	example = DeleteSpace(example);

	int bkt = 0;
	bool flag = true;
	double Result = 0;
	std::stack <double> numbers;
	std::stack <char> sings;

	std::string number = "0123456789";
	std::string operations = "-+%/*^";
	std::string digit;

	//Заполняем стеки и считаем
	size_t lastPos = 0;
	for (size_t i = 0; i < example.size(); i++)
		for (size_t i_oper = 0; i_oper < operations.size(); i_oper++)
			if (example[i] == operations[i_oper])
			{
				if (sings.empty()) // если стек символов пуст
				{
					if (example[0] == '-' && flag)
					{
						example.insert(example.begin(), '0');
						flag = false;
					}
					else
					{
						sings.push(operations[i_oper]); // добовляем символ в стек
						size_t pos = example.find(sings.top(), lastPos); // ищем позицию этого символа чтобы скопировать число
						if (pos - lastPos > 1 || lastPos == 0)
							numbers.push(atof(example.substr(lastPos, pos).c_str())); // добовляем числа в стек
						lastPos = pos + 1; // сдвигаем каретку за символ
					}
					break;
				}
				else // если стек не пуст
				{
					if (GetPriority(operations[i_oper]) > GetPriority(sings.top())) // если приоритет знака в стеке меньше чем тот на котором каретка
					{
						// всё тоже самое что и при пустом стеке
						sings.push(operations[i_oper]);
						size_t pos = example.find(sings.top(), lastPos);
						if (lastPos != pos)
							numbers.push(atof(example.substr(lastPos, pos).c_str()));
						lastPos = pos + 1;
						break;
					}
					if (GetPriority(operations[i_oper]) <= GetPriority(sings.top())) // если приоритет знака в стеке больше чем тот на котором каретка
					{
						size_t pos = example.find(operations[i_oper], lastPos);
						if (lastPos != pos)
							numbers.push(atof(example.substr(lastPos, pos).c_str()));
						lastPos = pos + 1;
						if (Math(numbers, sings)) // считаем и если всё ок присваеваем результат
							Result = numbers.top();
						else
							return "Error";
						sings.push(operations[i_oper]);
						break;
					}
				}
			}
			else
			{
				if (example[i] == '(')
				{
					bkt++;
					if (example[i + 1] == '-')
						example.insert(example.begin() + i + 1, '0');

					bool s = false;
					if (i == 0)
						s = true;
					else
					{
						for (size_t j_oper = 0; j_oper < operations.size(); j_oper++)
							if (!s && example[i - 1] == operations[j_oper])
							{
								s = true;
								break;
							}
					}
					if (!s)
						return "Error";
					sings.push('('); // добовляем символ в стек
					size_t pos = example.find(sings.top(), lastPos); // ищем позицию этого символа чтобы скопировать число
					//numbers.push(atof(example.substr(lastPos, pos).c_str()));
					lastPos = pos + 1; // сдвигаем каретку за символ
					break;
				}
				if (example[i] == ')')
				{
					if (bkt == 0 || sings.empty())
						return "Error";
					while (sings.top() != '(')
					{
						size_t pos = example.find(')', lastPos);
						if (example.substr(lastPos, pos) != ")" && lastPos != pos)
							numbers.push(atof(example.substr(lastPos, pos).c_str()));
						lastPos = pos + 1; // сдвигаем каретку за символ
						if (Math(numbers, sings)) // считаем и если всё ок присваеваем результат
							Result = numbers.top();
						else
							return "Error";
					}
					sings.pop();
					bkt--;
					break;
				}

			}

	if (lastPos < example.size())
		numbers.push(atof(example.substr(lastPos, example.size()).c_str())); //забираем самое правое число со строки

	if (bkt > 0)
		return "Error";

	if (Math(numbers, sings))
	{
		Result = numbers.top();
		std::string r = std::to_string(Result);
		r.resize(GetPrecision(Result, Precision));
		return r;
	}
	else
		return "Error";
}
