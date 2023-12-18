#pragma once
#include "DiskPath.h"
#include "FullPath.h"
#include "Separator.h"

namespace Memory
{
	class RelativePathCreator final
	{
	public:
		static FullPath combine(const std::string& from, const std::string& path)
		{
			if (path.find(FullPath::c_disk_div) < path.size())
				return FullPath(path);
			FullPath from_path(from);
			DiskPath result(from_path.disk_path());

			auto list = Separator::split(path, '\\');
			for (const auto& l : list)
			{
				if (l == "..")
					result = DiskPath(result.dir());
				else
					result = DiskPath(result.full_name(), l);
			}

			return FullPath(from_path.mark(), result);
		}
	};
}