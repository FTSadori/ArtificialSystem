#pragma once
#include <string>
#include <sstream>

class Parser final
{
public:

	template <typename T>
	static T from_string(const std::string& s)
	{
		std::stringstream ss(s);
		T t = T();
		ss >> t;
		return t;
	}

	template <typename T>
	static std::string to_string(T t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	static int from_bits(const std::string& s)
	{
		int mul = 1;
		int sum = 0;
		for (int i = s.size() - 1; i >= 0; --i)
		{
			sum += (s[i] - '0') * mul;
			mul *= 2;
		}
		return sum;
	}

	static std::string to_bits(int n)
	{
		std::string s = "";
		while (n > 0)
		{
			s = char('0' + (n % 2)) + s;
			n /= 2;
		}
		return s;
	}
};
