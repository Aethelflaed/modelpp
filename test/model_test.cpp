#define BOOST_TEST_MODULE test modelpp::model
#include <boost/test/unit_test.hpp>

#include "model_a.hpp"

BOOST_FIXTURE_TEST_SUITE(s, model_a)

BOOST_AUTO_TEST_CASE(test_load)
{
  load({{"id", 420}});

  BOOST_CHECK(id() == 420);

  load({
      {"id", 42},
      {"name", "answer"}
    });

  BOOST_CHECK(id() == 42);
  BOOST_CHECK(name() == "answer");
}

BOOST_AUTO_TEST_CASE(test_changes)
{
  BOOST_CHECK(!changed());
  BOOST_CHECK(!changed("id"));

  id(42);
  BOOST_CHECK(changed());
  BOOST_CHECK(changed("id"));

  load({{"id", 420}});
  BOOST_CHECK(!changed("id"));

  id(42);
  BOOST_CHECK(changed("id"));

  auto [old, newv] = changes().at("id");

  BOOST_CHECK(std::get<int>(old) == 420);
  BOOST_CHECK(std::get<int>(newv) == 42);

  id(1);

  BOOST_CHECK(changed("id"));

  std::tie(old, newv) = changes().at("id");

  BOOST_CHECK(std::get<int>(old) == 420);
  BOOST_CHECK(std::get<int>(newv) == 1);
}

BOOST_AUTO_TEST_CASE(test_data)
{
  auto model_data = data();

  BOOST_CHECK(model_data.contains("id"));
  BOOST_CHECK(model_data.contains("name"));

  BOOST_CHECK(std::get<std::string>(model_data.at("name")) == "");

  id(42);
  name("answer");

  model_data = data();

  BOOST_CHECK(std::get<int>(model_data.at("id")) == 42);
  BOOST_CHECK(std::get<std::string>(model_data.at("name")) == "answer");
}

BOOST_AUTO_TEST_SUITE_END()
