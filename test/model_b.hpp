#pragma once

#include "model_a.hpp"

class model_b : public model_a
{
  public:
    std::string title() const
    {
      return title_;
    }
    void title(const std::string& value)
    {
      change("title", &title_, value);
    }

    using parent_model = model_a;

    static modelpp::metadata<model_b> metadata;

    MODELPP_IMPLEMENT_METHODS()

  private:
    std::string title_;
};

modelpp::metadata<model_b> model_b::metadata{"model_b", {{"title", &model_b::title_}}};

