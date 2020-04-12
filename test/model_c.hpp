#pragma once

#include "model_b.hpp"

class model_c : public model_b
{
  public:
    int number() const      { return number_; }
    void number(int value)  { change("number", &number_, value); }

    using parent_model = model_b;

    static modelpp::metadata<model_c> metadata;

  private:
    int number_;
};

modelpp::metadata<model_c> model_c::metadata{"model_c", {{"number", &model_c::number_}}};

