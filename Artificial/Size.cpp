#include "Size.h"

namespace GUI
{
	bool operator!=(const Size& a, const Size& b)
	{
		return (a.columns != b.columns) || (a.rows != b.rows);
	}
}