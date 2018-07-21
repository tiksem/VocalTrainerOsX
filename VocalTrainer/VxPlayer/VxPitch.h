//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_VXPITCHDEFENITION_H
#define VOCALTRAINER_VXPITCHDEFENITION_H

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include "Pitch.h"

#include <vector>

struct Interval {
    int startTickNumber;
    int ticksCount;

    Interval(int startTickNumber, int ticksCount);
    Interval() = default;

    int endTickNumber() const;

    bool containsTick(int tick) const;
    bool intersectsWith(int begin, int end) const;
    bool intersectsWith(const Interval& interval) const;
};

struct VxPitch : Interval {
    Pitch pitch;

    VxPitch() = default;
    VxPitch(const Pitch &pitch, int startTickNumber, int ticksCount);

    template<typename Archive>
    void load(Archive & ar, const unsigned int version)
    {
        int pitchIndex = 0;
        ar & pitchIndex;
        pitch = Pitch::fromPerfectFrequencyIndex(pitchIndex);
        ar & startTickNumber;
        ar & ticksCount;
    }

    template<typename Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & pitch.getPerfectFrequencyIndex();
        ar & startTickNumber;
        ar & ticksCount;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

std::ostream& operator<<(std::ostream& os, const VxPitch& pitch);


#endif //VOCALTRAINER_VXPITCHDEFENITION_H
