#pragma once

#include <variant>

namespace modelpp
{
  template<typename ...ARGS>
    struct fields
    {
      using type = std::variant<ARGS...>;

      template<class MODEL>
        struct member_ptr
        {
          using type = std::variant<ARGS MODEL::* ...>;
        };

      template<class MODEL>
      using member_ptr_t = member_ptr<MODEL>::type;
    };

  template<class MODEL, class TYPE>
    struct member_ptr
    {
      using member_type = typename std::remove_cvref_t<TYPE>;
      using type = member_type MODEL::*;
    };

  template<class MODEL, class TYPE>
    using member_ptr_t = typename member_ptr<MODEL, TYPE>::type;
}
