#pragma once

#include <datamodel/instances/BasePart.h>
#include <datamodel/instances/Team.h>

namespace Engine {
	class SpawnPoint : public BasePart {
	public:
		SpawnPoint();

		void setTeam(Team* team) { m_team = team; }
		Team* getTeam() const { return m_team; }

		static void properties(Engine::ClassDescriptor* desc);

	private:
		Team* m_team;
	};
}