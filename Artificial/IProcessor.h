#pragma once
#include <vector>
#include <string>

namespace Mova
{
	class IProcessor
	{
	public:
		virtual std::vector<float>& registers() = 0;
		virtual std::vector<float>& process(const std::vector<std::string>& code, const std::vector<double>& input) = 0;
		virtual ~IProcessor() = default;
	};
}