
#include "AIEntity.h"
#include "RoomManager.h"
#include "Player.h"

UINT32 AIEntity::g_id = 1;

AIEntity::AIEntity(const AIContext& aiContext, BTBuilder builder)
    : m_id(g_id++)
    , m_context(aiContext)
{
    m_context.ID = m_id;
}

AIEntity::~AIEntity()
{
}

void AIEntity::Update()
{

}