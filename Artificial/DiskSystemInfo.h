#pragma once
#include "DiskInfo.h"
#include <map>

namespace Memory
{
	struct DiskSystemInfo final : ILoadable
	{
        std::vector<std::pair<std::string, DiskInfo>> disks_info;
        size_t main_disk = 0;

        DiskSystemInfo() {}

        DiskSystemInfo(const std::vector<std::pair<std::string, DiskInfo>>& _info)
            : disks_info(_info) {}

        virtual DataQueue get_as_data() const override
        {
            DataQueue data;

            data.push<size_t>(main_disk);
            data.push<size_t>(disks_info.size());
            
            for (const auto&[mark, info] : disks_info)
            {
                DataQueue info_data = info.get_as_data();
                data.concat(info_data);
            }

            return data;
        }

        virtual void load_from_data(DataQueue& _data) override
        {
            disks_info.clear();

            main_disk = _data.pop<size_t>();
            size_t size = _data.pop<size_t>();

            for (size_t i = 0; i < size; ++i)
            {
                DiskInfo info;
                info.load_from_data(_data);
                disks_info.emplace_back(info.mark, info);
            }
        }
    };
}