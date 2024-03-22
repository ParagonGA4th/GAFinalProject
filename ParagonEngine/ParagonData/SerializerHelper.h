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
		template <typename T, size_t varCount>
		static void OnDeserializerHelper(T* instance, SerializeVector& sv)
		{
			sv.resize(visit_struct::field_count<T>());

			visit_struct_to_tuple(instance, sv, std::make_index_sequence<varCount>());
		}

		template <typename T>
		static void OnSerializerHelper(T* instance, SerializeVector& sv)
		{
			sv.resize(visit_struct::field_count<T>());

			//거지같은 컴파일 타임 노가다 자동화 필요
			int tMat = 0;

			for (auto& [valName, typeInfo, val] : sv)
			{
				switch (tMat)
				{
				case 0:
				{
					valName = visit_struct::get_name<0, T>();
					val = &(visit_struct::get<0>(*instance));
					typeInfo = typeid(visit_struct::type_at<0, T>).name();
				}
				break;
				case 1:
				{
					valName = visit_struct::get_name<1, T>();
					val = &(visit_struct::get<1>(*instance));
					typeInfo = typeid(visit_struct::type_at<1, T>).name();
				}
				break;
				case 2:
				{
					valName = visit_struct::get_name<2, T>();
					val = &(visit_struct::get<2>(*instance));
					typeInfo = typeid(visit_struct::type_at<2, T>).name();
				}
				break;
				//case 3:
				//{
				//	valName = visit_struct::get_name<3, T>();
				//	val = &(visit_struct::get<3>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<3, T>).name();
				//}
				//break;
				//case 4:
				//{
				//	valName = visit_struct::get_name<4, T>();
				//	val = &(visit_struct::get<4>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<4, T>).name();
				//}
				//break;
				//case 5:
				//{
				//	valName = visit_struct::get_name<5, T>();
				//	val = &(visit_struct::get<5>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<5, T>).name();
				//}
				//break;
				//case 6:
				//{
				//	valName = visit_struct::get_name<6, T>();
				//	val = &(visit_struct::get<6>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<6, T>).name();
				//}
				//break;
				//case 7:
				//{
				//	valName = visit_struct::get_name<7, T>();
				//	val = &(visit_struct::get<7>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<7, T>).name();
				//}
				//break;
				//case 8:
				//{
				//	valName = visit_struct::get_name<8, T>();
				//	val = &(visit_struct::get<8>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<8, T>).name();
				//}
				//break;
				//case 9:
				//{
				//	valName = visit_struct::get_name<9, T>();
				//	val = &(visit_struct::get<9>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<9, T>).name();
				//}
				//break;
				//case 10:
				//{
				//	valName = visit_struct::get_name<10, T>();
				//	val = &(visit_struct::get<10>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<10, T>).name();
				//}
				//break;
				//case 11:
				//{
				//	valName = visit_struct::get_name<11, T>();
				//	val = &(visit_struct::get<11>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<11, T>).name();
				//}
				//break;
				//case 12:
				//{
				//	valName = visit_struct::get_name<12, T>();
				//	val = &(visit_struct::get<12>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<12, T>).name();
				//}
				//break;
				//case 13:
				//{
				//	valName = visit_struct::get_name<13, T>();
				//	val = &(visit_struct::get<13>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<13, T>).name();
				//}
				//break;
				//case 14:
				//{
				//	valName = visit_struct::get_name<14, T>();
				//	val = &(visit_struct::get<14>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<14, T>).name();
				//}
				//break;
				//case 15:
				//{
				//	valName = visit_struct::get_name<15, T>();
				//	val = &(visit_struct::get<15>(*instance));
				// 	typeInfo = typeid(visit_struct::type_at<15, T>).name();
				//}
				//break;
				default:
				{
					assert(false && "지원되지 않는 변수의 개수");
				}
				break;
				}
				tMat++;
			}
		}
	};
}