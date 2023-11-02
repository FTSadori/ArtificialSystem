#pragma once
#include <vector>
#include <string>
#include "Permissions.h"
#include "ILoadable.h"

namespace Core::Memory
{
	struct FileInfo final : public ILoadable
	{
		FileInfo() {}
		FileInfo(Permissions _permissions, time_t _creation_time, time_t _changing_time)
			: permissions(_permissions), creation_time(_creation_time), changing_time(_changing_time) {}

		Permissions permissions = Permissions();

		time_t creation_time = 0;
		time_t changing_time = 0;

		virtual DataQueue get_as_data() const override
		{
			DataQueue data;
			data.push(permissions);
			data.push(creation_time);
			data.push(changing_time);
			return data;
		}

		virtual void load_from_data(DataQueue& _data) override
		{
			permissions = _data.pop<Permissions>();
			creation_time = _data.pop<time_t>();
			changing_time = _data.pop<time_t>();
		}
	};
}