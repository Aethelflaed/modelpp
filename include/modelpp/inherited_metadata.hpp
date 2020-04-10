#pragma once

namespace modelpp
{
  template<class MODEL, HasMetadata PARENT>
    struct inherited_metadata_t : public metadata_t<MODEL>
  {
    using parent = PARENT;

    using fields_type_ptr = typename metadata_t<MODEL>::fields_type_ptr;
    //using fields_map = std::map<std::string, fields_type_ptr>;

    inherited_metadata_t(const std::string& name)
      :metadata_t<MODEL>{name}
    {
    }

    inherited_metadata_t(const inherited_metadata_t<MODEL, PARENT>&) = default;

    inherited_metadata_t& field(const std::string& name, fields_type_ptr ptr)
    {
      this->fields.insert_or_assign(name, ptr);
      return *this;
    }
  };

  template<class MODEL>
    concept HasInheritedMetadata = HasMetadata<MODEL> &&
    std::is_convertible_v<typename decltype(MODEL::metadata)::parent*, model*>;
}
