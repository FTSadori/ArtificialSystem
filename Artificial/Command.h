#pragma once
#include "ICommand.h"
#include <unordered_map>

namespace Commands
{
	class Command final : ICommand
	{
	public:
		Command(const std::string& line);
		
		virtual std::string get(const std::string& _key) const override;
		virtual bool has(const std::string& _key) const override;

	private:
		std::unordered_map<std::string, std::string> m_parameters;
	};
}