#include "stdafx.h"

#include "../../lsMisc/IsFileOpen.h"
#include "../../lsMisc/getStdString.net.h"
#include "../../lsMisc/OpenCommon.h"

#include "toH265.h"
#include "helper.h"
#include "TargetCodecDialog.h"

#include "FormMain.h"

#pragma comment(lib, "shell32.lib")

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Drawing;
		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Threading;

		using namespace Newtonsoft::Json::Linq;

		WaitCursor::WaitCursor()
		{
			if (1 == System::Threading::Interlocked::Increment(counter_))
			{
				cur_ = System::Windows::Forms::Cursor::Current;
				System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;
			}
		}

		WaitCursor::~WaitCursor()
		{
			if (0 == System::Threading::Interlocked::Decrement(counter_))
			{
				System::Windows::Forms::Cursor::Current = cur_;
			}
		}
	}

}