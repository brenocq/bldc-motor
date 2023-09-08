//--------------------------------------------------
// BLDC Motor Controller
// error.h
// Date: 2023-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BLDC_UTILS_ERROR_H
#define BLDC_UTILS_ERROR_H

namespace Error {

void hardFault();
void memFault();
void busFault();
void usageFault();

} // namespace Error

#endif // BLDC_UTILS_ERROR_H
