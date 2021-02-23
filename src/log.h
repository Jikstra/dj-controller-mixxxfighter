#pragma once

#include <Arduino.h>

#ifndef DEBUG_H
#define DEBUG_H

//#define DEBUG // Uncomment to enable debugging
 
#ifdef DEBUG 
  #define IFDEBUG(x) x
  #define IFNDEBUG(x)
  #define DBG(x, ...) println(x, ##__VA_ARGS__)
#else 
  #define IFDEBUG(x)
  #define IFNDEBUG(x, ...) x
  #define DBG(x, ...)
#endif

#endif

void println(char *fmt, ... );
