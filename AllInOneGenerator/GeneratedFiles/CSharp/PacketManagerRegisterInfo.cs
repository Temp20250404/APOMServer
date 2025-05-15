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

        _onRecv.Add((ushort)Game.PacketID.ScAcceptParty, MakePacket<SC_ACCEPT_PARTY>);
        _handler.Add((ushort)Game.PacketID.ScAcceptParty, PacketHandler.SC_AcceptParty);

        _onRecv.Add((ushort)Game.PacketID.ScCancelEnteringDungeon, MakePacket<SC_CANCEL_ENTERING_DUNGEON>);
        _handler.Add((ushort)Game.PacketID.ScCancelEnteringDungeon, PacketHandler.SC_CancelEnteringDungeon);

        _onRecv.Add((ushort)Game.PacketID.ScCreateMonster, MakePacket<SC_CREATE_MONSTER>);
        _handler.Add((ushort)Game.PacketID.ScCreateMonster, PacketHandler.SC_CreateMonster);

        _onRecv.Add((ushort)Game.PacketID.ScMakeParty, MakePacket<SC_MAKE_PARTY>);
        _handler.Add((ushort)Game.PacketID.ScMakeParty, PacketHandler.SC_MakeParty);

        _onRecv.Add((ushort)Game.PacketID.ScPrepareEnteringDungeon, MakePacket<SC_PREPARE_ENTERING_DUNGEON>);
        _handler.Add((ushort)Game.PacketID.ScPrepareEnteringDungeon, PacketHandler.SC_PrepareEnteringDungeon);

        _onRecv.Add((ushort)Game.PacketID.ScRemoveCharacter, MakePacket<SC_REMOVE_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScRemoveCharacter, PacketHandler.SC_RemoveCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScResponseEnterDungeonBegin, MakePacket<SC_RESPONSE_ENTER_DUNGEON_BEGIN>);
        _handler.Add((ushort)Game.PacketID.ScResponseEnterDungeonBegin, PacketHandler.SC_ResponseEnterDungeonBegin);

        _onRecv.Add((ushort)Game.PacketID.ScResponseEnterDungeonEnd, MakePacket<SC_RESPONSE_ENTER_DUNGEON_END>);
        _handler.Add((ushort)Game.PacketID.ScResponseEnterDungeonEnd, PacketHandler.SC_ResponseEnterDungeonEnd);

        _onRecv.Add((ushort)Game.PacketID.ScSpawnCharacter, MakePacket<SC_SPAWN_CHARACTER>);
        _handler.Add((ushort)Game.PacketID.ScSpawnCharacter, PacketHandler.SC_SpawnCharacter);

        _onRecv.Add((ushort)Game.PacketID.ScStartAiCalculate, MakePacket<SC_START_AI_CALCULATE>);
        _handler.Add((ushort)Game.PacketID.ScStartAiCalculate, PacketHandler.SC_StartAiCalculate);

        _onRecv.Add((ushort)Game.PacketID.ScStopAiCalculate, MakePacket<SC_STOP_AI_CALCULATE>);
        _handler.Add((ushort)Game.PacketID.ScStopAiCalculate, PacketHandler.SC_StopAiCalculate);

        _onRecv.Add((ushort)Game.PacketID.ScChat, MakePacket<SC_CHAT>);
        _handler.Add((ushort)Game.PacketID.ScChat, PacketHandler.SC_Chat);

        _onRecv.Add((ushort)Game.PacketID.ScKeyinfo, MakePacket<SC_KEYINFO>);
        _handler.Add((ushort)Game.PacketID.ScKeyinfo, PacketHandler.SC_Keyinfo);

        _onRecv.Add((ushort)Game.PacketID.ScMonsterDamaged, MakePacket<SC_MONSTER_DAMAGED>);
        _handler.Add((ushort)Game.PacketID.ScMonsterDamaged, PacketHandler.SC_MonsterDamaged);

        _onRecv.Add((ushort)Game.PacketID.ScMonsterDie, MakePacket<SC_MONSTER_DIE>);
        _handler.Add((ushort)Game.PacketID.ScMonsterDie, PacketHandler.SC_MonsterDie);

        _onRecv.Add((ushort)Game.PacketID.ScPlayerDamaged, MakePacket<SC_PLAYER_DAMAGED>);
        _handler.Add((ushort)Game.PacketID.ScPlayerDamaged, PacketHandler.SC_PlayerDamaged);

        _onRecv.Add((ushort)Game.PacketID.ScPlayerDie, MakePacket<SC_PLAYER_DIE>);
        _handler.Add((ushort)Game.PacketID.ScPlayerDie, PacketHandler.SC_PlayerDie);

        _onRecv.Add((ushort)Game.PacketID.ScPositionSync, MakePacket<SC_POSITION_SYNC>);
        _handler.Add((ushort)Game.PacketID.ScPositionSync, PacketHandler.SC_PositionSync);

        _onRecv.Add((ushort)Game.PacketID.ScMonsterAi, MakePacket<SC_MONSTER_AI>);
        _handler.Add((ushort)Game.PacketID.ScMonsterAi, PacketHandler.SC_MonsterAi);

        _onRecv.Add((ushort)Game.PacketID.ScMonsterRotate, MakePacket<SC_MONSTER_ROTATE>);
        _handler.Add((ushort)Game.PacketID.ScMonsterRotate, PacketHandler.SC_MonsterRotate);

        _onRecv.Add((ushort)Game.PacketID.ScTestPacket, MakePacket<SC_TEST_PACKET>);
        _handler.Add((ushort)Game.PacketID.ScTestPacket, PacketHandler.SC_TestPacket);
    }
}
