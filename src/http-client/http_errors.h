//
// Created by Domenico Neri on 28/11/22.
//

#ifndef HTTP_CLIENT_HTTP_ERRORS_H
#define HTTP_CLIENT_HTTP_ERRORS_H

#include <boost/asio.hpp>

namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


using namespace boost;

namespace http_errors {
    enum http_error_codes{
        invalid_response = 1
    };

    class http_errors_category: public boost::system::error_category{
    public:
        const char* name() const BOOST_SYSTEM_NOEXCEPT;
        std::string message(int e) const;
    };
    const boost::system::error_category& get_http_errors_category();

    boost::system::error_code make_error_code(http_error_codes e);
} // namespace http_errors

namespace boost {
    namespace system {
        template<>
        struct is_error_code_enum<http_errors::http_error_codes>{
            BOOST_STATIC_CONSTANT(bool, value = true);
        };
    } // namespace system
} // namespace boost


#endif //HTTP_CLIENT_HTTP_ERRORS_H
