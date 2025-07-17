#ifndef ATPAYLOADUNPACKER_HH
#define ATPAYLOADUNPACKER_HH

#include "unpacker/common/unpacking/Logger.hh"
#include "unpacker/common/unpacking/PayloadUnpacker.hh"
#include "unpacker/common/unpacking/UnpackerHelpers.hh"
#include "unpacker/nalu/unpacking/NaluTimeParser.hh"
#include "unpacker/nalu/data_products/NaluTime.hh"


namespace unpackers {

    class ATPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        ATPayloadUnpacker();

        //Destructor
        ~ATPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluTimePtrCol_;

    private:
        const std::string className_ = "ATPayloadUnpacker";

        //Parsers
        std::unique_ptr<parsers::NaluTimeParser> naluTimeParser_;

    };
}

#endif // ATPAYLOADUNPACKER_HH