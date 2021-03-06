#ifndef EXPECTEXCEPTION_H
#define EXPECTEXCEPTION_H

#include "backtrace.h"
#include "demangle.h"

/**
 * The EXPECT_EXCEPTION macro should ensure that an expression throws an exception of a given type.
 */

class expected_exception: virtual public boost::exception, virtual public std::exception {};
class unexpected_exception: virtual public boost::exception, virtual public std::exception {};
struct unexpected_exception_tag {};
typedef boost::error_info<struct unexpected_exception_tag,boost::exception_ptr> unexpected_exception_info;

struct tag_expected_exception_type {};
typedef boost::error_info<struct tag_expected_exception_type,std::type_info const *> expected_exception_type;

inline
std::string
to_string( expected_exception_type const & x )
    {
    return demangle(*x.value());
    }

#define EXPECT_EXCEPTION(X,F) do { \
    bool thrown = false; \
                         \
    try {                \
        F;               \
    } catch (const X&) { \
        thrown = true;   \
    } catch (const std::exception&) { \
        BOOST_THROW_EXCEPTION(unexpected_exception {}\
            << unexpected_exception_info {boost::current_exception ()} \
            << Backtrace::make () \
            << expected_exception_type {&typeid(X)}); \
    }                    \
                         \
    if (!thrown)         \
        BOOST_THROW_EXCEPTION(expected_exception {} \
            << Backtrace::make () \
            << expected_exception_type {&typeid(X)}); \
} while(false);


#endif // EXPECTEXCEPTION_H
