#pragma once
#include <vector>
#include <string>
#include <cassert>

#include <visit_struct/visit_struct_intrusive.hpp>

using SerializeVector = std::vector<std::tuple<std::string, std::string, void*>>;

namespace Pg::Data
{
	class SerializerHelper
	{
	public:

		template<typename T, size_t... I>
		static void visit_struct_to_tuple(T* instance, SerializeVector& tup, std::index_sequence<I...>)
		{
			// �Ʒ� �ּ��� ������� �ʾҴ� ���� : 
			// ...�� ������ Ÿ�ӿ����� ���� for���̶�� ���� ���� ��,
			// for(tup.at(I)); for(visit_struct::get_name<I, T>()); 
			// for(typeid(visit_struct::type_at<I, T>).name()); for(&(visit_struct::get<I>(*instance)));
			// �ϰ� �ִ� ��
			// 
			//auto& [valName, typeInfo, valAddress] = (tup.at(I), ...);

			//auto temp = (visit_struct::get_name<I, T>(), ...);
			//valName = temp;

			//auto temp2 = (typeid(visit_struct::type_at<I, T>).name(), ...);
			//typeInfo = temp2;

			//auto temp3 = (&(visit_struct::get<I>(*instance)), ...);
			//valAddress = (void*)temp3;

			((
				[&]() 
				{
					auto& [valName, typeInfo, valAddress] = tup.at(I);
					valName = visit_struct::get_name<I, T>();
					typeInfo = typeid(visit_struct::type_at<I, T>).name();
					valAddress = (void*)&(visit_struct::get<I>(*instance));
				}
			()), ...);
		}

	public:
		template <typename T>
		static void OnDeserializerHelper(T* instance, SerializeVector& sv)
		{
			sv.resize(visit_struct::field_count<T>());

			visit_struct_to_tuple(instance, sv, std::make_index_sequence<visit_struct::field_count<T>()>());
		}

		template <typename T>
		static void OnSerializerHelper(T* instance, SerializeVector& sv)
		{
			sv.resize(visit_struct::field_count<T>());
			visit_struct_to_tuple(instance, sv, std::make_index_sequence<visit_struct::field_count<T>()>());
		}
	};
}