public class PacketManagerRegisterInfo
{
    // 패킷 수신 및 핸들러를 등록하는 메서드입니다.
    public void Register()
    {
        _onRecv.Add((ushort)Game.PacketID.ScFindIdResponse, MakePacket<SC_FIND_ID_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScFindIdResponse, PacketHandler.SC_FindIdResponse);

        _onRecv.Add((ushort)Game.PacketID.ScFindPwResponse, MakePacket<SC_FIND_PW_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScFindPwResponse, PacketHandler.SC_FindPwResponse);

        _onRecv.Add((ushort)Game.PacketID.ScLoginResponse, MakePacket<SC_LOGIN_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScLoginResponse, PacketHandler.SC_LoginResponse);

        _onRecv.Add((ushort)Game.PacketID.ScRequestItemInfo, MakePacket<SC_REQUEST_ITEM_INFO>);
        _handler.Add((ushort)Game.PacketID.ScRequestItemInfo, PacketHandler.SC_RequestItemInfo);

        _onRecv.Add((ushort)Game.PacketID.ScResponseCharacterInfo, MakePacket<SC_RESPONSE_CHARACTER_INFO>);
        _handler.Add((ushort)Game.PacketID.ScResponseCharacterInfo, PacketHandler.SC_ResponseCharacterInfo);

        _onRecv.Add((ushort)Game.PacketID.ScSignupResponse, MakePacket<SC_SIGNUP_RESPONSE>);
        _handler.Add((ushort)Game.PacketID.ScSignupResponse, PacketHandler.SC_SignupResponse);

        _onRecv.Add((ushort)Game.PacketID.ScCreateMonster, MakePacket<SC_CREATE_MONSTER>);
        _handler.Add((ushort)Game.PacketID.ScCreateMonster, PacketHandler.SC_CreateMonster);

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

        _onRecv.Add((ushort)Game.PacketID.ScTestPacket, MakePacket<SC_TEST_PACKET>);
        _handler.Add((ushort)Game.PacketID.ScTestPacket, PacketHandler.SC_TestPacket);
    }
}
