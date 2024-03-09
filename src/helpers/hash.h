#pragma once

#include <unistd.h>
#include <cstdint>

uint64_t jenkins_one_at_a_time_hash(const uint8_t *data, size_t len);