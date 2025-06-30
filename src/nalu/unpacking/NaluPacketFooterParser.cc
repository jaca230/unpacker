#include "nalu/unpacking/NaluPacketFooterParser.hh"

using namespace parsers;
using json = nlohmann::json;

NaluPacketFooterParser::NaluPacketFooterParser() : Parser() {

    // Set each data location to its default values from new JSON mapping
    parser_index_data_location_ =             DataLocation(0, 32, 2);
    start_udp_packet_index_data_location_ =   DataLocation(0, 48, 2);
    fluff_data_location_ =                     DataLocation(1, 0, 4);     // fluff: word 1, bit 0, 4 bytes
    end_udp_packet_index_data_location_ =     DataLocation(1, 32, 2);    // end_udp_packet_index: word 1, bit 32, 2 bytes
    packet_footer_data_location_ =            DataLocation(1, 48, 2);    // packet_footer: word 1, bit 48, 2 bytes

    // Now look for a configuration file and use that
    const char* unpacker_path = std::getenv("UNPACKER_PATH");
    if (!unpacker_path) {
        utils::LoggerHolder::getInstance().WarningLogger 
            << "In " << className_ << " UNPACKER_PATH not set! Using default values for data location instead.\n" << std::endl;
    } else {

        std::string base_path(unpacker_path);
        std::string file_name = "config/nalu_data_config.json";

        if (!base_path.empty() && base_path.back() != '/') {
            base_path += '/';
        }

        std::string full_path = base_path + file_name;

        std::ifstream file(full_path);
        if (!file) { 
            utils::LoggerHolder::getInstance().WarningLogger 
                << "In " << className_ << " could not open config file here: " << full_path 
                << ". Using default values for data location instead." << std::endl;
        } else {

            json j;
            file >> j;

            const auto& nalu_packet_footer = GetSection(j, "nalu_packet_footer");

            parser_index_data_location_ =             ParseJsonData(GetSection(nalu_packet_footer, "parser_index"));
            start_udp_packet_index_data_location_ =   ParseJsonData(GetSection(nalu_packet_footer, "start_udp_packet_index"));
            fluff_data_location_ =                     ParseJsonData(GetSection(nalu_packet_footer, "fluff"));
            end_udp_packet_index_data_location_ =     ParseJsonData(GetSection(nalu_packet_footer, "end_udp_packet_index"));
            packet_footer_data_location_ =            ParseJsonData(GetSection(nalu_packet_footer, "packet_footer"));
        }
    }

}

NaluPacketFooterParser::~NaluPacketFooterParser() {}

// Method to create the data product, passing all fields including fluff
std::unique_ptr<dataProducts::NaluPacketFooter> 
NaluPacketFooterParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluPacketFooter>(
        GetParserIndex(),
        GetStartUdpPacketIndex(),
        GetFluff(),
        GetEndUdpPacketIndex(),
        GetPacketFooter()
    );
}

// Get methods
uint32_t NaluPacketFooterParser::GetParserIndex()           const { return GetData(parser_index_data_location_); }
uint32_t NaluPacketFooterParser::GetStartUdpPacketIndex()   const { return GetData(start_udp_packet_index_data_location_); }
uint32_t NaluPacketFooterParser::GetFluff()                  const { return GetData(fluff_data_location_); }
uint32_t NaluPacketFooterParser::GetEndUdpPacketIndex()     const { return GetData(end_udp_packet_index_data_location_); }
uint32_t NaluPacketFooterParser::GetPacketFooter()          const { return GetData(packet_footer_data_location_); }

std::ostringstream NaluPacketFooterParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluPacketFooter:" << std::endl;
    oss << "            ParserIndex:           " << std::dec << GetParserIndex() << std::endl;
    oss << "            StartUdpPacketIndex:   " << std::dec << GetStartUdpPacketIndex() << std::endl;
    oss << "            Fluff:                 0x" << std::hex << GetFluff() << std::endl;
    oss << "            EndUdpPacketIndex:     " << std::dec << GetEndUdpPacketIndex() << std::endl;
    oss << "            PacketFooter:          0x" << std::hex << GetPacketFooter() << std::endl;
    return oss;
}

void NaluPacketFooterParser::Print() {
    std::cout << this->Stream().str();
}
