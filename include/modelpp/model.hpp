#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include "metadata.hpp"

namespace modelpp
{
  /**
   * The registered types in a variant
   */
  using field_types_t =   field_types::type;
  /**
   * Unordered map associating a field name (`std::string`) to a \ref field_types_t
   */
  using fields_map =      std::unordered_map<std::string, field_types_t>;
  /**
   * A pair of field_types_t representing the old and new value respectively
   */
  using field_change =    std::pair<field_types_t, field_types_t>;
  /**
   * Unordered map associating a field name to its change
   */
  using fields_changes =  std::unordered_map<std::string, field_change>;

  class model;

  /**
   * Check that the given model is indeed a model and \ref HasMetadata
   *
   * \tparam MODEL
   */
  template <class MODEL>
    concept IsModel = std::is_convertible_v<MODEL*, model*> &&
    HasMetadata<MODEL>;

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
   * Parent class of all the model you want to define
   *
   * Subclasses should override save(fields_map) and load(fields_map).
   *
   * To enable inherited model load/export, subclasses should also define
   * a type `parent_model` to their direct parent model.
   */
  class model
  {
    public:
      /**
       * Return true if any change has been registered
       */
      bool changed() const
      {
        return !changes_.empty();
      }

      /**
       * Return true if any change has been registered for the given field_name
       *
       * \param field_name THe name of the field for which to check for changes
       */
      bool changed(const std::string& field_name) const
      {
        return changes_.contains(field_name);
      }

      /**
       * Return the list of changes
       */
      const fields_changes& changes()
      {
        return changes_;
      }

      /**
       * Load the given data in the model
       *
       * Should be overriden by subclasses, which will generally just call
       * \ref load(MODEL*, const fields_map&)
       */
      virtual void load(const fields_map& data) = 0;

      /**
       * Load the given data in the model
       *
       * \post \ref changes() will be empty
       *
       * \tparam MODEL A model which complies with \ref HasMetadata
       *
       * \param m Pointer to the model, to access the metadata
       * \param data The data to load into the model
       */
      template<HasMetadata MODEL>
        void load(MODEL* m, const fields_map& data)
        {
          auto lambda =
            []
            (auto& it, auto arg)
            {
              // deduce the field type from its pointer and get the value
              // held in it->second (which is a std::variant)
              return std::get<member_type_t<decltype(arg)>>(it->second);
            };

          load(m, data, lambda);
        }

      /**
       * Load the given data in the model using lambda
       *
       * \post \ref changes() will be empty
       *
       * \tparam MODEL A model which complies with \ref HasMetadata
       * \tparam MAP A map of field -> values
       * \tparam LAMBDA The lambda type
       *
       * \param m Pointer to the model, to access the metadata
       * \param data The data to load into the model
       * \param lambda A lambda called for each field, with an iterator to the
       *  corresponding field in `data` and the field pointer as parameter.
       *  This lambda should return a value suitable to be assigned to the
       *  instance variable.
       *
       * \see load(MODEL*, const fields_map&) for an example of a lambda to use
       *
       * Another example which could work with
       * [nlohmann::json](https://github.com/nlohmann/json):
       * \code{.cpp}
       * auto lambda =
       *    []
       *    // the second parameter, the field pointer, is ignored
       *    (auto& it, auto)
       *    {
       *      // If you're sure the value is compatible:
       *      return *it;
       *      // otherwise you'll have to validate and cast the input.
       *    };
       * \endcode
       */
      template<HasMetadata MODEL, class MAP, class LAMBDA>
        void load(MODEL* m, const MAP& data, LAMBDA& lambda)
        {
          if constexpr (HasParentModel<MODEL>)
          {
            load(dynamic_cast<MODEL::parent_model*>(m), data, lambda);
          }

          const auto& meta = MODEL::metadata;

          for (auto& [field, variant]: meta.fields)
          {
            if (const auto it = data.find(field); it != data.end())
            {
              // This is the lambda actually assigning the value
              auto lmb =
                [&lambda, &it, &m]
                (auto arg)
                {
                  (*m).*arg = lambda(it, arg);
                };

              std::visit(lmb, variant);
            }
          }

          changes_.clear();
        }

      /**
       * Return the data this model represents
       *
       * Should be overriden by subclasses, which will generally just call
       * \ref data(const MODEL*, fields_map) const
       */
      virtual fields_map data(fields_map data = {}) const = 0;

      /**
       * Return the data this model represents, merging it in the given data
       *
       * \tparam MODEL A model which complies with \ref HasMetadata
       *
       * \param m Pointer to the model, to access the metadata
       * \param model_data The data into which this model's data will be merged
       */
      template<HasMetadata MODEL>
        fields_map data(const MODEL* m, fields_map model_data = {}) const
        {
          if constexpr (HasParentModel<MODEL>)
          {
            model_data = data(dynamic_cast<const MODEL::parent_model*>(m), std::move(model_data));
          }

          const auto& meta = MODEL::metadata;

          for (const auto& [field, variant]: meta.fields)
          {
            auto lambda =
              [&m, &model_data, &field]
              (auto arg)
              {
                model_data[field] = m->*arg;
              };

            std::visit(lambda, variant);
          }

          return model_data;
        }

    protected:
      /**
       * Set the given field to the given value and register the change.
       *
       * \tparam MEMBER_PTR The pointer to the member, auto-detected
       * \tparam VALUE_TYPE The type of the value, auto-detected
       *
       * \param field Name of the field for which to register the change
       * \param member_addr A pointer to the member variable
       * \param value The new value
       */
      template<class MEMBER_PTR, class VALUE_TYPE>
        void change(const std::string& field, MEMBER_PTR* member_addr, VALUE_TYPE value)
        {
          if (auto it = changes_.find(field); it != changes_.end())
          {
            it->second.second = value;
          }
          else
          {
            changes_.emplace(field, std::make_pair(*member_addr, value));
          }
          *member_addr = value;
        }

    private:
      fields_changes changes_;
  };
}

#define MODELPP_IMPLEMENT_LOAD() \
    virtual void load(const modelpp::fields_map& data) override\
    {\
      modelpp::model::load(this, data);\
    }

#define MODELPP_IMPLEMENT_DATA() \
    virtual modelpp::fields_map data(modelpp::fields_map data = {}) const override\
    {\
      return modelpp::model::data(this, std::move(data));\
    }


#define MODELPP_IMPLEMENT_METHODS() \
  MODELPP_IMPLEMENT_LOAD()\
  MODELPP_IMPLEMENT_DATA()
