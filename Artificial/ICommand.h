#pragma once
#include <string>

namespace Commands
{
	class ICommand
	{
	public:
		virtual bool has(const std::string& _key) const = 0;
		virtual std::string get(const std::string& _key) const = 0;
		virtual std::string getwholeline() const = 0;
		virtual ~ICommand() = default;
	};
}
