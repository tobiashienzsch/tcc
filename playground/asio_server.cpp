#include <cstdlib>

#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

#include <boost/asio.hpp>

#include "tcc/core.hpp"

#include "chat_message.hpp"

using boost::asio::ip::tcp;

using ChatMessageQueue = std::deque<ChatMessage>;

class ChatParticipant
{
public:
    virtual ~ChatParticipant()                   = default;
    virtual void deliver(const ChatMessage& msg) = 0;
};

using ChatParticipantPtr = std::shared_ptr<ChatParticipant>;

class chat_room
{
public:
    void join(ChatParticipantPtr participant)
    {
        participants_.insert(participant);
        for (auto msg : recent_msgs_) participant->deliver(msg);
    }

    void leave(ChatParticipantPtr participant) { participants_.erase(participant); }

    void deliver(const ChatMessage& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs) recent_msgs_.pop_front();

        for (auto participant : participants_) participant->deliver(msg);
    }

private:
    std::set<ChatParticipantPtr> participants_;
    enum
    {
        max_recent_msgs = 100
    };
    ChatMessageQueue recent_msgs_;
};

class chat_session
    : public ChatParticipant
    , public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room) : socket_(std::move(socket)), room_(room) {}

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const ChatMessage& msg) final
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), ChatMessage::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);
                                        do_read_header();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }

    tcp::socket socket_;
    chat_room& room_;
    ChatMessage read_msg_;
    ChatMessageQueue write_msgs_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) : acceptor_(io_context, endpoint)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                std::make_shared<chat_session>(std::move(socket), room_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    chat_room room_;
};

int main(int argc, char* argv[])
{
    auto arguments = gsl::span<char*> {argv, argc};
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i)
        {
            auto const port = std::strtol(arguments[i], nullptr, 10);
            tcp::endpoint endpoint(tcp::v4(), port);
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}