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
   * Parent class of all the model you want to define
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

      /**
       * Return the data this model represents
       *
       * \tparam MODEL A model which complies with \ref HasMetadata
       *
       * \param m Pointer to the model, to access the metadata
       */
      template<HasMetadata MODEL>
        fields_map data(MODEL* m)
        {
          return data(m, {});
        }

      /**
       * Return the data this model represents, merging it in the given data
       *
       * \tparam MODEL A model which complies with \ref HasMetadata
       *
       * \param m Pointer to the model, to access the metadata
       * \param data The data into which this model's data will be merged
       */
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

