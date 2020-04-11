#pragma once

#include "modelpp/fields.hpp"

namespace modelpp
{
  using field_types = fields<
    int,
    std::string,
    std::optional<std::string>
      >;

  using field_types_t = field_types::type;
}
