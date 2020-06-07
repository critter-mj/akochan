#include "mjai_client.hpp"

MJAI_Interface::MJAI_Interface() {
    clear();
    init_scores();
}

void MJAI_Interface::init_scores(){
    std::fill(scores_.begin(), scores_.end(), 25000);
}

void MJAI_Interface::clear() {
    game_record.clear();
}

void MJAI_Interface::push(const json11::Json &receive){
    std::string type = receive["type"].string_value();
    if(type == "hello")
        return;
    if(type == "start_kyoku"){
        if(!receive["scores"].is_null()) {
            for(int i=0; i<4; i++) {
                scores_[i] = receive["scores"].array_items()[i].int_value();
            }
        }
        json11::Json json = json11::Json::object({
            {"bakaze", receive["bakaze"].string_value()},
            {"dora_marker", receive["dora_marker"].string_value()},
            {"honba", receive["honba"].int_value()},
            {"kyotaku", receive["kyotaku"].int_value()},
            {"kyoku", receive["kyoku"].int_value()},
            {"oya", receive["oya"].int_value()},
            {"scores", scores_},
            {"tehais", receive["tehais"].array_items()},
            {"type", "start_kyoku"}
        });
        game_record.emplace_back(json);
    } else {
        game_record.push_back(receive);
    }

    if (receive["scores"].is_null())
        return;

    for (int i = 0; i < 4; i++)
        scores_[i] = receive["scores"].array_items()[i].int_value();
}

void MJAI_Interface::push_start_game(const json11::Json &receive, const int kyoku_first) {
    assert_with_out(receive["type"].string_value() == "start_game", "push_start_game type error");
    // kyoku_first: 4 -> tonpu match, 0 -> tonnan match
    json11::Json::object obj = receive.object_items();
    obj["kyoku_first"] = kyoku_first;
    game_record.push_back(json11::Json(obj));
}

Moves MJAI_Interface::get_best_move(const int my_pid, const bool out_console_input) {
    if (out_console_input) {
        for (int i = 0; i < game_record.size(); i++) {
            std::cout << game_record[i].dump() << std::endl;
        }
    }
    return ai(game_record, my_pid, out_console_input);
}

TcpClient::TcpClient(const std::string &ip, const int port){
    socket_.reset(new boost::asio::ip::tcp::socket(io_service_));
    socket_->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port), error_);
    if(error_){
        std::cerr << "connect failed: " << error_.message() << std::endl;
        exit(1);
    }else{
        std::cerr << "successfully connected to " << ip << ":" << port << std::endl;
    }
}

TcpClient::~TcpClient(){
    socket_->close();
}

std::string TcpClient::ReadOneLine(){
    /// 出来ればメンバ変数buffer_の方に読み込みたいが，buffer_.consume(size)を行なっているにも関わらず
    /// 以前読み込んだものが残ってしまうため，ひとまず関数ローカルのbufferに読みこむことで対処する．
    boost::asio::streambuf buffer;  
    //size_t size = boost::asio::read_until(*socket_, buffer, "\n", error_);  ///< 1行読み込み
    boost::asio::read_until(*socket_, buffer, "\n", error_);  ///< 1行読み込み  
    if(error_){
        std::cerr << "read_until failed: " << error_.message() << std::endl;
        exit(1);
    }
    std::string ret = boost::asio::buffer_cast<const char *>(buffer.data());
    //  std::cout << "buffer_.size(): " << buffer.size() << ", size: " << size << std::endl;
    //  buffer_.consume(size);
    //  std::cout << "received: " << ret << std::endl;
    return ret;
}

void TcpClient::SendCommand(const std::string &command){
    boost::asio::write(*socket_, boost::asio::buffer(command), error_);
    if(error_){
        std::cerr << "write failed: " << error_.message() << std::endl;
        exit(1);
    }
}

