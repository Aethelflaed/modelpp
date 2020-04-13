#pragma once

namespace modelpp
{
  /**
   * Check that the given model respects \ref IsModel and defines a
   * `parent_model` type that does too
   *
   * \tparam MODEL
   */
  template<class MODEL>
    concept HasParentModel = IsModel<MODEL> &&
    IsModel<typename MODEL::parent_model>;

  /**
   * Inherit this class and a subclass of model to enable recursive load/export
   * of data up the inheritance tree.
   *
   * You should also define a `parent_model` type to reference the parent model
   *
   */
  class inherited_model
  {
    public:
      /**
       * Load the model.
       * \see model::load()
       */
      template <IsModel MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          m->model::load(m, data);
        }

      /**
       * Load the current model and then load its parent
       */
      template<HasParentModel MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          m->model::load(m, data);
          this->load(dynamic_cast<MODEL::parent_model*>(m), data);
        }

      /**
       * Export data from the model
       *
       * \see model::data()
       */
      template<IsModel MODEL>
        fields_map data(MODEL* m, fields_map data)
        {
          return m->model::data(m, std::move(data));
        }


      /**
       * Export the current model and merge with the parent data
       */
      template<HasParentModel MODEL>
        fields_map data(MODEL* m, fields_map data = {})
        {
          data = m->model::data(m, std::move(data));

          return this->data(dynamic_cast<MODEL::parent_model*>(m), std::move(data));
        }
  };
}
