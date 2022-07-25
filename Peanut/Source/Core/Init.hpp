#ifndef __INIT_H__
#define __INIT_H__

namespace pn::internal {

void InitWithoutContext();
void InitWithContext();
void ShutdownWithContext();
void ShutdownWithoutContext();

}

#endif