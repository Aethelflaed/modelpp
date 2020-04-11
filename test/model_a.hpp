#pragma once

#include "types.hpp"

#include "modelpp/model.hpp"

class model_a : public modelpp::model
{
  public:
    int id() const                      { return id_; }
    void id(int value)                  { change("id", &id_, value); }

    const std::string& name() const     { return name_; }
    void name(const std::string& value) { change("name", &name_, value); }

    static modelpp::metadata_t<model_a> metadata;

  private:
    int id_;
    std::string name_;
};

modelpp::metadata_t<model_a> model_a::metadata{
  modelpp::metadata_t<model_a>("model_a")
    .field("id", &model_a::id_)
    .field("name", &model_a::name_)
};

