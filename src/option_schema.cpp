#include "option_schema.h"
#include "string_utils.h"

namespace inicpp
{
	option_schema::option_schema(const option_schema &source)
	{
		this->operator =(source);
	}

	option_schema &option_schema::operator=(const option_schema &source)
	{
		type_ = source.type_;

		switch (type_) {
		case option_type::boolean_e:
			params_ = copy_schema<boolean_ini_t>(source.params_);
			break;
		case option_type::enum_e:
			params_ = copy_schema<enum_ini_t>(source.params_);
			break;
		case option_type::float_e:
			params_ = copy_schema<float_ini_t>(source.params_);
			break;
		case option_type::signed_e:
			params_ = copy_schema<signed_ini_t>(source.params_);
			break;
		case option_type::string_e:
			params_ = copy_schema<string_ini_t>(source.params_);
			break;
		case option_type::unsigned_e:
			params_ = copy_schema<unsigned_ini_t>(source.params_);
			break;
		case option_type::invalid_e:
			// never reached
			throw invalid_type_exception("Invalid option type");
			break;
		}

		return *this;
	}
	
	option_schema::option_schema(option_schema &&source)
	{
		this->operator =(std::move(source));
	}
	
	option_schema &option_schema::operator=(option_schema &&source)
	{
		if (this != &source) {
			type_ = source.type_;
			params_ = std::move(source.params_);
		}
		return *this;
	}

	const std::string &option_schema::get_name() const
	{
		return params_->name;
	}

	option_type option_schema::get_type() const
	{
		return type_;
	}

	bool option_schema::is_list() const
	{
		return params_->type == option_item::list;
	}

	const std::string &option_schema::get_default_value() const
	{
		return params_->default_value;
	}

	bool option_schema::is_mandatory() const
	{
		return params_->requirement == item_requirement::mandatory;
	}

	const std::string &option_schema::get_comment() const
	{
		return params_->comment;
	}
	
	bool option_schema::validate_option(option &opt) const
	{
		if (params_->type == option_item::single && opt.is_list()) {
			throw validation_exception("List given, single value expected");
		} else if (params_->type == option_item::list && !opt.is_list()) {
			throw validation_exception("Single value given, list expected");
		}

		// if option type doesn't match, parse it to proper one
		if (opt.get_type() != type_) {
			parse_option_items(opt);
		}

		// validate range using provided validator
		validate_option_items(opt);

		return true;
	}

	void option_schema::validate_option_items(option &opt) const
	{
		// load value and call validate function on it
		switch (type_) {
		case option_type::boolean_e:
			break;
		case option_type::enum_e:
			break;
		case option_type::float_e:
			break;
		case option_type::signed_e:
			validate_typed_option_items<signed_ini_t>(opt.get_list<signed_ini_t>());
			break;
		case option_type::string_e:
			break;
		case option_type::unsigned_e:
			break;
		case option_type::invalid_e:
			// never reached
			throw invalid_type_exception("Invalid option type");
			break;
		}
	}

	void option_schema::parse_option_items(option &opt) const
	{
		switch (type_) {
		case option_type::boolean_e:
			break;
		case option_type::enum_e:
			break;
		case option_type::float_e:
			break;
		case option_type::signed_e:
			opt.set_list<signed_ini_t>(parse_typed_option_items<signed_ini_t>(
				opt.get_list<string_ini_t>(), string_utils::parse_signed_type)
			);
			break;
		case option_type::string_e:
			break;
		case option_type::unsigned_e:
			break;
		case option_type::invalid_e:
			// never reached
			throw invalid_type_exception("Invalid option type");
			break;
		}
	}

	std::ostream &operator<<(std::ostream &os, const option_schema &opt_schema)
	{
		// write comment
		auto comment_lines = string_utils::split(opt_schema.get_comment(), '\n');
		for (auto &comment_line : comment_lines) {
			os << ";" << comment_line << std::endl;
		}

		// optional/mandatory and single/list
		std::string info_line = opt_schema.is_mandatory() ? "mandatory" : "optional";
		info_line += ", ";
		info_line += opt_schema.is_list() ? "list" : "single";
		os << ";<" << info_line << ">" << std::endl;

		// write name and default value
		os << opt_schema.get_name() << " = " << opt_schema.get_default_value() << std::endl;

		return os;
	}
}
