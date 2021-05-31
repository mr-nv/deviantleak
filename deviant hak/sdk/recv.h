#pragma once
#include "common.h"

// ================================================
//  valve sdk version of recvprop_t struct
// ================================================

class variant_t
{
public:
	union
	{
		float m_float;
		long m_int;
		char* m_char;
		void* m_data;
		float m_vector[3];
	};
};

struct recvprop_t;

class recvproxy_t
{
public:
	const recvprop_t* m_recvprop;
	variant_t m_value;
	int m_element;
	int m_object;
};

typedef void(*recvproxyfn)(const recvproxy_t* data, void* _struct, void* out);

struct recvtable_t
{
	recvprop_t* m_props;
	int m_propamt;
	void* m_decoder;
	char* m_tablename;
	bool m_initialized;
	bool m_mainlist;
};

struct recvprop_t
{
	char* m_name;
	int m_type;
	int m_flags;
	int m_bufsize;
	bool m_inarray;
	const void* m_data;
	recvprop_t* m_arrayprop;
	void* m_arrayproxy;
	void* m_proxyfn;
	void* m_dtproxyfn;
	recvtable_t* m_table;
	int m_offset;
	int m_stride;
	int m_elements;
	const char* m_parentname;
};

struct clientclass_t
{
	void* m_createfn;
	void* m_createeventfn;
	char* m_netname;
	recvtable_t* m_table;
	clientclass_t* m_next;
	int m_classid;
};