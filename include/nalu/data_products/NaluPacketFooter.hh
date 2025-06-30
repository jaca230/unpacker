#ifndef NALUPACKETFOOTER_HH   
#define NALUPACKETFOOTER_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluPacketFooter
     * @brief Represents the footer information for a NALU packet, including fluff.
     *
     * Inherits from DataProduct.
     */
    class NaluPacketFooter : public DataProduct {

        public:
            /**
             * @brief Default constructor.
             */
            NaluPacketFooter();

            /**
             * @brief Constructor with full footer fields including fluff.
             * @param parser_index Parser index value extracted from footer.
             * @param start_udp_packet_index Start UDP packet index.
             * @param end_udp_packet_index End UDP packet index.
             * @param fluff The 4-byte fluff value.
             * @param packet_footer The footer word value.
             */
            NaluPacketFooter(
                uint32_t parser_index,
                uint32_t start_udp_packet_index,
                uint32_t fluff,
                uint32_t end_udp_packet_index,
                uint32_t packet_footer
            );

            /**
             * @brief Destructor.
             */
            ~NaluPacketFooter();

            /**
             * @brief Parser index.
             */
            uint32_t parser_index;

            /**
             * @brief Start UDP packet index.
             */
            uint32_t start_udp_packet_index;

            /**
             * @brief 4-byte fluff data inserted before footer.
             */
            uint32_t fluff;

            /**
             * @brief End UDP packet index.
             */
            uint32_t end_udp_packet_index;

            /**
             * @brief Packet footer word.
             */
            uint32_t packet_footer;

            /**
             * @brief Make a string representation of this class's contents.
             * @return String containing all footer info.
             */
            std::string String() const;

            /**
             * @brief Display the contents of the NaluPacketFooter.
             */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluPacketFooter,1)
    };

    /**
     * @typedef NaluPacketFooterCollection
     * @brief A collection (vector) of NaluPacketFooter objects.
     */
    typedef std::vector<NaluPacketFooter> NaluPacketFooterCollection;

} // namespace dataProducts

#endif // NALUPACKETFOOTER_HH
