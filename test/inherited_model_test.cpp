#define BOOST_TEST_MODULE test modelpp::inherited_model
#include <boost/test/unit_test.hpp>

#include "model_a.hpp"
#include "model_b.hpp"

BOOST_FIXTURE_TEST_SUITE(s, model_b)

BOOST_AUTO_TEST_CASE(test_load)
{
  inherited_model::load(dynamic_cast<model_b*>(this), {{"id", 420}});

  BOOST_CHECK(id() == 420);

  inherited_model::load(dynamic_cast<model_b*>(this), {{"title", "test"}});

  BOOST_CHECK(title() == "test");
}

BOOST_AUTO_TEST_CASE(test_data)
{
  id(42);
  name("answer");
  title("of everything");

  auto data = inherited_model::data(dynamic_cast<model_b*>(this));

  BOOST_CHECK(std::get<int>(data["id"]) == 42);
  BOOST_CHECK(std::get<std::string>(data["name"]) == "answer");
  BOOST_CHECK(std::get<std::string>(data["title"]) == "of everything");
}

BOOST_AUTO_TEST_SUITE_END()
