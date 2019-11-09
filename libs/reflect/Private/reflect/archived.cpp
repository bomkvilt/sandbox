#include "reflect/archived.hpp"
#include "reflect/archiver.hpp"
#include <type_traits>


namespace reflect
{
	void FArchived::Marshal(Archiver& archive) const
	{
		using self_t = std::remove_const_t<std::remove_pointer_t<decltype(this)>>;
		auto* self = const_cast<self_t*>(this);
		self->__archivate(archive, true);
	}

	void FArchived::Unmarshal(const Archiver& archive)
	{
		auto& arch = const_cast<Archiver&>(archive);
		__archivate(arch, false);
	}
}
