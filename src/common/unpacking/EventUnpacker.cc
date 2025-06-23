#include "common/unpacking/EventUnpacker.hh"

ClassImp(unpackers::EventUnpacker)

using namespace unpackers;

EventUnpacker::EventUnpacker()
    : CollectionsHolder(),
    className_("EventUnpacker")
{}

EventUnpacker::~EventUnpacker() {}