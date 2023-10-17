#pragma once
#include <stdint.h>

namespace Core::Memory
{
	struct FilePermissions final
	{
		bool hidden = false;

		uint8_t read_perm_lvl = 0;
		uint8_t write_perm_lvl = 0;
		uint8_t exec_perm_lvl = 0;

		bool is_system_exe = false;

		FilePermissions() {}
		FilePermissions(bool _hidden, uint8_t _read_perm_lvl, uint8_t _write_perm_lvl,uint8_t _exec_perm_lvl, bool _is_system_exe)
			: hidden(_hidden), read_perm_lvl(_read_perm_lvl), write_perm_lvl(_write_perm_lvl),
			  exec_perm_lvl(_exec_perm_lvl), is_system_exe(_is_system_exe) {}
	};

	using hash_t = size_t;

	struct FolderPermissions final
	{
		bool hidden = false;

		uint8_t read_perm_lvl = 0;
		uint8_t write_perm_lvl = 0;

		hash_t password_hash = 0;

		FolderPermissions() {}
		FolderPermissions(bool _hidden, uint8_t _read_perm_lvl, uint8_t _write_perm_lvl, hash_t _password_hash)
			: hidden(_hidden), read_perm_lvl(_read_perm_lvl), 
			  write_perm_lvl(_write_perm_lvl), password_hash(_password_hash) {}
	};
}
