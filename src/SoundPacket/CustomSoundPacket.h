#pragma once

#include <mc/deps/core/utility/BinaryStream.h>
#include "ll/api/network/packet/Packet.h"
#include <mc/deps/core/utility/ReadOnlyBinaryStream.h>
#include <string>

class TestPacket : public ll::network::PacketBase<TestPacket> {
public:
    std::string soundName;
    uint64     actorID;

    TestPacket() = default;
    TestPacket(const std::string& msg, uint64 count): soundName(msg), actorID(count) {};

    void write(BinaryStream& stream) const override {
    stream.writeString(soundName, "SoundName", "Target sound name.");
    stream.writeUnsignedInt64(actorID, "ActorID", "Actor's unique ID.");
};
    Bedrock::Result<void> read(ReadOnlyBinaryStream& stream) override {
    if (auto res = stream.getString(100); res) {
        soundName = res.value();
    } else {
        return nonstd::make_unexpected(res.error());
    }

    if (auto res = stream.getUnsignedInt64(); res) {
        actorID = res.value();
    } else {
        return nonstd::make_unexpected(res.error());
    }
    return {};
};
};

