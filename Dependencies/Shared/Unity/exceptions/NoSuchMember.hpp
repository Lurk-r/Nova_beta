#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		enum class ExpectedMemberType : int
		{
			FIELD,
			PROPERTY,
			METHOD
		};

		class NoSuchMember : public ExceptionBase
		{
		public:
			ExpectedMemberType expectedResult;
			std::string classname;
			std::string wantedName;

			explicit NoSuchMember(ExpectedMemberType expectedResult, const std::string& classname, const std::string& wantedName) : ExceptionBase()
			{
				this->expectedResult = expectedResult;
				this->classname = classname;
				this->wantedName = wantedName;

				std::string resultTypeName = "unknown";

				if (expectedResult == ExpectedMemberType::FIELD)
				{
					resultTypeName = "field";
				}
				else if (expectedResult == ExpectedMemberType::PROPERTY)
				{
					resultTypeName = "property";
				}
				else if (expectedResult == ExpectedMemberType::METHOD)
				{
					resultTypeName = "method";
				}

				InitializeException(
					IL2CPP_NAMEOF(NoSuchMember),
					std::format("No such {0} member named \"{1}\" found in {2}.",
						resultTypeName,
						wantedName,
						classname
					)
				);
			}
		};
	}
}
