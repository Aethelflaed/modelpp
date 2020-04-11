#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include "metadata.hpp"

namespace modelpp
{
  using fields_map =      std::unordered_map<std::string, field_types_t>;
  using field_change =    std::pair<field_types_t, field_types_t>;
  using fields_changes =  std::unordered_map<std::string, field_change>;

  class model
  {
    public:
      bool changed() const
      {
        return !changes_.empty();
      }

      bool changed(const std::string& field_name) const
      {
        return changes_.contains(field_name);
      }

      const fields_changes& changes()
      {
        return changes_;
      }

      template<class FIELD_TYPE, class VALUE_TYPE>
        void change(const std::string& field, FIELD_TYPE* field_addr, VALUE_TYPE value)
        {
          if (auto it = changes_.find(field); it != changes_.end())
          {
            it->second.second = value;
          }
          else
          {
            changes_.emplace(field, std::make_pair(*field_addr, value));
          }
          *field_addr = value;
        }

      template<HasMetadata MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          const auto& meta = MODEL::metadata;

          for (const auto& field : meta.fields)
          {
            if (auto it = data.find(field.first); it != data.end())
            {
              auto lambda =
                [&m, &field]
                (auto&& arg)
                {
                  using type = member_ptr_t<MODEL, decltype(arg)>;

                  if (std::holds_alternative<type>(field.second))
                  {
                    auto field_ptr = std::get<type>(field.second);
                    m->*field_ptr = arg;
                  }
                };

              std::visit(lambda, it->second);
            }
          }

          changes_.clear();
        }

      template<HasMetadata MODEL>
        fields_map data(MODEL* m)
        {
          return data(m, {});
        }

      template<HasMetadata MODEL>
        fields_map data(MODEL* m, fields_map data)
        {
          const auto& meta = MODEL::metadata;

          for (const auto& field : meta.fields)
          {
            auto lambda =
              [&m, &data, &field]
              (auto arg)
              {
                data[field.first] = m->*arg;
              };

            std::visit(lambda, field.second);
          }

          return data;
        }

    private:
      fields_changes changes_;
  };

  template <class MODEL>
    concept IsModel = std::is_convertible_v<MODEL*, model*> &&
    HasMetadata<MODEL>;
}

