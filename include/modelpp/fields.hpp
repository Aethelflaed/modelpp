#pragma once

#include <variant>

namespace modelpp
{
  /**
   * A specialization typedef'd as `modelpp::field_types` of this class is
   * to be used by your application to define the different types model++
   * will have to handle.
   *
   * The types are given in parameter.
   */
  template<typename ...ARGS>
    struct fields
    {
      /**
       * The actual type that will be used to represent a field's value
       */
      using type = std::variant<ARGS...>;

      /**
       * Any specialization of this struct for an actual model class will permit
       * to represent the pointer to the member variables.
       */
      template<class MODEL>
        struct member_ptr
        {
          using type = std::variant<ARGS MODEL::* ...>;
        };

      /**
       * Short-hand for member_ptr<MODEL>::type
       */
      template<class MODEL>
      using member_ptr_t = member_ptr<MODEL>::type;
    };

  /**
   * Compute a pointer to a member field of type TYPE in class MODEL.
   *
   * This struct is used to determine the correct field from a value.
   *
   * Both references and cv-qualifiers are removed from TYPE.
   */
  template<class MODEL, class TYPE>
    struct member_ptr
    {
      using member_type = typename std::remove_cvref_t<TYPE>;
      using type = member_type MODEL::*;
    };

  /**
   * Short-hand for member_ptr<MODEL, TYPE>::type
   */
  template<class MODEL, class TYPE>
    using member_ptr_t = typename member_ptr<MODEL, TYPE>::type;
}
