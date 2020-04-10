#pragma once

namespace modelpp
{
  template<class MODEL>
    struct metadata_t
    {
      using fields_type_ptr = typename fields_type_t::member_ptr<MODEL>::type;
      using fields_map = std::map<std::string, fields_type_ptr>;

      metadata_t(const std::string& name)
        :name{name}, fields{}
      {
      }

      metadata_t(const std::string& name, fields_map fields)
        :name{name}, fields{fields}
      {
      }

      metadata_t& field(const std::string& name, fields_type_ptr ptr)
      {
        fields[name] = ptr;
        return *this;
      }

      std::string name;
      fields_map fields;
    };

  template<class MODEL>
    concept HasMetadata = 
        std::is_convertible_v<decltype(MODEL::metadata)&, metadata_t<MODEL>&>;

}
