#ifndef _KW_ABSTRACT_FACTORY_INCL_
#define _KW_ABSTRACT_FACTORY_INCL_


#include <map>
#include <string>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <boost/log/trivial.hpp>


namespace kw {

/**
 * abstract factory class
 *
 * provides support for passing arguments to creator functions
 *
 * @code
 * struct product {};
 * struct product_a : public product {};
 * struct product_b : public product {};
 *
 * class custom_factory : public abstract_factory<product> {};
 *
 * custom factory factory;
 * factory.register_creator("product_a", [] { return new product_a(): });
 *
 * @endcode
 */
template<typename T, typename KeyType = std::string, typename... Args>
class abstract_factory {
public:
    using creator_type = std::function<T* (Args... args)>;

	abstract_factory() = default;
	abstract_factory(const abstract_factory&) = default;
	abstract_factory& operator = (const abstract_factory&) = default;
	virtual ~abstract_factory() = default;

    /// returns the list of all registered creators
	std::list<KeyType> keys() const {
		std::list<std::string> result;
		for(auto p : creators) {
			result.push_back(p.first);
		}
		return result;
	}

    /// return new product for the given key
    /// @param args these arguments will be passed to the creator function
    /// @throws std::out_of_range if no such key exists
	std::shared_ptr<T> create(const KeyType &key, const Args&... args) {
		creator_type creator;
		try {
			creator = creators.at(key);
		} catch(const std::out_of_range &error) {
			BOOST_LOG_TRIVIAL(debug) << error.what();
			throw std::out_of_range("no creator for " + key + " registered");
		}
		return std::shared_ptr<T> (creator(args...));
	}

    /// registers the product creator for the given key
    /// returns true on success
	bool register_product(const KeyType &key, creator_type creator) {
		bool registered = false;
		if(is_creator_registered(key)) {
			BOOST_LOG_TRIVIAL(warning) << "creator for \"" << key << "\" already registered, nothing to do";
		} else {
			creators[key] = creator;
			assert(is_creator_registered(key));
			registered = true;
			BOOST_LOG_TRIVIAL(info) << "registered a creator for key \"" << key << "\"";
		}
		return registered;
	}

    /// unregisters a product creator for the given key
    /// returns true on success
	bool unregister_creator(const KeyType &key) {
		bool unregistered = false;
		if(!is_creator_registered(key)) {
			BOOST_LOG_TRIVIAL(warning) << "creator for \"" << key << "\" is not registered, nothing to do";
		} else {
			creators.erase(creators.find(key));
			assert(!is_creator_registered(key));
			unregistered = true;
			BOOST_LOG_TRIVIAL(info) << "unregistered the creator for key \"" << key << "\"";
		}
		return unregistered;
	}

    /// returns true if a creator for the given key is registered
	bool is_creator_registered(const KeyType &key) {
		return creators.find(key) != creators.end();
	}

private:
	std::map<KeyType, creator_type> creators;
};


} // namepsace kw


#endif // _KW_ABSTRACT_FACTORY_INCL_
