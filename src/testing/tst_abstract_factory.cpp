#define BOOST_TEST_MODULE abstract_factory_test

#include <boost/test/unit_test.hpp>
#include "../abstract_factory.hpp"


class base_product {
public:
	virtual std::string get_value() const = 0;
};

class product_a : public base_product {
public:
    std::string get_value() const override { return "A";};
};

class product_b : public base_product {
public:
    std::string get_value() const override { return "B";};
};


class test_factory : public kw::abstract_factory<base_product> { };


BOOST_AUTO_TEST_CASE(init) {
    test_factory factory;

    BOOST_CHECK(factory.keys().empty());
    BOOST_CHECK(!factory.is_creator_registered("some_key"));
    BOOST_CHECK_THROW(factory.create("some keys"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(register_creator) {
    test_factory factory;

    BOOST_CHECK(factory.register_creator("product_a", [] {return new product_a;}));
    BOOST_CHECK(factory.register_creator("product_b", [] {return new product_b;}));

    BOOST_CHECK(factory.is_creator_registered("product_a"));
    BOOST_CHECK(factory.is_creator_registered("product_b"));
    BOOST_CHECK(!factory.is_creator_registered("invalid key"));

    // try to register the same porducts again
    BOOST_CHECK(!factory.register_creator("product_a", [] {return new product_a;}));
    BOOST_CHECK(!factory.register_creator("product_b", [] {return new product_b;}));
    BOOST_CHECK(factory.is_creator_registered("product_a"));
    BOOST_CHECK(factory.is_creator_registered("product_b"));
}

BOOST_AUTO_TEST_CASE(unregister_creator) {
    test_factory factory;

    // register
    BOOST_REQUIRE(factory.register_creator("product_a", [] {return new product_a;}));
    BOOST_REQUIRE(factory.register_creator("product_b", [] {return new product_b;}));
    BOOST_REQUIRE(factory.is_creator_registered("product_a"));
    BOOST_REQUIRE(factory.is_creator_registered("product_b"));

    // unregister
    BOOST_CHECK(factory.unregister_creator("product_a"));
    BOOST_CHECK(factory.unregister_creator("product_b"));
    BOOST_CHECK(!factory.unregister_creator("invalid key"));
    BOOST_REQUIRE(!factory.is_creator_registered("product_a"));
    BOOST_REQUIRE(!factory.is_creator_registered("product_b"));
}

BOOST_AUTO_TEST_CASE(create) {
    test_factory factory;

    BOOST_REQUIRE(factory.register_creator("product_a", [] {return new product_a;}));
    BOOST_REQUIRE(factory.register_creator("product_b", [] {return new product_b;}));

    BOOST_CHECK_EQUAL(factory.create("product_a")->get_value(), "A");
    BOOST_CHECK_EQUAL(factory.create("product_b")->get_value(), "B");
}

BOOST_AUTO_TEST_CASE(keys) {
    test_factory factory;

    factory.register_creator("product_a", [] {return new product_a;});
    factory.register_creator("product_b", [] {return new product_b;});

    std::list<std::string> actual = factory.keys();
    std::list<std::string> expected = {"product_a", "product_b"};

    BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                  expected.begin(), expected.end());
}
