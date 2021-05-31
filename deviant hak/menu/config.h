#pragma once
#include <map>

// simple system to store multiple types inside one class
// only works with default c++ types up to 64 bits of entropy
class value {
public:
	template <typename t = float>
	t get( ) {
		return ( t ) m_value;
	}

	template <typename t = float>
	void set( t in ) {
		m_value = ( double ) in;
	}

private:
	double m_value = 0.0;
};

extern std::map< std::string, value > g_cfg;

/* 
	figure out saving by yourself, it's incredibly simple, actually.
*/