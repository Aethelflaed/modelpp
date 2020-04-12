#pragma once

#include "model_a.hpp"
#include "modelpp/inherited_model.hpp"

class model_b : public model_a, public modelpp::inherited_model
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

  private:
    std::string title_;
};

modelpp::metadata<model_b> model_b::metadata{"model_b", {{"title", &model_b::title_}}};

