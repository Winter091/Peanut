#pragma once

namespace pn::internal {

	void InitWithoutContext();
	void InitWithContext();
	void ShutdownWithContext();
	void ShutdownWithoutContext();

}
