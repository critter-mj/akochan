#pragma once

#include "share/include.hpp"
#include <boost/bind.hpp>
#include "share/types.hpp"
#include "mjai_manager.hpp"

class MJAI_Interface {
    public:
    MJAI_Interface();

    Moves game_record;
    std::array<int, 4> scores_;

    void push(const json11::Json &receive);
    void clear();
    void init_scores();

    Moves get_best_move(const int my_pid, const bool out_console_input);
};

class TcpClient{
    public:
    /// ipとportで指定した相手にソケットを繋げる
    TcpClient(const std::string &ip, const int port);
    ~TcpClient();

    std::string ReadOneLine();
    void SendCommand(const std::string &command);
  
    protected:
    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    boost::system::error_code error_;
    boost::asio::streambuf buffer_;
};