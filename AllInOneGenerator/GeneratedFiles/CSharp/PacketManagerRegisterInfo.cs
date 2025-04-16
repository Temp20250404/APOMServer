public class PacketManagerRegisterInfo
{
    // 패킷 수신 및 핸들러를 등록하는 메서드입니다.
    public void Register()
    {
        _onRecv.Add((ushort)Game.PacketID.ScLoginResponse, MakePacket<SC_LOGIN_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScLoginResponse, PacketHandler.SC_LoginResponse);

        _onRecv.Add((ushort)Game.PacketID.ScRegisterResponse, MakePacket<SC_REGISTER_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScRegisterResponse, PacketHandler.SC_RegisterResponse);

        _onRecv.Add((ushort)Game.PacketID.ScRemoveCharacter, MakePacket<SC_REMOVE_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScRemoveCharacter, PacketHandler.SC_RemoveCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScSpawnCharacter, MakePacket<SC_SPAWN_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScSpawnCharacter, PacketHandler.SC_SpawnCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScChat, MakePacket<SC_CHAT>);
        _handler.Add((ushort)Game.PacketID.ScChat, PacketHandler.SC_Chat);

        _onRecv.Add((ushort)Game.PacketID.ScKeyinfo, MakePacket<SC_KEYINFO>);
        _handler.Add((ushort)Game.PacketID.ScKeyinfo, PacketHandler.SC_Keyinfo);

        _onRecv.Add((ushort)Game.PacketID.ScPlayerAttack, MakePacket<SC_PLAYER_ATTACK>);
        _handler.Add((ushort)Game.PacketID.ScPlayerAttack, PacketHandler.SC_PlayerAttack);

        _onRecv.Add((ushort)Game.PacketID.ScPlayerDamaged, MakePacket<SC_PLAYER_DAMAGED>);
        _handler.Add((ushort)Game.PacketID.ScPlayerDamaged, PacketHandler.SC_PlayerDamaged);

        _onRecv.Add((ushort)Game.PacketID.ScPlayerDie, MakePacket<SC_PLAYER_DIE>);
        _handler.Add((ushort)Game.PacketID.ScPlayerDie, PacketHandler.SC_PlayerDie);

        _onRecv.Add((ushort)Game.PacketID.ScPositionSync, MakePacket<SC_POSITION_SYNC>);
        _handler.Add((ushort)Game.PacketID.ScPositionSync, PacketHandler.SC_PositionSync);

        _onRecv.Add((ushort)Game.PacketID.ScBossPhase, MakePacket<SC_BOSS_PHASE>);
        _handler.Add((ushort)Game.PacketID.ScBossPhase, PacketHandler.SC_BossPhase);
    }
}
