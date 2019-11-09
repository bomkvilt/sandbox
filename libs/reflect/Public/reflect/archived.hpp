#ifndef REFLECT_ARCHIVED_HPP
#define REFLECT_ARCHIVED_HPP


namespace reflect
{
	struct FArchived
	{	//!^ a basic class for archivable structs
	public:
		virtual void Marshal(class Archiver& archive) const;   //!< store this to the archiver
		virtual void Unmarshal(const class Archiver& archive); //!< load this from the archiver
		virtual void __archivate(class Archiver& archive, bool bMarshal) {} //!< internal function.

	public:
		virtual ~FArchived() = default;
	};
}


#define ARCH_NAME(CATEGORY, ALT_NAME, NAME)												\
	(#CATEGORY != "" && #ALT_NAME != "") ? (#CATEGORY "." #ALT_NAME) :					\
	(#CATEGORY != "" && #ALT_NAME == "") ? (#CATEGORY "." #NAME) :						\
	(#CATEGORY == "" && #ALT_NAME != "") ? (#ALT_NAME) : (#NAME)						\
/**/

#define ARCH_BEGIN(SUPER)																\
	void __archivate(class reflect::Archiver& archive, bool bMarshal) override			\
	{																					\
		SUPER::__archivate(archive, bMarshal);											\
/**/

#define ARCH_FIELD(CATEGORY, ALT_NAME, NAME)											\
		if (bMarshal) { archive.AddField(NAME, ARCH_NAME(CATEGORY, ALT_NAME, NAME)); }	\
		else		  { archive.GetField(NAME, ARCH_NAME(CATEGORY, ALT_NAME, NAME)); }	\
/**/

#define ARCH_END()																		\
	}																					\
/**/


#endif //!REFLECT_ARCHIVED_HPP
