#pragma once
#include <stdint.h>
#include <cassert>
#include <map>
#include <bitset>
#include <initializer_list>

/// <summary>
/// Collision Layer�� ����ϴ� Ŭ����.
/// 2024.04.04
/// </summary>
namespace Pg::Engine
{
	class PgLayer
	{
	public:
		//�ִ�, �ּ� ���̾� ����
		static constexpr uint32_t MIN_LAYER = 0;
		static constexpr uint32_t MAX_LAYER = 31;

	public:
		static void Clear();
		static void SetCollisionData(uint32_t _layerID, const std::initializer_list<uint32_t>& _collisionLayers);
		static void SetCollisionLayer(uint32_t _layerID, uint32_t _collisionLayerID, bool _value);
		static bool CanCollide(uint32_t _layerID0, uint32_t _layerID1);

	private:
		static std::map<uint32_t, std::bitset<32>> layerData;
		static void ValidateLayer(uint32_t _layerID);

	};
}
