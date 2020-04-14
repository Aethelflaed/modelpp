#pragma once

#include "modelpp/fields.hpp"

namespace modelpp
{
  using field_types = fields<int, std::string>;
}

#include "modelpp/model.hpp"

class model_a : public modelpp::model
{
  public:
    int id() const                      { return id_; }
    void id(int value)                  { change("id", &id_, value); }

    const std::string& name() const     { return name_; }
    void name(const std::string& value) { change("name", &name_, value); }

    static modelpp::metadata<model_a> metadata;

    virtual void load(const modelpp::fields_map& data) override
    { modelpp::model::load(this, data); }

    virtual modelpp::fields_map data(modelpp::fields_map data = {}) const override
    { return modelpp::model::data(this, std::move(data)); }
  private:
    int id_;
    std::string name_;
};

modelpp::metadata<model_a> model_a::metadata{
  modelpp::metadata<model_a>("model_a")
    .field("id", &model_a::id_)
    .field("name", &model_a::name_)
};

