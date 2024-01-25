#pragma once
#include <vector>
#include <string>

namespace Mova
{
	class IProcessor
	{
	public:
		virtual std::vector<float>& registers() = 0;
		virtual double process(const std::vector<std::string>& code) = 0;
		virtual ~IProcessor() = default;
	};
}