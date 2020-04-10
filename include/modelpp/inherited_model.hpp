#pragma once

#include "inherited_metadata.hpp"

namespace modelpp
{
  class inherited_model : public virtual model
  {
    public:
      template <HasMetadata MODEL>
        void load(MODEL* m, const std::map<std::string, fields_type>& data)
        {
          this->model::load(m, data);
        }

      template<HasInheritedMetadata MODEL>
        void load(MODEL* m, const std::map<std::string, fields_type>& data)
        {
          using parent = decltype(MODEL::metadata)::parent;

          this->model::load(m, data);
          this->load(dynamic_cast<parent*>(m), data);
        }

      template<HasMetadata MODEL>
        std::map<std::string, fields_type> save(MODEL* m, std::map<std::string, fields_type> data)
        {
          return this->model::save(m, std::move(data));
        }

      template<HasInheritedMetadata MODEL>
        std::map<std::string, fields_type> save(MODEL* m)
        {
          return this->save(m, std::map<std::string, fields_type>());
        }

      template<HasInheritedMetadata MODEL>
        std::map<std::string, fields_type> save(MODEL* m, std::map<std::string, fields_type> data)
        {
          using parent = decltype(MODEL::metadata)::parent;

          data = this->model::save(m, std::move(data));

          return this->save(dynamic_cast<parent*>(m), std::move(data));
        }
  };
}
