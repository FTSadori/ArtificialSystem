#pragma once
#include <stdint.h>

namespace Core::Memory
{
	struct BasePermissions
	{
		bool hidden;

		uint8_t read_perm_lvl;
		uint8_t write_perm_lvl;
	};

	struct FilePermissions final : public BasePermissions
	{
		bool is_system_exe;

		uint8_t exec_perm_lvl;
	};

	using hash_t = size_t;

	struct FolderPermissions final : public BasePermissions
	{
		hash_t password_hash;
	};
}
