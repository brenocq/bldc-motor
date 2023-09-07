//--------------------------------------------------
// BLDC Motor Controller
// error.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UTILS_ERROR_H
#define UTILS_ERROR_H

namespace Error {

void hardFault();
void memFault();
void logicFault();

} // namespace Error

#endif // UTILS_ERROR_H
