#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int otp_input;

otp_input exit_on_error(otp_input otp, const char *msg, short err_code) {
  if (otp != err_code)
    return otp;

  perror(msg);
  exit(EXIT_FAILURE);
  return otp;
}