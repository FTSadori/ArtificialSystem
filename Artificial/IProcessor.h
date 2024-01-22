#pragma once
#include <vector>

namespace Mova
{
	class IProcessor
	{
	public:
		virtual std::vector<float>& registers() = 0;
		virtual ~IProcessor() = default;
	};
}