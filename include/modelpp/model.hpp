#pragma once

#include <string>
#include <map>
#include <utility>
#include <type_traits>

#include "metadata.hpp"

namespace modelpp
{
  class model
  {
    public:
      bool changed() const
      {
        return changes_.empty();
      }

      bool changed(const std::string& field_name) const
      {
        return changes_.contains(field_name);
      }

      const std::map<std::string, std::pair<fields_type, fields_type>>& changes()
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
            changes_.emplace(std::make_pair(field, std::make_pair(*field_addr, value)));
          }
          *field_addr = value;
        }


      template<HasMetadata MODEL>
        void load(MODEL* m, const std::map<std::string, fields_type>& data)
        {
          const metadata_t<MODEL>& meta = MODEL::metadata;
          for (const auto& field : meta.fields)
          {
            if (auto it = data.find(field.first); it != data.end())
            {
              auto lambda =
                [&m, &field]
                (auto&& arg)
                {
                  using type = typename std::remove_cvref<decltype(arg)>::type;
                  if (std::holds_alternative<type MODEL::*>(field.second))
                  {
                    auto field_ptr = std::get<type MODEL::*>(field.second);
                    m->*field_ptr = arg;
                  }
                };

              std::visit(lambda, it->second);
            }
          }

          changes_.clear();
        }

      template<HasMetadata MODEL>
        std::map<std::string, fields_type> save(MODEL* m)
        {
          return save(m, std::map<std::string, fields_type>());
        }

      template<HasMetadata MODEL>
        std::map<std::string, fields_type> save(MODEL* m, std::map<std::string, fields_type> data)
        {
          const metadata_t<MODEL>& meta = MODEL::metadata;

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
      std::map<std::string, std::pair<fields_type, fields_type>> changes_;
  };
}

