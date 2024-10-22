// remote_sniffer.cpp

#include <iostream>
#include <pcap.h>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <chrono>

using namespace boost::asio;
using websocket = boost::beast::websocket::stream<ip::tcp::socket>;

// Callback function to handle captured packets
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    auto ws = reinterpret_cast<websocket*>(user_data);
    try {
        ws->write(boost::asio::buffer(packet, pkthdr->len));
    } catch (const std::exception &e) {
        std::cerr << "Error sending packet: " << e.what() << std::endl;
    }
}

int main() {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const char *device = pcap_lookupdev(error_buffer);
    if (device == nullptr) {
        std::cerr << "Failed to find default device: " << error_buffer << std::endl;
        return 1;
    }

    // Open the device for packet capture
    handle = pcap_open_live(device, BUFSIZ, 1, 1000, error_buffer);
    if (handle == nullptr) {
        std::cerr << "Failed to open device: " << error_buffer << std::endl;
        return 1;
    }

    // Setup WebSocket connection
    try {
        io_context io_ctx;
        ip::tcp::resolver resolver(io_ctx);
        auto const results = resolver.resolve("your_server_address", "your_server_port");
        websocket ws(io_ctx);
        connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake("your_server_address", "/");

        // Start capturing packets
        pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&ws));
    } catch (const std::exception &e) {
        std::cerr << "WebSocket error: " << e.what() << std::endl;
    }

    pcap_close(handle);
    return 0;
}
