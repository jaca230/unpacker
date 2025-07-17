#ifndef ATBANKUNPACKER_HH
#define ATBANKUNPACKER_HH

//Custom
#include "unpacker/common/unpacking/Logger.hh"
#include "unpacker/common/unpacking/UnpackerHelpers.hh"
#include "unpacker/common/unpacking/BankUnpacker.hh"
#include "unpacker/nalu/unpacking/ATPayloadUnpacker.hh"

#define AT_PAYLOAD_ID 0

namespace unpackers {

    class ATBankUnpacker : public BankUnpacker {
        
    public:
        
        //Constructor
        ATBankUnpacker();

        //Destructor
        ~ATBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(ATBankUnpacker,1)

    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // ATBANKUNPACKER_HH