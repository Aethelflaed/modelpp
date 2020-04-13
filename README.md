# model++

`model++` is a C++20 library containing various modules used to easily represent data models.

The main goal is to provide an easy way to load and extract the field values
from the models, so that the data can be transmitted via a web-service or
stored in a database.

This library provides absolutely no feature related to data storage, but could
easily be used in such a project.

## Install

This project is still in early development, so nothing's done yet to simplify
your life...

Just clone the repo and configure your include path in the meantime.

## Setup

You need to specify the different types your application models will use:

```c++
#include "modelpp/fields.hpp"

namespace modelpp
{
  using field_types = fields<int, std::string>;
}

#include "modelpp/model.hpp"
```

These lines can be put in a single header which you will include instead to load
modelpp.

## Model definition

```c++
// inherit from modelpp::model
class model_a : public modelpp::model
{
  public:
    int id() const { return id_; }
    void id(int value)
    {
      // this tells the model to register the change, and also set the new value
      change("id", &id_, value);
    }

    const std::string& name() const { return name_; }
    void name(const std::string& value)
    {
      // Another example
      change("name", &name_, value);
    }

    // Provides the definition of the different fields
    static modelpp::metadata<model_a> metadata;

    // implement load virtual methods
    // can be replaced by MODELPP_IMPLEMENT_LOAD()
    virtual void load(const modelpp::fields_map& data) override
    { modelpp::model::load(this, data); }

    // implement data virtual methods
    // can be replaced by MODELPP_IMPLEMENT_DATA()
    virtual modelpp::fields_map data(modelpp::fields_map data = {}) override
    { return modelpp::model::data(this, std::move(data)); }

    // both methods can be replaced by MODELPP_IMPLEMENT_METHODS()

  private:
    int id_;
    std::string name_;
};
```

The `data()` and `load()` method simplify the call to the underlying template
method call, which will in turn use the metadata.

The static variable declaration is needed to provide the actual definition of
the fields and to link them to the model.

It should be defined in only one translation unit per target:

```c++
modelpp::metadata<model_a> model_a::metadata{
  // construct a new object
  modelpp::metadata<model_a>("model_a")
    // call field(name, member_ptr) for each neede field
    .field("id", &model_a::id_)
    .field("name", &model_a::name_)
};

// alternative syntax, by providing the map directly

modelpp::metadata<model_b> model_b::metadata{
  "model_b",
  {
    {"id", &model_a::id_},
    {"name", &model_a::name_}
  }
};
```

## Usage

```c++
model_a model;

model.id(420);

model.changed("id"); // true
model.changed("name"); // false

model.load({ {"id", 42}, {"name", "answer"} });

model.changed(); // false

model.id(); // 42
model.name(); // "answer"

model.name("question");

// it returns an unordered_map<string, std::variant<field_types>>
for (const auto& field_value : model.data())
{
  std::cout << field.first << ": ";
  // std::visit can be used to call the lambda on the correct variant value
  std::visit([](auto&& arg){ std::cout << arg; }, field.second);
  std::cout << std::endl;
}

// changes is an unordered_map<string, std::pair<std::variant<field_types std::variant<field_types>>>>
auto name_change = model.changes()["name"];

// old value
std::get<std::string>(name_change.first);

// new value
std::get<std::string>(name_change.second);
```

## Inheritance

Simply inherit from your base model, then you'll need to define the new fields
AND a new metadata static member.

You must define a type `parent_model` to represent the parent model which data
will be loaded / exported along the new model.

The `data()`/`load()` method need to be overriden again to call the generic
method with the right type.

```c++
class model_b : public model_a
{
  public:
    std::string title() const
    {
      return title_;
    }
    void title(const std::string& value)
    {
      // again, change the attribute
      change("title", &title_, value);
    }

    // this is needed to know the parent model
    using parent_model = model_a;

    // again, define the metadata
    static modelpp::metadata<model_b> metadata;

    MODELPP_IMPLEMENT_METHODS()

  private:
    std::string title_;
};

// define the metadata, but you don't need to specify the parent model fields
// only once, again:
modelpp::metadata<model_b> model_b::metadata{"model_b", { {"title", &model_b::title_} } };
```

## `load()`/`data()` first parameter and call ambiguity

You generally won't call these methods from everywhere, and usually limit that
to a place where you make database or web API calls, so it's quite easy
to provide the first parameter.

For the ambiguity, you can use the `HasParentModel` concept to check which
method you should call.

## Design goals

- Simple interface
- Modern C++
- No external compiler to detect fields or whatever
- No MACRO needed, although some may be provided later but just to provide a
  short-hand syntax.

## Features

- model export (`data()`) and load (`load()`) for registered fields
- inherited model support
- tracking changes (`changed()`, `changed(field)`, `changes()`)

## Bugs

- At the moment, `std::variant` has troubles handling `bool` and `std:optional`,
  as identified in [P0608R3](www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0608r3.html)

## TODO

- provides support for nullable data with `std::optional`
- handle containers
- validation module?
- provide overload for `change(field, member_ptr_t, value)` to deduce `member_ptr_t`?
- callback support?
