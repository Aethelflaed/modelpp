#pragma once

namespace modelpp
{
  /**
   * Represents the metadata of the given MODEL.
   *
   * An instance of this class needs stored as a static member of the model
   * with the name `metadata`, so the rest of the library can access it.
   */
  template<class MODEL>
    struct metadata
    {
      using members_ptr_t = typename field_types::member_ptr_t<MODEL>;
      using members_ptr_map = std::unordered_map<std::string, members_ptr_t>;

      /**
       * Construct a metadata by providing the model name
       */
      metadata(const std::string& name)
        :name{name}, fields{}
      {
      }

      /**
       * Construct a metadata by providing the model name and a map of the
       * field names associated to their pointer.
       *
       * Example:
       * \code{.cpp}
       * metadata("model_a", {
       *   {"id", &model_a::id_},
       *   {"name", &model_a::name_}
       * });
       * \endcode
       */
      metadata(const std::string& name, members_ptr_map fields)
        :name{name}, fields{fields}
      {
      }

      /**
       * Define field `name` identified by pointer `ptr` to be part of the model
       *
       * Example:
       * \code{.cpp}
       * metadata("model_a")
       *  .field("id", &model_a::id_)
       *  .field("name", &model_a::name_)
       *  ;
       * \endcode
       */
      metadata& field(const std::string& name, members_ptr_t ptr)
      {
        fields.insert_or_assign(name, ptr);
        return *this;
      }

      /**
       * Model name
       */
      std::string name;

      /**
       * Map associating a field name to its member pointer
       */
      members_ptr_map fields;
    };

  /**
   * Check that the given type has a metadata static member and that this member
   * is a metadata
   *
   * \tparam MODEL
   */
  template<class MODEL>
    concept HasMetadata =
        std::is_convertible_v<decltype(MODEL::metadata)&, metadata<MODEL>&>;
}
