#ifndef NALUEVENTHEADER_HH   
#define NALUEVENTHEADER_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluEventHeader
     * @brief Represents the header information for a NALU event.
     *
     * Inherits from DataProduct. Stores metadata such as the event index and number of packets.
     */
    class NaluEventHeader : public DataProduct {

        public:
            /** @brief Default constructor. */
            NaluEventHeader();

            /** @brief Constructor with all fields. */
            NaluEventHeader(
                 uint16_t event_header
                ,uint16_t event_info
                ,uint32_t event_index
                ,uint32_t event_reference_time
                ,uint32_t time_threshold               // ← new
                ,uint32_t clock_frequency              // ← new
                ,uint16_t packet_size
                ,uint64_t channel_mask
                ,uint16_t num_windows
                ,uint16_t num_packets);

            /** @brief Destructor. */
            ~NaluEventHeader();

            uint16_t event_header;
            uint16_t event_info;
            uint32_t event_index;
            uint32_t event_reference_time;
            uint32_t time_threshold;
            uint32_t clock_frequency;

            uint16_t packet_size;
            uint64_t channel_mask;
            uint16_t num_windows;
            uint16_t num_packets;

            /** @brief Make a string of this class's contents. */
            std::string String() const;

            /** @brief Display the contents of the NaluEventHeader. */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluEventHeader, 2)  // ← bumped from 1 to 2
    };

    typedef std::vector<NaluEventHeader> NaluEventHeaderCollection;

} // namespace dataProducts

#endif // NALUEVENTHEADER_HH
