#ifndef NALUPACKETFOOTERERPARSER_HH
#define NALUPACKETFOOTERERPARSER_HH

// Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluPacketFooter.hh"

namespace parsers {

    class NaluPacketFooterParser : public Parser {
        
    public:
        
        // Constructor
        NaluPacketFooterParser();

        // Destructor
        ~NaluPacketFooterParser();

        // Method to create unique_ptr
        std::unique_ptr<dataProducts::NaluPacketFooter> NewDataProduct();

        // Get methods
        uint32_t GetParserIndex() const;
        uint32_t GetStartUdpPacketIndex() const;    
        uint32_t GetFluff() const;                    // Fluff getter
        uint32_t GetEndUdpPacketIndex() const;      
        uint32_t GetPacketFooter() const;

        // Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketFooterParser";

        DataLocation parser_index_data_location_;
        DataLocation start_udp_packet_index_data_location_; 
        DataLocation fluff_data_location_;                  // Fluff DataLocation
        DataLocation end_udp_packet_index_data_location_;   
        DataLocation packet_footer_data_location_;

    };
}

#endif // NALUPACKETFOOTERERPARSER_HH
