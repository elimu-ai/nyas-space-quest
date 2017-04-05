//
//  androidFix.h
// Nya
//
//  Created by German Torres on 12/28/15.
//
//

#ifndef androidFix_h
#define androidFix_h
//android doesnt have to_string
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace std
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream ss;
		ss << n;
		return ss.str();
	}
}
#endif

#endif /* androidFix_h */
