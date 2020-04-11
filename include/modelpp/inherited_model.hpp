#pragma once

namespace modelpp
{
  template<class MODEL>
    concept HasParentModel = IsModel<MODEL> &&
    std::is_convertible_v<typename MODEL::parent_model*, model*>;

  class inherited_model
  {
    public:
      template <IsModel MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          m->model::load(m, data);
        }

      template<HasParentModel MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          m->model::load(m, data);
          this->load(dynamic_cast<MODEL::parent_model*>(m), data);
        }

      template<IsModel MODEL>
        fields_map data(MODEL* m, fields_map data)
        {
          return m->model::data(m, std::move(data));
        }

      template<HasParentModel MODEL>
        fields_map data(MODEL* m)
        {
          return this->data(m, {});
        }

      template<HasParentModel MODEL>
        fields_map data(MODEL* m, fields_map data)
        {
          data = m->model::data(m, std::move(data));

          return this->data(dynamic_cast<MODEL::parent_model*>(m), std::move(data));
        }
  };
}
