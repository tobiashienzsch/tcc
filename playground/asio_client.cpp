
#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

#include "chat_message.hpp"
#include "tcc/core.hpp"

using boost::asio::ip::tcp;

using ChatMessageQueue = std::deque<ChatMessage>;

class ChatClient {
 public:
  ChatClient(boost::asio::io_context& io_context,
             const tcp::resolver::results_type& endpoints)
      : io_context_(io_context), socket_(io_context) {
    do_connect(endpoints);
  }

  void write(const ChatMessage& msg) {
    boost::asio::post(io_context_, [this, msg]() {
      bool write_in_progress = !write_msgs_.empty();
      write_msgs_.push_back(msg);
      if (!write_in_progress) {
        do_write();
      }
    });
  }

  void close() {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

 private:
  void do_connect(const tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(
        socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint) {
          if (!ec) {
            do_read_header();
          }
        });
  }

  void do_read_header() {
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(read_msg_.data(), ChatMessage::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec && read_msg_.decode_header()) {
            do_read_body();
          } else {
            socket_.close();
          }
        });
  }

  void do_read_body() {
    boost::asio::async_read(
        socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          } else {
            socket_.close();
          }
        });
  }

  void do_write() {
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_msgs_.front().data(),
                            write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
          } else {
            socket_.close();
          }
        });
  }

 private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  ChatMessage read_msg_;
  ChatMessageQueue write_msgs_{};
};

int main(int argc, char* argv[]) {
  auto const arguments = gsl::span<char*>{argv, argc};
  try {
    if (arguments.size() != 3) {
      std::cerr << "Usage: ChatClient <host> <port>\n";
      return 1;
    }

    auto ioContext = boost::asio::io_context{};
    auto resolver = tcp::resolver(ioContext);
    auto endpoints = resolver.resolve(arguments[1], arguments[2]);
    auto client = ChatClient(ioContext, endpoints);
    auto backgroundThread = std::thread([&ioContext]() { ioContext.run(); });

    auto line = std::array<char, ChatMessage::max_body_length + 1>{};
    while (std::cin.getline(line.data(), line.size())) {
      auto msg = ChatMessage{};
      msg.body_length(std::strlen(line.data()));
      std::memcpy(msg.body(), line.data(), msg.body_length());
      msg.encode_header();
      client.write(msg);
    }

    client.close();
    backgroundThread.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}