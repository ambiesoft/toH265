#include "stdafx.h"

#include "helper.h"
#include "AVDuration.h"


namespace Ambiesoft {
	namespace toH265 {
		String^ AVDuration::ToString() 
		{
			return ToCommonDurationString(*ts_);
			
			//return String::Format(L"{0:00}:{1:00}:{2:00}",
			//	(ts_->Days * 24) + ts_->Hours,
			//	ts_->Minutes,
			//	ts_->Seconds);
			
			//if (ts_->Days != 0)
			//	return String::Format(I18N(L"{0} hours+"), ts_->Days*24);
			//return ts_->ToString("hh\\:mm\\:ss");
		}
	}
}