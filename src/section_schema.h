#ifndef INICPP_SECTION_SCHEMA_H
#define INICPP_SECTION_SCHEMA_H

#include <vector>
#include <iostream>

#include "exception.h"
#include "option_schema.h"


namespace inicpp
{
	class option_schema;

	class section_schema
	{
	private:
		std::string name_;
		std::vector<option_schema> options_;
		bool mandatory_;
		std::string comment_;

	public:
		section_schema();
		section_schema(const section_schema &source);
		section_schema& operator=(const section_schema &source);
		section_schema(section_schema &&source);
		section_schema& operator=(section_schema &&source);

		section_schema(const std::string &name, bool is_mandatory = true);

		void add_option(const std::string &option_name);
		void add_option(const option_schema &opt);

		std::ostream &operator<<(std::ostream &os);
	};
}

#endif
