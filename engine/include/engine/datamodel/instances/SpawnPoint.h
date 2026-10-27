#pragma once

#include <datamodel/instances/BasePart.h>
#include <datamodel/instances/Team.h>

namespace Engine {
	class SpawnPoint : public BasePart {
	public:
		SpawnPoint();

		Team* getTeam() const { return m_team; }
		void setTeam(Team* team) { m_team = team; }

		double getSpawnProtection() const { return m_spawnProtection; }
		void setSpawnProtection(double spawnProtection) { m_spawnProtection = spawnProtection; }

		static void properties(Engine::ClassDescriptor* desc);

	private:
		Team* m_team = nullptr;
		double m_spawnProtection = 15;
	};
}