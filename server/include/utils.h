#pragma once

#include <stdint.h>

typedef int otp_input;

otp_input exit_on_error(otp_input otp, const char* msg, short err_code);
