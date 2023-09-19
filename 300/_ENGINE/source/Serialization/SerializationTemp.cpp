#include "Serialization/SerializationTemp.h"

std::string to_json(rttr::instance obj)
{
	if (!obj.is_valid())
		return std::string();

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

	to_json_recursive(obj, writer);

	return sb.GetString();
}

void to_json_recursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

	auto prop_list = obj.get_derived_type().get_properties();

	for (auto prop : prop_list)
	{
		if (prop.get_metadata("NO_SERIALIZE"))
			continue;

		rttr::variant prop_val = prop.get_value(obj);
		if (!prop_val)
			continue;

		const auto name = prop.get_name();
		writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length(), false));

		if (!write_variant(prop_val, writer))
			std::cerr << "Cannot serialize property: " << name << std::endl;

		writer.EndObject();
	}
}

bool write_variant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	auto val_type = var.get_type();
	auto wrapped_type = val_type.is_wrapper() ? val_type.get_wrapped_type() : val_type;
	bool is_wrapper = wrapped_type != val_type;

	if (write_atomic_types(is_wrapper ? wrapped_type : val_type, is_wrapper ? var.extract_wrapped_value() : var, writer))
	{
	}
	else if (var.is_sequential_container())
	{
		write_array(var.create_sequential_view(), writer);
	}
	else if (var.is_associative_container())
	{
		write_associative_container(var.create_associative_view(), writer);
	}
	else
	{
		auto child_prop = is_wrapper ? wrapped_type.get_properties() : val_type.get_properties();
		if (!child_prop.empty())
			to_json_recursive(var, writer);
		else
		{
			bool ok = false;
			auto text = var.to_string(&ok);
			if (!ok)
			{
				writer.String(text.c_str());
				return false;
			}

			writer.String(text.c_str());
		}
	}
	return true;
}

bool write_atomic_types(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	if (t.is_arithmetic())
	{
		if (t == rttr::type::get<bool>())
			writer.Bool(var.to_bool());
		else if (t == rttr::type::get<char>())
			writer.Bool(var.to_bool());
		else if (t == rttr::type::get<int8_t>())
			writer.Int(var.to_int8());
		else if (t == rttr::type::get<int16_t>())
			writer.Int(var.to_int16());
		else if (t == rttr::type::get<int32_t>())
			writer.Int(var.to_int32());
		else if (t == rttr::type::get<int64_t>())
			writer.Int64(var.to_int64());
		else if (t == rttr::type::get<uint8_t>())
			writer.Uint(var.to_uint8());
		else if (t == rttr::type::get<uint16_t>())
			writer.Uint(var.to_uint16());
		else if (t == rttr::type::get<uint32_t>())
			writer.Uint(var.to_uint32());
		else if (t == rttr::type::get<uint64_t>())
			writer.Uint64(var.to_uint64());
		else if (t == rttr::type::get<float>())
			writer.Double(var.to_double());
		else if (t == rttr::type::get<double>())
			writer.Double(var.to_double());

		return true;
	}
	else if (t.is_enumeration())
	{
		bool ok = false;
		auto result = var.to_string(&ok);

		if (ok)
			writer.String(var.to_string().c_str());
		else
		{
			ok = false;
			auto val = var.to_uint64(&ok);
			if (ok)
				writer.Uint64(val);
			else
				writer.Null();
		}
		return true;
	}
	else if (t == rttr::type::get<std::string>())
	{
		writer.String(var.to_string().c_str());
		return true;
	}
	
	return false;
}

static void write_array(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartArray();
	for (const auto& item : view)
	{
		if (item.is_sequential_container())
			write_array(item.create_sequential_view(), writer);
		else
		{
			rttr::variant wrapped_var = item.extract_wrapped_value();
			rttr::type val_type = wrapped_var.get_type();

			if (val_type.is_arithmetic() || val_type == rttr::type::get<std::string>() || val_type.is_enumeration())
				write_atomic_types(val_type, wrapped_var, writer);
			else
				to_json_recursive(wrapped_var, writer);
		}
	}
	writer.EndArray();
}

static void write_associative_container(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	static const rttr::string_view key_name("key");
	static const rttr::string_view val_name("value");

	writer.StartArray();
	if (view.is_key_only_type())
	{
		for (auto& item : view)
			write_variant(item.first, writer);
	}
	else
	{
		for (auto& item : view)
		{
			writer.StartObject();
			writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);
			write_variant(item.first, writer);
			writer.String(val_name.data(), static_cast<rapidjson::SizeType>(val_name.length()), false);
			write_variant(item.second, writer);
			writer.EndObject();
		}
	}
	writer.EndArray();
}