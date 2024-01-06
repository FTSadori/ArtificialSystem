#pragma once
#include "Core.h"

namespace Commands
{
	class NetFilesLoader final
	{
	public:
		static void load(Core& core)
		{
			std::hash<std::string> hasher;

			core.net().add_new("google.bom", NetData("RklSRSBJTiBUSEUgSEFMTA==", 0, 0));
			core.net().add_new("google.chmom", NetData("T01PUg==", hasher("password"), 10));
			core.net().add_new("google.vroomvroom", NetData("SXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIElUIFdBUyBBTiBBQ0NJREVOVCE=", 0, 100));
		}
	};
}