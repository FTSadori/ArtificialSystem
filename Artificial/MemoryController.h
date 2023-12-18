#pragma once
#include "IController.h"
#include "DiskSystem.h"
#include <memory>

namespace Commands
{
	class MemoryController final : public IController
	{
	public:
		MemoryController(Memory::DiskSystem& _memory)
			: m_memory(_memory) {}

		virtual bool has_option(const std::string& name) override
		{
			return m_options.find(name) != m_options.end();
		}

		virtual void add_option(const std::string& name, std::unique_ptr<AbstractControllerOption>&& option) override
		{
			m_options.emplace(name, option.release());
		}

		virtual std::string execute(const ICommand& command, const User& sender) override
		{
			return m_options.at(command.get("name"))->execute(command, sender);
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<AbstractControllerOption>> m_options;
		Memory::DiskSystem& m_memory;
	};
}