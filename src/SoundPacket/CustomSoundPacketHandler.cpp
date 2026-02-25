#pragma once
#include "CustomSoundPacket.h"
#include "ClientWorld.h"


class CustomSoundPacketHandler : public ll::network::PacketHandlerBase<CustomSoundPacketHandler, TestPacket> {
public:
    void handlePacket(const NetworkIdentifier& netId, NetEventCallback& callback, const TestPacket& packet) const {
        World::playSoundAtEntity(packet.actorID, packet.soundName);
};
};