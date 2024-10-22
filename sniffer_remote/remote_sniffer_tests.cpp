
// remote_sniffer_tests.cpp

#define BOOST_TEST_MODULE RemoteSnifferAppTests
#include <boost/test/included/unit_test.hpp>
#include <boost/asio.hpp>
#include <pcap.h>
#include "remote_sniffer.cpp"  // Include the code to be tested

using namespace boost::asio;

BOOST_AUTO_TEST_CASE(test_pcap_open_device)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    const char *device = pcap_lookupdev(error_buffer);
    BOOST_CHECK_MESSAGE(device != nullptr, "Failed to find default device: " << error_buffer);

    if (device != nullptr) {
        pcap_t *handle = pcap_open_live(device, BUFSIZ, 1, 1000, error_buffer);
        BOOST_CHECK_MESSAGE(handle != nullptr, "Failed to open device: " << error_buffer);
        if (handle != nullptr) {
            pcap_close(handle);
        }
    }
}

BOOST_AUTO_TEST_CASE(test_websocket_connection)
{
    try {
        io_context io_ctx;
        ip::tcp::resolver resolver(io_ctx);
        auto const results = resolver.resolve("localhost", "8080");  // Replace with your server address/port
        websocket ws(io_ctx);
        connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake("localhost", "/");
        BOOST_CHECK(true);
    } catch (const std::exception &e) {
        BOOST_FAIL("WebSocket connection failed: " << e.what());
    }
}

BOOST_AUTO_TEST_CASE(test_packet_handler)
{
    // Simulate a packet to verify the packet handler
    pcap_pkthdr pkthdr;
    pkthdr.len = 64;
    u_char packet[64] = {0};
    io_context io_ctx;
    websocket ws(io_ctx);
    try {
        // Mock WebSocket setup
        ip::tcp::resolver resolver(io_ctx);
        auto const results = resolver.resolve("localhost", "8080");  // Replace with your server address/port
        connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake("localhost", "/");

        // Use the packet handler
        packet_handler(reinterpret_cast<u_char*>(&ws), &pkthdr, packet);
        BOOST_CHECK(true);
    } catch (const std::exception &e) {
        BOOST_FAIL("Packet handler failed: " << e.what());
    }
}
