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
    };
}
