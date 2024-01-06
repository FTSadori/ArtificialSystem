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

			core.net().add_new("google.bom", NetData("RklSRSBJTiBUSEUgSExM", "PRAY.txt", 0, 0));
			core.net().add_new("google.chmom", NetData("T01PUg==", "mar.txt", hasher("password"), 10));
			core.net().add_new("google.vroomvroom", NetData("SXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIEl0IHdhcyBhbiBhY2NpZGVudCEgSXQgd2FzIGFuIGFjY2lkZW50ISBJdCB3YXMgYW4gYWNjaWRlbnQhIElUIFdBUyBBTiBBQ0NJREVOVCE=", "SHE", 0, 100));
		}
	};
}