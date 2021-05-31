#include "netvar.h"
#include "interfaces.h"

netvar_t* netvar;

void netvar_t::get(void)
{
	m_tables.clear();

	if (!ctx::chlclient)
		return;

	auto prop_class = ctx::chlclient->GetAllClasses();

	if (!prop_class)
		return;

	while (prop_class)
	{
		m_tables.push_back(prop_class->m_pRecvTable);
		prop_class = prop_class->m_pNext;
	}
}

netvar_t::~netvar_t(void)
{
	m_tables.clear();
}

uintptr_t netvar_t::get_offset(const char* table, const char* prop)
{
	auto offset = get_prop(table, prop, 0);

	/* hacky oneliner way to do it */
	return offset ? offset : 0;
}

uintptr_t netvar_t::get_prop(const char* table, const char* prop, recvprop_t** prop_ptr = 0)
{
	auto prop_table = get_table(table);

	if (!prop_table)
		return 0;

	auto offset = get_prop(prop_table, prop, prop_ptr);

	return offset ? offset : 0;
}

recvproxyfn netvar_t::hook_netvar(const char* table, const char* prop, recvproxyfn function)
{
	recvprop_t* prop_ptr;

	get_prop(table, prop, &prop_ptr);

	auto old_function = reinterpret_cast< recvproxyfn >(prop_ptr->m_proxyfn);

	prop_ptr->m_proxyfn = function;

	return old_function;
}

uintptr_t netvar_t::get_prop(recvtable_t* table, const char* prop, recvprop_t** prop_ptr = 0)
{
	auto extra = 0;

	for (auto i = 0; i < table->m_propamt; ++i)
	{
		auto recv_prop = &table->m_props[i];

		auto child = recv_prop->m_table;

		if (child && child->m_propamt)
		{
			auto temp = get_prop(child, prop, prop_ptr);

			if (temp)
			{
				extra += (recv_prop->m_offset + temp);
			}
		}

		if (_stricmp(recv_prop->m_name, prop))
		{
			continue;
		}

		if (prop_ptr)
		{
			*prop_ptr = recv_prop;
		}

		return (recv_prop->m_offset + extra);
	}

	return extra;
}

recvtable_t* netvar_t::get_table(const char* table)
{
	if (m_tables.empty())
		return nullptr;

	for (auto &prop_table : m_tables)
	{
		if (!prop_table)
		{
			continue;
		}

		if (!_stricmp(prop_table->m_tablename, table))
		{
			return prop_table;
		}
	}

	return nullptr;
}
