#define BOOST_TEST_MODULE test modelpp::model
#include <boost/test/unit_test.hpp>

#include "modelpp/fields.hpp"

namespace modelpp
{
  using field_types = fields<int, std::string, std::vector<int>>;
}

#include "modelpp/model.hpp"

class model_container : public modelpp::model
{
  public:
    int id() const                          { return id_; }
    void id(int value)                      { change("id", &id_, value); }

    const std::string& name() const         { return name_; }
    void name(const std::string& value)     { change("name", &name_, value); }

    const std::vector<int>& vec() const     { return vec_; }
    void vec(const std::vector<int>& value) { change("vec", &vec_, value); }

    static modelpp::metadata<model_container> metadata;

    MODELPP_IMPLEMENT_METHODS()

  private:
    int id_;
    std::string name_;
    std::vector<int> vec_;
};

modelpp::metadata<model_container> model_container::metadata{
  modelpp::metadata<model_container>("model_container")
    .field("id", &model_container::id_)
    .field("name", &model_container::name_)
    .field("vec", &model_container::vec_)
};


BOOST_FIXTURE_TEST_SUITE(s, model_container)

BOOST_AUTO_TEST_CASE(test_container_field)
{
  std::vector<int> v;
  v.push_back(1);

  vec(v);
  BOOST_CHECK(changed("vec"));

  auto [old, newv] = changes().at("vec");

  BOOST_CHECK(std::get<std::vector<int>>(old) == std::vector<int>{});
  BOOST_CHECK(std::get<std::vector<int>>(newv) == std::vector<int>{1});

  auto model_data = data();

  vec({});

  load(model_data);

  BOOST_CHECK(vec() == std::vector<int>{1});
}

BOOST_AUTO_TEST_SUITE_END()

