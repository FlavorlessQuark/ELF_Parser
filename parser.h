#pragma once

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <elf.h>


#include "debug.h"


int read_header_info(int fd, unsigned char *hdr_info);

void print_header_info(unsigned char *hdr_info);
void print_type(char c);
void print_machine(char c);
void print_flags(char c);

void read_header32(int fd, Elf32_Ehdr *header);
void read_header64(int fd, Elf64_Ehdr *header);
