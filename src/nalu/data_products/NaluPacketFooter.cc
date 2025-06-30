#include "nalu/data_products/NaluPacketFooter.hh"
#include <sstream>
#include <iostream>

using namespace dataProducts;

NaluPacketFooter::NaluPacketFooter()
    : DataProduct(),
      parser_index(0),
      start_udp_packet_index(0),
      fluff(0),
      end_udp_packet_index(0),
      packet_footer(0)
{}

/**
 * @brief Construct a NaluPacketFooter with all fields initialized.
 * @param parser_index_arg Parser index extracted from packet footer.
 * @param start_udp_packet_index_arg Starting UDP packet index.
 * @param fluff_arg 4-byte fluff value.
 * @param end_udp_packet_index_arg Ending UDP packet index.
 * @param packet_footer_arg Footer word value.
 */
NaluPacketFooter::NaluPacketFooter(
    uint32_t parser_index_arg,
    uint32_t start_udp_packet_index_arg,
    uint32_t fluff_arg,
    uint32_t end_udp_packet_index_arg,
    uint32_t packet_footer_arg
) : DataProduct(),
    parser_index(parser_index_arg),
    start_udp_packet_index(start_udp_packet_index_arg),
    fluff(fluff_arg),
    end_udp_packet_index(end_udp_packet_index_arg),
    packet_footer(packet_footer_arg)
{}

NaluPacketFooter::~NaluPacketFooter() {}

std::string NaluPacketFooter::String() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluPacketFooter: " << std::endl;
    oss << "    parser_index: " << std::dec << parser_index << std::endl;
    oss << "    start_udp_packet_index: " << std::dec << start_udp_packet_index << std::endl;
    oss << "    fluff: 0x" << std::hex << fluff << std::endl;
    oss << "    end_udp_packet_index: " << std::dec << end_udp_packet_index << std::endl;
    oss << "    packet_footer: 0x" << std::hex << packet_footer << std::endl;
    return oss.str();
}

void NaluPacketFooter::Show() const {
    std::cout << this->String();
}
