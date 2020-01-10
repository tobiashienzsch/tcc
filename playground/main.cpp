#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>

struct ParameterMsg
{
    int32_t plugin_id {};
    int32_t parameter_id {};
    float value {};

    template<typename archive>
    void serialize(archive& ar, const unsigned /*version*/)
    {
        ar& plugin_id;
        ar& parameter_id;
        ar& value;
    }
};

void on_send_completed(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
        std::cout << "Send failed: " << ec.message() << "\n";
    else
        std::cout << "Send succesful (" << bytes_transferred << " bytes)\n";
}

namespace ba = boost::asio;
using ba::ip::tcp;

struct IO
{
    boost::asio::streambuf buf;

    void send_asynchronously(tcp::socket& socket)
    {
        ParameterMsg info;

        info.plugin_id    = 1;
        info.parameter_id = 1;
        info.value        = 0.99f;

        {
            std::ostream os(&buf);
            boost::archive::text_oarchive out_archive(os);
            out_archive << info;
        }

        async_write(socket, buf, on_send_completed);
    }
};

int main()
{
    ba::io_service ios;
    tcp::socket s(ios);
    s.connect({{}, 6868});

    IO io;
    io.send_asynchronously(s);

    ios.run();
}