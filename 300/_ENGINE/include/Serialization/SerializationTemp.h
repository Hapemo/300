#include "pch.h"
#include "rttr/type.h"
#include "prettywriter.h"
#include "document.h"

std::string to_json(rttr::instance obj);

void to_json_recursive(const rttr::instance& obj2, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

bool write_variant(const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

bool write_atomic_types(const rttr::type& t, const rttr::variant& var, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

static void write_array(const rttr::variant_sequential_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

static void write_associative_container(const rttr::variant_associative_view& view, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
