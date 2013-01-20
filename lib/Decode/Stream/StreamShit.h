#pragma once

#include "Helpers.h"
#include "Shit.h"

namespace PddBy
{

class StreamShit : public Shit
{
public:
    PDDBY_INHERIT_SHIT_CTORS(StreamShit, Shit)
};

class EndOfStreamShit : public StreamShit
{
public:
    PDDBY_INHERIT_SHIT_CTORS(EndOfStreamShit, StreamShit)
};

} // namespace PddBy
