#ifndef REFLECT_ARCHIVER_HPP
#define REFLECT_ARCHIVER_HPP

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

#include "reflect/archived.hpp"


namespace reflect::traits
{
	namespace EType
	{
		struct eStruct {}; //!< inherited from @FArchived
		struct eSimple {}; //!< default case. io << and >> are required
		struct eVector {}; //!< any stretching array like type
		struct eMap {}; //!< any stretch map like type
		// TODO:: static arrays
	}

	// -----------------| TType - type's type selector

	template<typename T>
	struct TType //!< by default all types are simple or struct
	{
		using type = std::conditional_t< std::is_base_of_v<reflect::FArchived, T>
			, EType::eStruct
			, EType::eSimple>;
		using subtype = void; // conainers
		using keytype = void; // associated containers
	};

	template<typename T> //!< helper
	using TType_t = typename TType<T>::type;

	template<typename T> //!< helper
	using TType_s = typename TType<T>::subtype;

	template<typename T> //!< helper
	using TType_k = typename TType<T>::keytype;

	// -----------------| custom types

	template<typename T> //!< std::vector
	struct TType<std::vector<T>> 
	{ 
		using type = EType::eVector; 
		using subtype = T;
	};

	template<typename K, typename V>
	struct TType<std::map<K, V>> //!< std::map
	{
		using type = EType::eMap;
		using subtype = V;
		using keytype = K;
	};

	template<typename K, typename V>
	struct TType<std::unordered_map<K, V>> //!< std::map
	{
		using type = EType::eMap;
		using subtype = V;
		using keytype = K;
	};
}


namespace reflect
{
	namespace pt = boost::property_tree;

	class Archiver final : public boost::noncopyable
	{	//!^ archivator
	public:
		// load the json's data
		Archiver& Load(const std::string& json);

		// serialize self to a json
		std::string Save() const;

	public:
		template<typename T>
		Archiver& AddField(const T& t, const std::string& full_name)
		{
			tree.put_child(full_name, Marshal(t));
			return *this;
		}

		template<typename T>
		const Archiver& GetField(T& t, const std::string& full_name) const
		{
			if (auto o = tree.get_child_optional(full_name); o)
			{
				Unmarshal(t, o.value());
			}
			return *this;
		}

	private:
		pt::ptree tree;

	private:
		template<typename T>
		static pt::ptree Marshal(const T& t)
		{
			return Marshaller<T, traits::TType_t<T>>()(t);
		}

		template<typename T>
		static void Unmarshal(T& t, const pt::ptree& root)
		{
			Unmarshaller<T, traits::TType_t<T>>()(t, root);
		}

		template<typename T, typename K>
		struct Marshaller //!< marshal a simple type
		{
			pt::ptree operator()(const T& t)
			{
				auto root = pt::ptree();
				root.put_value(t);
				return root;
			}
		};

		template<typename T>
		struct Marshaller<T, traits::EType::eStruct>
		{
			pt::ptree operator()(const T& t)
			{
				auto ar = Archiver();
				t.Marshal(ar);
				return ar.tree;
			}
		};

		template<typename T>
		struct Marshaller<T, traits::EType::eVector>
		{
			pt::ptree operator()(const T& t)
			{
				auto root = pt::ptree();
				for (auto&& field : t)
				{
					auto child = Marshal(field);
					root.push_back({ "", child });
				}
				return root;
			}
		};

		template<typename T>
		struct Marshaller<T, traits::EType::eMap>
		{
			pt::ptree operator()(const T& t)
			{
				auto root = pt::ptree();
				for (auto&& [key, field] : t)
				{
					auto ss = std::stringstream(); ss << key;
					auto child = Marshal(field);
					root.add_child(ss.str(), child);
				}
				return root;
			}
		};

		template<typename T, typename K>
		struct Unmarshaller //!< unmarshal a simple type
		{
			void operator()(T& t, const pt::ptree& root)
			{
				if (auto o = root.get_value_optional<T>(); o)
				{
					t = o.value();
				}
			}
		};

		template<typename T>
		struct Unmarshaller<T, traits::EType::eStruct>
		{
			void operator()(T& t, const pt::ptree& root)
			{
				auto ar = Archiver();
				ar.tree = root;
				t.Unmarshal(ar);
			}
		};

		template<typename T>
		struct Unmarshaller<T, traits::EType::eVector>
		{
			using S = traits::TType_s<T>;

			void operator()(T& t, const pt::ptree& root)
			{
				if (auto o = root.get_child_optional(""); o)
				{
					for (auto&& [subname, subroot] : o.value())
					{
						auto elem = S();
						Unmarshal(elem, subroot);
						t.push_back(elem);
					}
				}
			}
		};

		template<typename T>
		struct Unmarshaller<T, traits::EType::eMap>
		{
			using S = traits::TType_s<T>;
			using K = traits::TType_k<T>;

			void operator()(T& t, const pt::ptree& root)
			{
				if (auto o = root.get_child_optional(""); o)
				{
					for (auto&& [subname, subroot] : o.value())
					{
						auto key = K(); std::stringstream(subname) >> key;
						auto val = S(); Unmarshal(val, subroot);
						t[key] = val;
					}
				}
			}
		};
	};
}


#endif //!REFLECT_ARCHIVER_HPP
