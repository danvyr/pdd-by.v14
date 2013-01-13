project(pddby)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(${PROJECT_NAME}_SOURCES
    Decode/Filter/HeaderCheckingFilter.cpp
    Decode/Filter/Image/A8ImageFilter.cpp
    Decode/Filter/Image/BpftcamImageFilter.cpp
    Decode/Filter/Image/BpftHelper.cpp
    Decode/Filter/Image/BpftImageFilter.cpp
    Decode/Filter/Image/UglyPngImageFilter.cpp
    Decode/Filter/NoopFilter.cpp
    Decode/Filter/TripleDes24Filter.cpp
    Decode/Filter/TripleDesFilter.cpp
    Decode/Random/AmPrngRandom.cpp
    Decode/Random/DelphiRandom.cpp
    Decode/Random/RollingStonesRandom.cpp
    Decode/Random/TrashCartPrngRandom.cpp
    Decode/Stream/BaseFilteringReadStream.cpp
    Decode/Stream/FileReadStream.cpp
    Decode/Stream/MemoryReadStream.cpp
    Decode/Stream/MemoryWriteStream.cpp
    Decode/Util/CodepageConverter.cpp
    Decode/Util/Hash.cpp
    Decode/Util/Path.cpp
    Limb/JsonSectionLimb.cpp
    Limb/JsonTopicLimb.cpp
    Oak/Oak14/Oak14ImageLimb.cpp
    Oak/Oak14/Oak14QuestionLimb.cpp
    Oak/Oak14.cpp
    Forest.cpp
)

set(${PROJECT_NAME}_HEADERS
    Decode/IFilter.h
    Decode/Filter/HeaderCheckingFilter.h
    Decode/Filter/Image/A8ImageFilter.h
    Decode/Filter/Image/BpftcamImageFilter.h
    Decode/Filter/Image/BpftHelper.h
    Decode/Filter/Image/BpftImageFilter.h
    Decode/Filter/Image/UglyPngImageFilter.h
    Decode/Filter/NoopFilter.h
    Decode/Filter/TripleDes24Filter.h
    Decode/Filter/TripleDesFilter.h
    Decode/IReadStream.h
    Decode/Random/AmPrngRandom.h
    Decode/Random/DelphiRandom.h
    Decode/Random/RollingStonesRandom.h
    Decode/Random/TrashCartPrngRandom.h
    Decode/Stream/BaseFilteringReadStream.h
    Decode/Stream/FileReadStream.h
    Decode/Stream/MemoryReadStream.h
    Decode/Stream/MemoryWriteStream.h
    Decode/Stream/StreamShit.h
    Decode/Util/CodepageConverter.h
    Decode/Util/Hash.h
    Decode/Util/Path.h
    Decode/IWriteStream.h
    Limb/JsonSectionLimb.h
    Limb/JsonTopicLimb.h
    Oak/Oak14/Oak14ImageLimb.h
    Oak/Oak14/Oak14QuestionLimb.h
    Oak/Oak14.h
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} jsoncpp)