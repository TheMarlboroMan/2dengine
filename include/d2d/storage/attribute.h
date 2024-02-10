#pragma once

#include <string>

namespace d2d { namespace storage {

//Map nodes might have attributes. The attributes will be stored in a map
//of std::string to attribute, which represents any basic type in a sort
//of detestable union way.
struct attribute {

	enum types{type_int, type_bool, type_str};

	attribute(const std::string& _val) {

		strval=_val;
		type=type_str;
	}

	attribute(int _val) {

		intval=_val;
		type=type_int;
	}

	attribute(bool _val) {

		boolval=_val;
		type=type_bool;
	}

	bool        is_int() const {return type_int==type;}
	bool        is_bool() const {return type_bool==type;}
	bool        is_string() const {return type_str==type;}

	int         get_int() const {return intval;}
	bool        get_bool() const {return boolval;}
	const std::string   get_string() const {return strval;}

	private:

	int         intval{0}, type{0};
	bool        boolval{false};
	std::string strval;
};

}}
