#define BOOST_TEST_MODULE modelpp::model
#include <boost/test/unit_test.hpp>

#include "modelpp/fields.hpp"

namespace modelpp
{
  using field_types = fields<int, std::string>;
  using field_types_t = field_types::type;
}

#include "modelpp/model.hpp"

class model_test : public modelpp::model
{
  public:
    int id() const                      { return id_; }
    void id(int value)                  { change("id", &id_, value); }

    const std::string& name() const     { return name_; }
    void name(const std::string& value) { change("name", &name_, value); }

    static modelpp::metadata_t<model_test> metadata;

  private:
    int id_;
    std::string name_;
};

modelpp::metadata_t<model_test> model_test::metadata{
  modelpp::metadata_t<model_test>("model_test")
    .field("id", &model_test::id_)
    .field("name", &model_test::name_)
};

BOOST_FIXTURE_TEST_SUITE(s, model_test)

BOOST_AUTO_TEST_CASE(test_load)
{
  load(dynamic_cast<model_test*>(this), {{"id", 420}});

  BOOST_CHECK(id() == 420);

  load(dynamic_cast<model_test*>(this), {
      {"id", 42},
      {"name", "answer"}
    });

  BOOST_CHECK(name() == "answer");
  BOOST_CHECK(id() == 42);
}

BOOST_AUTO_TEST_CASE(test_changes)
{
  BOOST_CHECK(!changed());
  BOOST_CHECK(!changed("id"));

  id(42);
  BOOST_CHECK(changed());
  BOOST_CHECK(changed("id"));

  load(dynamic_cast<model_test*>(this), {{"id", 420}});
  BOOST_CHECK(!changed("id"));

  id(42);
  BOOST_CHECK(changed("id"));

  auto id_changes = changes().at("id");

  BOOST_CHECK(std::get<int>(id_changes.first) == 420);
  BOOST_CHECK(std::get<int>(id_changes.second) == 42);

  id(1);

  BOOST_CHECK(changed("id"));

  id_changes = changes().at("id");

  BOOST_CHECK(std::get<int>(id_changes.first) == 420);
  BOOST_CHECK(std::get<int>(id_changes.second) == 1);
}

BOOST_AUTO_TEST_CASE(test_data)
{
  auto model_data = data(dynamic_cast<model_test*>(this));

  BOOST_CHECK(model_data.contains("id"));
  BOOST_CHECK(model_data.contains("name"));

  BOOST_CHECK(std::get<std::string>(model_data.at("name")) == "");

  id(42);
  name("answer");

  model_data = data(dynamic_cast<model_test*>(this));

  BOOST_CHECK(std::get<int>(model_data.at("id")) == 42);
  BOOST_CHECK(std::get<std::string>(model_data.at("name")) == "answer");
}

BOOST_AUTO_TEST_SUITE_END()
