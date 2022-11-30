//
// Created by Domenico Neri on 28/11/22.
//

#ifndef HTTP_CLIENT_CLIENT_H
#define HTTP_CLIENT_CLIENT_H

#include "http_errors.h"
#include <map>
#include <sstream>

class HTTPClient;

class HTTPRequest;

class HTTPResponse;

typedef void(*Callback)(const HTTPRequest &request,
                        const HTTPResponse &response,
                        const boost::system::error_code &ec);

class HTTPResponse {
    friend class HTTPRequest;

    HTTPResponse();

public:

    unsigned int get_status_code() const;

    const std::string &get_status_message() const;

    const std::map<std::string, std::string> &get_headers();

    const std::istream &get_response() const;

private:
    asio::streambuf &get_response_buf();

    void set_status_code(unsigned int status_code);

    void set_status_message(const std::string &status_message);

    void add_header(const std::string &name, const std::string &value);

private:
    unsigned int m_status_code; // HTTP status code.
    std::string m_status_message; // HTTP status message.

    // Response headers.
    std::map<std::string, std::string> m_headers;
    net::streambuf m_response_buf;
    std::istream m_response_stream;
};

class HTTPRequest {
    friend class HTTPClient;

    static const unsigned int DEFAULT_PORT = 80;

    HTTPRequest(asio::io_service &ios, unsigned int id);

public:
    void set_host(const std::string &host);

    void set_port(unsigned int port);

    void set_uri(const std::string &uri);

    void set_callback(Callback callback);

    std::string get_host() const;

    unsigned int get_port() const;

    const std::string &get_uri() const;

    unsigned int get_id() const;

    void execute();

    void cancel();

private:
    void on_host_name_resolved(const boost::system::error_code &ec, asio::ip::tcp::resolver::iterator iterator);

    void on_connection_established(const boost::system::error_code &ec, asio::ip::tcp::resolver::iterator iterator);

    void on_request_sent(const boost::system::error_code &ec, std::size_t bytes_transferred);

    void on_status_line_received(const boost::system::error_code &ec, std::size_t bytes_transferred);

    void on_headers_received(const boost::system::error_code &ec, std::size_t bytes_transferred);

    void on_response_body_received(const boost::system::error_code &ec, std::size_t bytes_transferred);

    void on_finish(const boost::system::error_code &ec);

private:
    // Request parameters.
    std::string m_host;
    unsigned int m_port;
    std::string m_uri;

    // Object unique identifier.
    unsigned int m_id;

    // Callback to be called when request completes.
    Callback m_callback;

    // Buffer containing the request line.
    std::string m_request_buf;

    asio::ip::tcp::socket m_sock;
    asio::ip::tcp::resolver m_resolver;

    HTTPResponse m_response;

    bool m_was_cancelled;
    std::mutex m_cancel_mux;

    asio::io_service &m_ios;
};

class HTTPClient {
public:
    HTTPClient();

    std::shared_ptr<HTTPRequest> create_request(unsigned int id);

    void close();

private:
    asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;
};

#endif //HTTP_CLIENT_CLIENT_H
