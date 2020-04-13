#define BOOST_TEST_MODULE test modelpp::inherited_model
#include <boost/test/unit_test.hpp>

#include "model_b.hpp"

BOOST_FIXTURE_TEST_SUITE(s1, model_b)

BOOST_AUTO_TEST_CASE(test_load)
{
  load({{"id", 420}});

  BOOST_CHECK(id() == 420);

  load({{"title", "test"}});

  BOOST_CHECK(title() == "test");
}

BOOST_AUTO_TEST_CASE(test_data)
{
  id(42);
  name("answer");
  title("of everything");

  auto model_data = data();

  BOOST_CHECK(std::get<int>(model_data["id"]) == 42);
  BOOST_CHECK(std::get<std::string>(model_data["name"]) == "answer");
  BOOST_CHECK(std::get<std::string>(model_data["title"]) == "of everything");
}

BOOST_AUTO_TEST_SUITE_END()

#include "model_c.hpp"

BOOST_FIXTURE_TEST_SUITE(s2, model_c)

BOOST_AUTO_TEST_CASE(test_load)
{
  load({{"id", 420}});

  BOOST_CHECK(id() == 420);

  load({{"title", "test"}, {"number", 3}});

  BOOST_CHECK(title() == "test");
  BOOST_CHECK(number() == 3);
}

BOOST_AUTO_TEST_CASE(test_data)
{
  id(42);
  name("answer");
  title("of everything");
  number(5);

  auto model_data = data();

  BOOST_CHECK(std::get<int>(model_data["id"]) == 42);
  BOOST_CHECK(std::get<std::string>(model_data["name"]) == "answer");
  BOOST_CHECK(std::get<std::string>(model_data["title"]) == "of everything");
  BOOST_CHECK(std::get<int>(model_data["number"]) == 5);
}

BOOST_AUTO_TEST_SUITE_END()
