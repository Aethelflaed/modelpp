#pragma once

namespace modelpp
{
  template<class MODEL>
    struct metadata
    {
      using members_ptr_t = typename field_types::member_ptr<MODEL>::type;
      using members_ptr_map = std::map<std::string, members_ptr_t>;

      metadata(const std::string& name)
        :name{name}, fields{}
      {
      }

      metadata(const std::string& name, members_ptr_map fields)
        :name{name}, fields{fields}
      {
      }

      metadata& field(const std::string& name, members_ptr_t ptr)
      {
        fields.insert_or_assign(name, ptr);
        return *this;
      }

      std::string name;
      members_ptr_map fields;
    };

  template<class MODEL>
    concept HasMetadata = 
        std::is_convertible_v<decltype(MODEL::metadata)&, metadata<MODEL>&>;
}
