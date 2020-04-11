#define BOOST_TEST_MODULE modelpp::model
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

#include "modelpp/fields.hpp"

using fields_type_t = modelpp::fields<int, std::string>;
using fields_type = fields_type_t::type;

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
}

BOOST_AUTO_TEST_SUITE_END()
