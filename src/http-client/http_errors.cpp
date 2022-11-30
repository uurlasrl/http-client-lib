//
// Created by Domenico Neri on 28/11/22.
//

#include "http_errors.h"

const char *http_errors::http_errors_category::name() const noexcept {
    return "http_errors";
}

std::string http_errors::http_errors_category::message(int e) const {
    switch (e) {
        case invalid_response:
            return "Server response cannot be parsed.";
            break;
        default:
            return "Unknown error.";
            break;
    }
}

boost::system::error_code http_errors::make_error_code(http_error_codes e) {
    return boost::system::error_code(static_cast<int>(e), get_http_errors_category());
}