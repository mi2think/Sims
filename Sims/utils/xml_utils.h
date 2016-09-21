/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   18:08
	filename: 	D:\Code\Sims\Sims\utils\xml_utils.h
	file path:	D:\Code\Sims\Sims\utils
	file base:	xml_utils
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	XML Utils
*********************************************************************/
#ifndef __XML_UTILS_H__
#define __XML_UTILS_H__

#include "sims.h"
#include "rapidxml/rapidxml.hpp"
using namespace rapidxml;

namespace sims
{
	inline bool xml_get_float_attrite(xml_node<>* node, const char* attr_name, string& val)
	{
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if (!attr)
			return false;

		val = attr->value();
		return true;
	}

	inline bool xml_get_float_attrite(xml_node<>* node, const char* attr_name, float& val)
	{
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if (!attr)
			return false;

		val = atof(attr->value());
		return true;
	}

	inline bool xml_get_int_attrite(xml_node<>* node, const char* attr_name, int& val)
	{
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if (!attr)
			return false;

		val = atoi(attr->value());
		return true;
	}

	inline bool xml_get_int32_attrite(xml_node<>* node, const char* attr_name, int32& val)
	{
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if (!attr)
			return false;

		val = atoi(attr->value());
		return true;
	}

	inline bool xml_get_uint32_attrite(xml_node<>* node, const char* attr_name, uint32& val)
	{
		xml_attribute<>* attr = node->first_attribute(attr_name);
		if (!attr)
			return false;

		val = atoi(attr->value());
		return true;
	}
}

#endif
